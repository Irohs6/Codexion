# Codexion — bilan d’avancement et prochaines étapes

Analyse du **1er octobre 2026**, fondée sur les sources actuelles et le [sujet original, version 1.5](subject.md), sections III à VII. La [traduction française](subject_fr.md) est également disponible.

Ce document remplace le bilan du 26 septembre, qui décrivait encore les anciens fichiers et indiquait à tort, pour l’état actuel, que le mutex de log manquait. Aucun code source n’a été modifié pendant cet audit. Les commandes de compilation ont régénéré les objets et l’exécutable.

## 1. Où en est le projet ?

**Le cycle de travail et la gestion des dongles sont opérationnels sur les essais réalisés. La gestion de fin de simulation n’est pas encore terminée.** Le monitoring est le prochain gros chantier, mais il doit être accompagné de la propagation de l’arrêt, de la synchronisation des données surveillées et du nettoyage des nouveaux éléments.

La compilation complète et la Norminette passent. Ce n’est pas encore un projet conforme à toutes les exigences du sujet : le burnout est absent, le cas d’un seul coder ne termine pas et le README obligatoire reste à écrire. Un pourcentage d’avancement serait trompeur : ces éléments représentent peu de fichiers mais une part importante du comportement attendu.

## 2. Comparaison au sujet

| Exigence | État constaté | Suite |
| --- | --- | --- |
| C, aucune variable globale, flags requis | En place à la lecture ; compilation réussie avec `cc -Wall -Wextra -Werror -pthread`. | Conserver ces contraintes. |
| Norme 42 | Tous les `.c` et `.h` passent Norminette. | Recontrôler après chaque ajout. |
| Huit arguments obligatoires | En place, erreurs détaillées sur stderr. | Conserver ; compléter les essais de limites si les types changent. |
| Refus des nombres invalides et scheduler strict | Onze cas invalides testés et correctement rejetés. | Pas de réécriture du parser nécessaire pour le monitoring. |
| Un thread par coder, dongles voisins en cercle | En place. | Ajouter le thread séparé du moniteur. |
| Protection des dongles par mutex | Présente pour disponibilité, cooldown et demandes. | Conserver l’ordre commun des verrouillages. |
| Compile → debug → refactor | Présent, durées en millisecondes converties pour `usleep`. | Rendre les attentes interruptibles et corriger les grandes durées. |
| Réservation des deux dongles | Vérifications et réservation sous les deux mutex. | Intégrer l’arrêt sans perdre une réservation. |
| Cooldown | Présent ; cohérent sur les six scénarios FIFO/EDF testés. | Tester davantage les limites et le cooldown nul. |
| Tas personnalisé FIFO/EDF | Tableau de deux demandes par dongle ; insertion, consultation et retrait présents. | Tester directement l’ordre, les égalités et la progression sous charge. |
| Départage EDF | Les deadlines égales conservent l’ordre d’insertion. | Documenter cette règle et vérifier sa cohérence. |
| Absence de famine EDF si paramètres faisables | Pas démontrée par les essais actuels. | Campagne dédiée après ajout du moniteur. |
| Logs sérialisés | Mutex partagé, format des quatre actions correct. | Coordonner les logs avec l’arrêt et l’annonce du burnout. |
| Burnout et annonce sous 10 ms | Non implémentés ; `monitoring.c` ne contient que son en-tête. | Obligatoire. |
| Arrêt global quand tous ont atteint le quota | Partiel : chaque routine finit son quota puis termine debug/refactor. | Décider de l’arrêt global dès que tous les quotas sont atteints. |
| Un seul coder | Évite le double verrouillage, mais attend indéfiniment avec quota positif. | Le moniteur doit détecter son burnout et faire sortir l’attente. |
| Mémoire et nettoyage | Parcours normal testé sans fuite ; gestion de certains échecs présente. | Compléter le cycle de vie du mutex de simulation et les échecs partiels. |
| Makefile | Règles requises et listes explicites ; pas de relink inutile. | Ajouter `monitoring.h` aux dépendances ; intégrer `monitoring.c` lorsqu’il aura du code. |
| README anglais conforme | Contient uniquement `# Codexion`. | Rédiger les sections imposées par le sujet. |

