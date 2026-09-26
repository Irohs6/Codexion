# Codexion — étapes d’implémentation

État relu le **26 septembre 2026**, à partir des sources présentes et du [sujet original](subject.md), notamment ses sections V, VI et VII. La [traduction française](subject_fr.md) reste disponible.

Cette feuille de route distingue les mécanismes écrits des exigences encore à réaliser ou à vérifier. Les logs fournis montrent des exécutions cohérentes ; ils ne prouvent pas à eux seuls la conformité de tous les cas concurrents.

## 1. Ce qui est déjà en place

| Partie | État dans le code |
| --- | --- |
| Arguments | Huit arguments obligatoires, nombres non négatifs, contrôle de plage, scheduler `fifo` ou `edf`, refus de zéro coder. |
| Erreurs | Identifiants et messages centralisés, affichage sur `stderr`, nom de l’argument concerné. |
| Convention de retour | **`TRUE = 0`, `FALSE = -1`**, conformément au choix du projet. Comparaisons explicites. |
| Mémoire | Un tableau de coders et un tableau de dongles ; allocation vérifiée, nettoyage normal et certains échecs partiels. |
| Association des dongles | Deux voisins par coder ; pointeurs rangés dans l’ordre du tableau pour verrouiller le plus petit ID d’abord. |
| Threads | Un thread par coder, puis `pthread_join` avant destruction des mutex et libération normale. |
| Temps | `gettimeofday` converti en millisecondes ; un même instant initial attribué à tous les coders. |
| Cycle | Inscription → attente → compilation → libération → debug → refactor → cycle suivant. |
| Compteur | `nb_compile` augmente après une compilation terminée. |
| Tas | Un tableau de deux demandes intégré à chaque dongle ; ajout, consultation et retrait de la racine. |
| Inscription | Une seule inscription par cycle ; les deux mutex sont détenus pendant les inscriptions et la vérification de capacité. |
| Attribution | Le coder doit être premier dans les deux tas, les deux dongles libres et leurs cooldowns terminés. Réservation et retrait des deux demandes sous mutex. |
| FIFO | L’ordre d’insertion est conservé. |
| EDF | La plus petite deadline passe en tête ; les égalités conservent l’ordre d’insertion. |
| Cooldown | L’heure de libération est enregistrée pour les deux dongles ; `cooldown_ready` vérifie le délai sous leurs mutex. |
| Logs des phases | Les textes de prise, compilation, debug et refactor sont présents, avec timestamp relatif et ID. |
| Makefile | Sources et headers explicites, flags requis et dépendances sur les headers. Pas de `wildcard`. |

**Le debug, le refactor et le cooldown ne sont plus des étapes à écrire : ils sont intégrés.** La priorité est maintenant la surveillance et l’arrêt de toute la simulation.

## 2. Organisation actuelle

| Fichier | Rôle |
| --- | --- |
| `main.c` | Parsing, lancement, attente des threads, destruction et libération. |
| `parser.c`, `config.h` | Validation et stockage des paramètres. |
| `error.c`, `error.h` | Catalogue des erreurs et affichage. |
| `memory_manager.c` | Allocation à zéro et libération des tableaux. |
| `codexion.c` | Initialisation des coders/dongles, ordre des mutex, création des threads. |
| `codexion2.c` | Temps, réservation, compilation, routine des threads et départ commun. |
| `codexion3.c` | Vérification du cooldown. |
| `simulation.c` | Debug et refactor. |
| `requests.c` | Préparation et inscription des demandes sous mutex. |
| `heap.c`, `heap.h` | Tas de demandes de capacité deux. |

Les tas sont intégrés aux structures des dongles : ils ne demandent pas d’allocation séparée. Deux cases suffisent tant qu’un coder ne peut avoir qu’une demande par dongle, puisqu’un dongle n’a que deux voisins. Le cas d’un seul coder n’inscrit qu’une demande dans son unique tas.

`request.deadline` est calculée depuis `last_time_compile_start`. Le champ `coder.deadline` existe mais n’est pas actuellement mis à jour ni utilisé pour ce calcul. `arrival_order` vaut actuellement zéro et n’est pas utilisé par le comparateur : l’ordre d’insertion assure le départage actuel. Ce ne sont pas deux compteurs déjà fonctionnels.

## 3. Prochaine étape : état commun et synchronisation

Avant d’écrire la boucle du moniteur, prévoir les données qu’elle partagera avec les coders :

- Un indicateur d’arrêt commun à toute la simulation.
- La cause de fin : burnout, quota atteint ou erreur d’exécution.
- Un mutex pour protéger cet état.
- Un mutex commun pour les affichages.
- L’accès aux coders, à leurs compteurs et à leur dernier début de compilation.
- Le thread du moniteur et les informations nécessaires à son nettoyage.

