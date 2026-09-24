# Codexion — étapes d’implémentation

Cette feuille de route décrit la suite du développement, dans l’ordre. Le [sujet original](subject.md) et sa [traduction française](subject_fr.md) restent les références pour les exigences.

## État actuel — 24 septembre 2026

| Partie | Avancement |
| --- | --- |
| Parsing et messages d’erreur | En place ; continuer les vérifications aux limites lors des tests finaux. |
| Allocation des tableaux de coders et de dongles | En place. |
| Identifiants, configuration commune et liens vers les dongles | En place dans `init_codexion`. |
| Initialisation des mutex | En place, avec destruction des mutex déjà initialisés en cas d’échec. |
| Création et attente des threads | En place ; la routine affiche seulement un message puis se termine. |
| Nettoyage | Présent pour le lancement actuel ; à étendre aux attentes, au moniteur et aux futures allocations. |
| File FIFO/EDF | Une liste chaînée existe ; elle reste à remplacer par un véritable tas. |
| Cycle des coders, cooldown, moniteur et arrêt partagé | À réaliser. |

La création des threads fonctionne comme point de départ, mais la simulation complète n’est pas encore implémentée. Les messages de démarrage actuels devront laisser place aux logs du sujet.

## Organisation retenue

- Une configuration commune `t_config`, accessible en lecture seule via `const t_config *config`.
- Un tableau `array_coder` contenant les structures de tous les coders.
- Un tableau `array_dongle` contenant les structures de tous les dongles.
- Une allocation par tableau, sans liste chaînée de suivi des allocations.
- Un thread par coder, dont l’identifiant est stocké dans le champ `pthread_t` de sa structure. `pthread_create` crée le thread ; allouer le tableau ne le crée pas.
- Un mutex par dongle, à initialiser explicitement avec `pthread_mutex_init`.

La configuration et les tableaux doivent rester valides tant que les threads les utilisent. On libère chaque tableau en entier, jamais une de ses cases séparément. Les tas servant aux files FIFO/EDF auront également besoin d’une gestion de leur mémoire.

## 1. Associer les dongles aux coders — en place

Chaque coder conserve deux pointeurs vers ses dongles voisins, qui sont des éléments de `array_dongle`.

Pour le coder d’indice `i`, une convention possible est :

```text
premier dongle : array_dongle[i]
second dongle  : array_dongle[(i + 1) % count]
```

Les indices commencent à zéro ; les identifiants affichés commencent à un. Cette formule suppose `count > 0`.

Avec un seul coder, les deux pointeurs désignent le même dongle. Ce cas doit être traité spécialement : le coder ne peut pas obtenir deux dongles distincts et il ne faut pas tenter de verrouiller deux fois le même mutex comme s’il s’agissait de deux ressources.

**À vérifier :** les voisins partagent bien un dongle et le dernier coder est relié au premier.

## 2. Initialiser les mutex des dongles — en place

Parcourir le tableau et appeler `pthread_mutex_init` sur chaque mutex. Vérifier le résultat de chaque appel : zéro signifie succès.

Conserver le nombre de mutex initialisés. Si une initialisation échoue, détruire uniquement ceux dont l’initialisation a réussi, puis libérer les tableaux.

Mettre les octets à zéro avec `ft_calloc` ne remplace pas l’initialisation d’un mutex.

**À vérifier :** le nettoyage fonctionne aussi après une initialisation partielle.

## 3. Préparer les données communes de simulation

Prévoir notamment :

- L’instant de départ commun.
- Un indicateur d’arrêt partagé.
- Un mutex pour sérialiser les logs.
- Les informations nécessaires au moniteur pour suivre les coders et le quota.
- Les mécanismes permettant de réveiller les threads en attente lors de l’arrêt.

Définir quelles données chaque mutex protège et dans quel ordre les mutex peuvent être acquis. Le dernier début de compilation et le compteur de compilations sont écrits par les coders et lus par le moniteur : ces accès doivent être synchronisés. Un indicateur d’arrêt partagé doit lui aussi être protégé.

**À vérifier :** aucune donnée partagée modifiable n’est lue ou écrite sans le mécanisme de synchronisation prévu.

## 4. Préparer les fonctions de temps et les logs

- Obtenir le temps en millisecondes avec une base temporelle cohérente.
- Afficher les horodatages relativement au départ de la simulation.
- Produire exactement les messages imposés par le sujet.
- Protéger les affichages pour éviter le mélange de deux lignes.
- Rendre les attentes interruptibles à l’arrêt, plutôt que de retarder inutilement la fin du programme.

Le délai de burnout court depuis le **début** de la dernière compilation :

```text
deadline = last_compile_start + time_to_burnout
```

Avant la première compilation, le point de départ est le début de la simulation. La compilation, le debug, le refactor et l’attente des dongles consomment ce délai.