La convention volontaire **`TRUE = 0`, `FALSE = -1`** est conservée et n’est pas une erreur. Continuer les comparaisons explicites et l’initialisation des indicateurs à `FALSE`.

## 3. Organisation actuelle

| Module | Responsabilité |
| --- | --- |
| `main.c` | Arguments, parser, création/destruction du mutex de log, code de sortie. |
| `simulation.c` | Structure de simulation locale, allocation, lancement, attente et nettoyage normal. |
| `init.c` | ID, liens entre coders/dongles, instant initial commun, lancement de l’initialisation des mutex et threads. |
| `threads.c` | Création/jonction des threads et cycle de vie des mutex de dongles. |
| `coder.c` | Routine, compilation, debug et refactor. |
| `dongle.c` | Test de priorité/disponibilité/cooldown et réservation des deux dongles. |
| `requests.c` | Inscription sous mutex ; demandes initiales impaires puis paires avant les threads. |
| `heap.c` | File de priorité de capacité deux. |
| `time_utils.c` | `gettimeofday` converti en millisecondes. |
| `log.c` | Affichage protégé par le mutex commun. |
| `monitoring.h`, `monitoring.c` | Structure `stop` + mutex présente ; fonctions de surveillance absentes. |
| `parser.c`, `error.c`, `memory_manager.c`, `utils.c` | Parsing, erreurs, allocation/libération, utilitaires. |

Le découpage est cohérent pour poursuivre. Il n’est pas nécessaire de réorganiser encore les fichiers avant le monitoring.

## 4. Ce qui manque concrètement

### A. Utiliser et nettoyer l’état partagé

Dans `run_codexion`, la structure locale `simulation` est bien reliée au manager, puis tous les coders en reçoivent l’adresse. Sur le parcours normal, sa durée de vie couvre les threads puisqu’ils sont rejoints avant le retour de la fonction.

Cependant :

- `simulation.stop` est initialisé à `FALSE`, mais jamais consulté ni modifié ensuite.
- Le retour de `pthread_mutex_init(&simulation.mutex, NULL)` n’est pas vérifié.
- Ce mutex n’est jamais détruit, ni en sortie normale ni dans les sorties d’erreur après son initialisation.

Prochaine réalisation : les deux accès protégés pour lire l’arrêt et le demander, puis un cycle de vie complet du mutex. Aucun `malloc` supplémentaire n’est nécessaire pour cette structure locale.

### B. Protéger les informations lues par le futur moniteur

`last_time_compile_start` et `nb_compile` sont aujourd’hui modifiés par le thread du coder. Le compteur est consulté par le thread principal après les jonctions : cela explique pourquoi le test Helgrind actuel peut être propre.

Dès que le moniteur lit ces champs pendant l’exécution, **leurs lectures et leurs écritures doivent employer la même protection**. Verrouiller uniquement dans le moniteur ne suffit pas. Définir aussi un ordre de verrouillage compatible avec les mutex de dongles et de log.

### C. Ajouter le moniteur séparé

Le sujet exige un thread distinct qui surveille les échéances :

```text
échéance = last_time_compile_start + time_to_burnout
burnout si maintenant >= échéance
```

Avant la première compilation, `last_time_compile_start` contient déjà le départ commun. La durée de compilation compte dans ce délai ; attendre des dongles ne repousse pas l’échéance.

Le moniteur doit accéder aux coders, lire un état cohérent, détecter le burnout, décider de l’arrêt, produire une seule annonce et respecter le délai de 10 ms. Prévoir sa création, sa jonction et leurs échecs.

Coordonner le début d’une compilation avec ce contrôle : une mise à jour tardive du dernier début ne doit pas effacer un burnout déjà atteint. Le départ commun est actuellement fixé avant la création des threads ; prévoir un protocole de démarrage cohérent pour que le moniteur surveille correctement cette phase, surtout avec beaucoup de coders.

### D. Propager l’arrêt partout

La boucle qui attend `take_dongle()` ne vérifie que la réussite de la prise. Les pauses de compilation/debug/refactor sont des `usleep` couvrant toute la phase.

À réaliser :