Une structure commune est une possibilité, sans variable globale. Choisir explicitement quels mutex protègent quelles données, puis un ordre de verrouillage commun.

Actuellement, chaque coder modifie son compteur et son dernier début de compilation dans son propre thread. Dès que le moniteur les lira pendant l’exécution, **lectures et écritures devront être synchronisées**. Protéger seulement les lectures du moniteur ne suffit pas.

Conserver la convention `TRUE = 0`, `FALSE = -1`. Un champ mis à zéro par `ft_calloc` n’est donc pas automatiquement « faux » : initialiser les futurs indicateurs d’arrêt à `FALSE` explicitement.

## 4. Ajouter le moniteur de burnout obligatoire

Le sujet exige un **thread de surveillance séparé**. Il n’existe pas encore.

Pour chaque coder surveillé, calculer :

```text
échéance = dernier début de compilation + time_to_burnout
burnout lorsque le temps actuel atteint ou dépasse cette échéance
```

Avant la première compilation, le dernier début correspond à l’instant initial commun, déjà enregistré. Ne jamais repousser la deadline simplement parce qu’un coder attend un dongle.

Le moniteur doit :

1. Lire un état cohérent des coders.
2. Détecter le premier burnout.
3. Déclencher l’arrêt commun.
4. Afficher une seule annonce `timestamp X burned out`, dans les **10 ms** suivant le burnout réel.
5. Faire terminer les autres threads, y compris ceux qui attendent une ressource.

Prévoir l’arbitrage entre la décision du moniteur et un coder qui s’apprête à commencer une compilation : ne pas laisser une nouvelle mise à jour du temps masquer un burnout déjà atteint.

## 5. Arrêt global au quota et attentes interruptibles

Aujourd’hui, chaque coder termine sa propre boucle après son quota. Le programme attend ensuite tous les threads. Ce n’est pas encore une décision d’arrêt global au moment où tous ont terminé assez de compilations.

En particulier, chaque coder exécute encore `debug` et `refactor` après sa dernière compilation. **Quand tous les quotas sont atteints, le sujet demande l’arrêt de la simulation**, sans attendre inutilement ces dernières phases.

À réaliser :

- Détecter que tous les compteurs ont atteint le quota et déclencher l’arrêt commun.
- Faire consulter cet arrêt par chaque routine.
- Rendre interruptibles compilation, debug, refactor et attente de dongles.
- Lors d’un arrêt, libérer correctement les dongles détenus et abandonner les demandes restantes.
- Éviter qu’un thread en échec laisse les autres attendre indéfiniment.

Les appels actuels à `usleep` couvrent toute une phase ; ils ne consultent pas d’indicateur d’arrêt. Une attente par courtes tranches avec vérification de l’arrêt, ou une attente temporisée adaptée, reste à concevoir. Les variables de condition sont autorisées, mais leur utilisation n’est pas obligatoire.

## 6. Cas d’un seul coder

Le code reconnaît que les deux pointeurs désignent le même dongle et évite le double verrouillage. Toutefois, `take_dongle` retourne toujours `FALSE`, et la boucle d’attente n’a pas de condition d’arrêt : **elle reste infinie pour un quota positif**.

Le comportement à compléter est : impossibilité de compiler avec deux dongles, attente surveillée, burnout au délai prévu, arrêt et nettoyage. Ne pas inventer un second dongle et ne pas rejeter arbitrairement ce cas prévu par le sujet.

## 7. Logs : format présent, sérialisation à ajouter

Les textes actuellement affichés correspondent aux phases du sujet :

```text
timestamp X has taken a dongle
timestamp X has taken a dongle
timestamp X is compiling
timestamp X is debugging
timestamp X is refactoring
```

Mais la section VI exige explicitement **un mutex pour protéger les sorties**. Aucun mutex commun de log n’est encore présent. Un affichage propre sur les essais ne remplace pas cette exigence.

Prévoir une fonction d’affichage commune qui :

- Protège les logs avec le mutex commun.
- Utilise un timestamp relatif au départ de la simulation.
- Coordonne l’affichage avec l’arrêt afin d’éviter les messages normaux après le burnout.
- Permet l’unique message de burnout.

Les deux prises et le début de compilation sont actuellement imprimés ensemble dans `compile`, après la réservation. Vérifier leur placement temporel lors de l’intégration de l’arrêt et du moniteur. Aucun message de « fin de refactor » n’est demandé.

## 8. Arbitrage et progression : à confirmer par des tests

Les opérations de tas et leur usage sont écrits. La règle EDF actuelle départage les deadlines égales par l’ordre d’insertion ; documenter cette règle et vérifier sa cohérence entre les deux files. Si `arrival_order` doit servir de numéro explicite, son attribution reste à implémenter.