**À vérifier :** la précision permet l’affichage du burnout dans les 10 ms exigées.

## 5. Construire l’acquisition et la libération des dongles

Commencer par construire le tas indépendamment des threads. Le [guide illustré du tas](tas_codexion.md) explique sa représentation, les échanges, l’ajout et le retrait. La structure actuelle avec `next` représente une liste chaînée, même si elle trie les demandes par deadline.

Une fois les opérations du tas vérifiées, les intégrer à l’arbitrage et protéger leurs accès concurrents. Retirer une demande du tas ne signifie pas que le coder possède déjà ses deux dongles : cette attribution dépend aussi de leur disponibilité et du cooldown.

Gérer ensemble :

- L’état disponible ou occupé de chaque dongle.
- Son instant de libération et son cooldown.
- Les demandes des coders et leur arbitrage.
- Les files de priorité implémentées avec un tas, comme l’exige le sujet.
- FIFO : ordre d’arrivée des demandes.
- EDF : échéance de burnout la plus proche, avec un départage déterministe des égalités.
- La prévention des interblocages et de la famine.

Un mutex seul ne garantit ni FIFO ni EDF. La prise des deux dongles doit être conçue pour éviter que tous les coders gardent chacun une ressource en attendant indéfiniment la seconde.

**À vérifier :** exclusion mutuelle, respect du cooldown et de l’ordre d’arbitrage, progression sous EDF lorsque les paramètres le permettent.

## 6. Écrire le comportement d’un coder

Le cycle est :

```text
Attendre les deux dongles
        ↓
Compiler
        ↓
Libérer les dongles
        ↓
Déboguer
        ↓
Refactoriser
        ↓
Recommencer
```

Mettre à jour le dernier début de compilation lorsque la compilation commence, et compter les compilations terminées lorsqu’elles se terminent. Vérifier l’arrêt aux endroits nécessaires, notamment pendant les attentes.

**À vérifier :** les ressources détenues sont correctement relâchées lorsqu’un arrêt survient pendant le cycle.

## 7. Écrire le moniteur

Un thread séparé doit :

- Détecter le premier burnout.
- Détecter lorsque tous les coders ont atteint le quota de compilations.
- Signaler l’arrêt de la simulation.
- Faire réveiller les threads bloqués en attente de ressources ou de conditions.

Le burnout d’un coder arrête toute la simulation. Il ne provoque pas la libération immédiate de sa case dans le tableau.

**À vérifier :** détection et log du burnout dans les délais, arrêt au quota et absence de threads qui restent bloqués.

## 8. Compléter le lancement et le nettoyage

L’ordre général dans le programme est :

```text
Parser les arguments
        ↓
Allouer les tableaux
        ↓
Initialiser les données, liens, mutex et autres synchronisations
        ↓
Créer les threads et coordonner leur départ
        ↓
Exécuter la simulation et détecter sa fin
        ↓
Signaler l’arrêt et réveiller les attentes
        ↓
Attendre les threads avec pthread_join
        ↓
Détruire les mutex et variables de condition initialisés
        ↓
Libérer les tableaux et les autres allocations
```

Un thread peut commencer à s’exécuter immédiatement après `pthread_create`. Les données qu’il utilise doivent donc être prêtes, et son départ doit être coordonné avec celui des autres threads.

Prévoir aussi l’échec de création après le lancement de seulement quelques threads : arrêter et rejoindre ceux qui ont réellement été créés, puis nettoyer les ressources initialisées.

**À vérifier :** sorties normales, burnout, quota atteint et échecs partiels sans fuite mémoire, accès invalide ni attente bloquée. Utiliser Valgrind et des tests adaptés aux accès concurrents.

## Priorité immédiate

Les étapes 1 et 2 sont déjà présentes. La prochaine étape est le **tas**, en commençant sans concurrence pour comprendre et vérifier chaque opération.

1. Lire le [guide illustré du tas appliqué à Codexion](tas_codexion.md).
2. Remplacer les maillons par un tableau et distinguer capacité allouée et nombre de demandes présentes.
3. Définir les informations d’une demande et la comparaison FIFO/EDF, y compris les égalités.
4. Écrire l’ajout avec remontée, puis l’extraction avec descente.
5. Vérifier les cas vide, un élément, plein, deadlines égales et extractions successives. Vérifier que les coders restent à leur adresse d’origine.
6. Reprendre les étapes 3 et 4 : état commun, temps et logs.
7. Intégrer le tas à l’acquisition des dongles, puis écrire le cycle, le moniteur et l’arrêt coordonné.

Lorsque la routine des threads ne se terminera plus immédiatement, le nettoyage après un échec de création devra d’abord demander l’arrêt et réveiller les threads déjà lancés, avant de les rejoindre.