1. Consulter l’arrêt dans la boucle principale et dans l’attente des dongles.
2. Rendre les pauses interruptibles, par exemple avec de courtes attentes et vérification du temps écoulé.
3. Si une compilation est interrompue, ne pas la compter comme terminée ; libérer les dongles détenus.
4. Abandonner proprement les demandes lorsque la simulation s’arrête. Si tout le monde s’arrête et que les tableaux ne sont libérés qu’après les jonctions, il n’est pas nécessaire de retirer individuellement chaque demande pour éviter une fuite : les tas sont intégrés aux dongles.
5. Arrêter lorsque tous les compteurs atteignent le quota, sans imposer les derniers debug/refactor à tout le monde.
6. Coordonner l’arrêt et le mutex de log pour empêcher un message normal après l’annonce finale du burnout.

Le cas d’un seul coder sera alors couvert : impossible de compiler avec son unique dongle, burnout au délai prévu, sortie de l’attente et nettoyage.

### E. Corriger la conversion des grandes durées

Défaut reproduit avec une compilation temporaire utilisant UBSan :

```sh
./codexion 2 2147483647 2147484 1 1 1 0 fifo
```

Diagnostic dans `coder.c` : `2147484 * 1000` déborde un `int` signé. Le parser accepte cette durée, mais la multiplication est faite en `int` avant l’appel à `usleep`. Debug et refactor emploient la même expression.

Corriger la gestion du temps, sans nécessairement changer le parser. Un calcul dans un type assez large doit intervenir **avant** la multiplication ; de plus, ne pas simplement transmettre une durée gigantesque à `usleep`. L’attente interruptible par petites tranches permet de traiter les deux besoins.

### F. Fiabiliser les sorties d’erreur

À la lecture de `threads.c`, un échec de création partielle conduit à rejoindre les threads déjà lancés sans leur demander de s’arrêter. Or les tas contiennent déjà les demandes de tous les coders, y compris ceux qui ne seront jamais lancés. Les threads existants peuvent attendre ces demandes indéfiniment.

Il faut demander l’arrêt avant les jonctions lors d’un échec partiel. Ce risque est établi par lecture ; aucun échec de `pthread_create` n’a été injecté pendant cet audit.

Dans `finish_codexion`, un échec de jonction fait retourner avant le nettoyage. Traiter cette voie sans libérer des données encore utilisées par un thread. Tester également les échecs d’allocation et d’initialisation des mutex. Contrôler les retours système nécessaires à une sortie cohérente ; `gettimeofday` et plusieurs opérations mutex ne sont pas vérifiés actuellement.

## 5. FIFO/EDF : ce qui est fait et ce qui reste à prouver

Deux cases suffisent pour un dongle partagé par deux voisins si chacun n’y possède qu’une demande. Avec deux éléments, le rangement actuel est suffisant pour maintenir la priorité à la racine ; il n’est pas nécessaire de créer un grand tas dynamique.

- FIFO conserve l’ordre d’inscription.
- EDF place la plus petite deadline en tête.
- Une égalité conserve la demande déjà présente : c’est une règle de départage, pas une absence de règle. Elle doit être expliquée et testée.
- `arrival_order` reste toujours à zéro et ne participe pas au comparateur. `coder.deadline` est également inutilisé ; c’est `request.deadline` qui sert au tas.
- Les demandes initiales sont inscrites avant les threads, impairs puis pairs. Le coder ne se réinscrit pas au premier tour. Cette organisation permet le parallélisme observé.

L’ordre des mutex évite un cycle de verrouillage ; il ne prouve pas l’absence de famine. La priorité simultanée dans deux files peut retarder un coder dont les ressources sont physiquement libres. Une exécution parfois séquentielle n’est pas automatiquement une violation de FIFO. La propriété EDF demandée par le sujet doit être vérifiée sur des cas faisables variés, avec le monitoring actif.

Les variables de condition sont autorisées mais pas obligatoires. L’attente actuelle `usleep(10)` peut multiplier les réveils et mérite une mesure de charge, sans être à elle seule une preuve de non-conformité.

## 6. Vérifications exécutées le 1er octobre