Le sujet exige aussi l’absence de famine sous EDF lorsque les paramètres sont faisables. **L’ordre des mutex prévient les cycles de verrouillage, mais ne démontre pas cette propriété d’ordonnancement.** Vérifier notamment si l’attente simultanée de la première place dans deux files retarde inutilement des compilations possibles et provoque un burnout évitable.

Les exemples à quatre coders montrent un ordre très régulier, souvent une compilation à la fois. Cela ne suffit pas à prouver la progression pour d’autres durées, d’autres nombres de coders ou des deadlines différentes.

Le cooldown est implémenté : vérifier maintenant sa justesse, plutôt que le réécrire. Tester une reprise exactement à la fin du délai, un cooldown nul et le démarrage où les dongles n’ont encore jamais été utilisés.

## 9. Départ, erreurs et nettoyage à compléter

Un timestamp commun existe, mais les threads commencent dès leur création ; il n’y a pas encore de départ coordonné avec le moniteur. Pour un grand nombre de threads ou un petit délai de burnout, le temps passé à les créer peut compter avant que les derniers aient commencé à travailler. Prévoir un protocole de départ cohérent.

Le nettoyage normal est présent. Pour la suite :

- Créer et rejoindre aussi le moniteur.
- À l’échec d’une création partielle, demander l’arrêt avant d’attendre les threads déjà lancés.
- Traiter les échecs système pertinents pendant la simulation ; plusieurs retours de mutex et le retour de `gettimeofday` ne sont pas vérifiés actuellement.
- Détruire uniquement les mutex et éventuelles conditions initialisés, après la fin des threads utilisateurs.
- Vérifier les branches d’échec de `join`, de destruction et d’allocation sans accès invalide ni fuite.

La conversion des durées en microsecondes utilise actuellement `int * 1000`. Vérifier les grandes valeurs acceptées par le parsing : cette multiplication peut déborder avant l’appel à `usleep`. Ce point concerne la gestion des durées, pas une demande de remplacement du parsing.

## 10. Norme, tests et README

Contrôles réalisés lors de cette mise à jour :

- Vérification des sources avec `cc -Wall -Wextra -Werror -pthread -fsyntax-only` : réussie. Ce contrôle ne réalise pas l’édition des liens.
- Norminette sur les fichiers C et headers : deux erreurs relevées, les autres fichiers passent.
  - `codexion3.c` : saut de ligne final manquant, `BRACE_SHOULD_EOL`.
  - `simulation.c` : lignes vides consécutives après les includes, `CONSECUTIVE_NEWLINES`.
- Aucun code modifié pour cette mise à jour documentaire ; pas de nouvelle campagne de tests concurrents exécutée ici.

Avant de considérer le projet terminé :

- [ ] Compiler avec le Makefile et vérifier qu’un deuxième `make` ne relie pas inutilement.
- [ ] Corriger les deux erreurs de norme.
- [ ] Tester FIFO et EDF, les deadlines égales et la capacité des tas.
- [ ] Tester un coder, deux coders, un nombre impair et un grand nombre de coders.
- [ ] Tester le burnout pendant l’attente, la compilation, le debug et le refactor.
- [ ] Vérifier le délai maximal de 10 ms pour le log de burnout.
- [ ] Tester le quota 1, un quota supérieur et le quota 0 actuellement accepté.
- [ ] Vérifier cooldown, exclusion des dongles et absence de messages après l’arrêt.
- [ ] Tester les échecs partiels et la libération de toutes les ressources.
- [ ] Refaire Valgrind et Helgrind après l’intégration du moniteur et de l’arrêt.

Le `README.md` ne contient actuellement que le titre du projet. La section VII du sujet demande un README **en anglais** avec :

- La première ligne en italique : « This project has been created as part of the 42 curriculum by … » avec les logins.
- `Description`.
- `Instructions` : compilation, arguments et exécution.
- `Resources`, avec les références et les usages de l’IA précisés.
- `Blocking cases handled` : interblocages, famine, cooldown, burnout et logs.
- `Thread synchronization mechanisms` : les mécanismes réellement utilisés et les données qu’ils protègent.

## Ordre conseillé à partir de maintenant

1. Préparer l’état partagé, le mutex de log et la protection des données lues par le futur moniteur.
2. Écrire le moniteur de burnout et le brancher au départ de la simulation.
3. Relier l’arrêt à toutes les boucles et attentes, puis au quota global.
4. Finaliser le cas d’un seul coder et le nettoyage des arrêts/échecs.
5. Vérifier la progression FIFO/EDF, les délais et les logs avec des tests ciblés.
6. Terminer le README et les derniers contrôles de norme et de mémoire.

Pour les schémas des tas, voir [le guide](tas_codexion.md) et [l’exemple des files de dongles](tas_dongles_exemple.svg). Les variantes pédagogiques du guide ne remplacent pas la description du tableau fixe de deux demandes utilisée actuellement.