| Contrôle | Résultat |
| --- | --- |
| `make re`, puis `make -q` | Réussite ; aucun rebuild nécessaire après compilation. |
| Norminette sur tous les `.c` et `.h` | Tous passent. |
| `N 1000 30 4 5 3 10 fifo`, N = 2, 4, 7 | Tous terminent avec trois compilations par coder. |
| Même série en EDF | Même résultat. |
| Logs des six essais | Chaque ligne suit le format des actions attendues ; espacements sur dongles partagés compatibles avec compilation + cooldown, avec tolérance de 1 ms liée aux logs. |
| Parallélisme estimé depuis les débuts de compilation | Pics de 1, 2 et 3 pour respectivement 2, 4 et 7 coders, dans les deux modes. |
| Onze entrées invalides | Rejet avec code non nul et erreur sur stderr, sans log de simulation. |
| `1 100 30 4 5 0 10 fifo` | Quota nul accepté, sortie immédiate sans log. |
| `1 100 30 4 5 1 10 edf` | Encore en attente après 1 seconde ; essai interrompu automatiquement. |
| `2 10 30 4 5 1 0 edf` | Termine normalement sans burnout alors que le délai de 10 ms est dépassé. Non conforme, faute de moniteur. |
| `2 5000 10 200 200 1 0 fifo` | Dernière compilation terminée vers 20 ms, refactors affichés à 210/220 ms, sortie vers 422 ms. Arrêt au quota encore tardif. |
| Valgrind Memcheck, `4 1000 10 2 2 2 5 edf` | Zéro erreur, zéro octet alloué restant à la sortie. |
| Helgrind, même scénario | Zéro erreur rapportée. |
| UBSan sur la grande durée décrite plus haut | Débordement signé confirmé. |

Les onze entrées invalides couvrent : arguments manquants/supplémentaires, chaîne vide, `-`, négatif, suffixe alphabétique, décimal, deux dépassements de plage, zéro coder et scheduler `EDF` en majuscules.

**Limites :** cette campagne courte ne prouve ni l’absence universelle de races, ni la précision future du moniteur, ni la progression EDF dans tous les cas faisables. Valgrind sur le parcours normal ne remplace pas les essais d’échecs et ne valide pas le cycle de vie du mutex de simulation. L’estimation du parallélisme par les logs n’est pas une instrumentation exacte de chaque réservation.

## 7. README et Makefile

Le README doit être rédigé en anglais et commencer par la phrase imposée en italique :

```markdown
*This project has been created as part of the 42 curriculum by <login>.*
```

Remplacer le login et ajouter : `Description`, `Instructions`, `Resources` avec l’usage précis de l’IA, `Blocking cases handled`, `Thread synchronization mechanisms`. Expliquer les protections réellement présentes, la règle EDF en cas d’égalité, le cooldown et l’arrêt. Ne pas annoncer comme réalisées des garanties encore non vérifiées.

Dans le Makefile, `monitoring.h` manque à `HEADERS` alors qu’il est déjà inclus par le projet : sa modification seule risque de ne pas recompiler les objets. `monitoring.c` est absent de `SRCS` ; c’est sans effet fonctionnel tant qu’il est vide, mais il faudra l’ajouter lors de l’écriture du moniteur. Garder les listes explicites, sans wildcard.

## 8. Ordre de travail conseillé

1. **Terminer l’état partagé** : accès protégés à `stop`, vérification de création et destruction du mutex, dépendance Makefile.
2. **Protéger les données surveillées** : dernier début de compilation et compteur, avec un ordre de verrouillage défini.
3. **Relier l’arrêt aux routines et aux attentes** : traiter au même endroit les grandes durées et les pauses interruptibles.
4. **Créer le thread moniteur** : départ cohérent, burnout, quota global, annonce unique et délai maximal de 10 ms.
5. **Compléter le nettoyage et les erreurs partielles** : arrêter puis rejoindre tous les threads effectivement créés avant de détruire les ressources.
6. **Valider les cas limites** : un coder, quotas 0/1/plusieurs, burnout pendant chaque phase et pendant l’attente, cooldown 0/positif, égalités EDF, nombres pairs/impairs et charge plus élevée.
7. **Finaliser le README**, puis refaire compilation, Norminette, Valgrind et Helgrind après le monitoring.

La prochaine étape reste donc le monitoring au sens large : surveillance, arrêt partagé et terminaison propre. Le parser, les tableaux mémoire et le cycle des phases n’ont pas besoin d’être reconstruits.
