# Retours et erreurs

## Une seule convention booléenne

`TRUE` vaut **0**, `FALSE` vaut **-1**. Comparer explicitement les statuts à `TRUE` ou `FALSE` : ne pas utiliser `!result` pour tester un échec. Le type `t_bool` désigne un résultat logique.

Les fonctions de validation, de parsing, d’initialisation, de réservation et de modification du tas renvoient `TRUE` si elles réussissent et `FALSE` sinon. Un dongle disponible vaut `TRUE`, un dongle réservé vaut `FALSE`.

```c
if (parse(argc, argv, &config) == FALSE)
    return (EXIT_FAILURE);
```

La conversion numérique conserve la logique initiale : elle renvoie le nombre converti, ou `FALSE` (-1) en cas d’erreur. Zéro est une valeur numérique possible ; seul -1 signale un échec. Le calcul utilise un `long` et vérifie la limite 2147483647 après chaque chiffre.

## Les retours qui ne sont pas des booléens

- `main` renvoie `EXIT_SUCCESS` (0) ou `EXIT_FAILURE` : ce sont des codes de sortie.
- Les appels `pthread_*` gardent leur convention native : 0 pour succès, un code d’erreur sinon. Ne pas comparer leur retour à `TRUE`.
- `ft_calloc` renvoie une adresse ou `NULL` ; il affiche l’erreur avant de renvoyer `NULL`.
- `ft_strlen` renvoie une longueur ; sur un pointeur nul, il affiche l’erreur et renvoie 0. Ce retour seul ne distingue pas le pointeur nul d’une chaîne vide.
- `now_ms` renvoie une date en millisecondes, pas un statut.
- `get_heap_first` renvoie une copie de la demande ; si le tas est vide, son champ `coder` vaut `NULL`.
- Les routines de thread renvoient un `void *`. Le champ `failed` signale un échec d’inscription ; le thread principal le consulte après les `pthread_join`.

## Un identifiant correspond à un message

Le catalogue se trouve dans `error.h` et la sélection du message dans `error.c`.

```c
return (print_error(ERR_RANGE, arg_position));
```

`print_error` reçoit seulement l’identifiant et la position de l’argument (0 hors parsing). Il affiche sur `stderr`, renvoie `FALSE` et ne termine pas le programme. L’appelant propage l’échec et effectue son nettoyage. Éviter de réafficher une erreur déjà signalée par la fonction appelée.

| ID | Nom | Signification |
| --- | --- | --- |
| 1 | ERR_EMPTY | Chaîne vide |
| 2 | ERR_NEGATIVE | Nombre négatif |
| 3 | ERR_NUMBER | Nombre invalide |
| 4 | ERR_RANGE | Entier hors plage |
| 5 | ERR_SCHEDULER | Scheduler inconnu |
| 6 | ERR_MEMORY | Allocation échouée |
| 7 | ERR_NULL | Pointeur nul |
| 8 | ERR_ARG_COUNT | Mauvais nombre d’arguments |
| 9 | ERR_ZERO | Zéro interdit pour cet argument |
| 10 | ERR_THREAD | Création de thread échouée |
| 11 | ERR_MUTEX | Initialisation de mutex échouée |
| 12 | ERR_ALLOC_OVERFLOW | Taille d’allocation excessive |
| 13 | ERR_THREAD_JOIN | Attente de thread échouée |
| 14 | ERR_MUTEX_DESTROY | Destruction de mutex échouée |
| 15 | ERR_COND | Initialisation de condition échouée, réservée pour la suite |
| 16 | ERR_HEAP_FULL | Tas plein |
| 17 | ERR_HEAP_EMPTY | Retrait demandé sur un tas vide |
| 18 | ERR_COUNT | Nombre de ressources non positif |

Les valeurs de l’énumération sont explicites pour que l’ajout d’une erreur ne décale pas les identifiants existants. Les codes affichés ne sont pas les codes de sortie de `main`.

## Inscription dans les tas

`register_requests` verrouille les deux mutex dans l’ordre établi à l’initialisation, vérifie la place dans les deux tas, puis inscrit les demandes avant de déverrouiller. Cela empêche les inscriptions concurrentes de perdre des demandes ou d’établir deux ordres d’arrivée contradictoires. Le même mutex n’est verrouillé qu’une fois dans le cas d’un seul dongle.

L’ordre FIFO et le départage des deadlines égales reposent actuellement sur l’ordre d’insertion. Le champ `arrival_order`, encore inutilisé par le comparateur, est initialisé à zéro ; il ne représente pas encore un compteur d’arrivée.

## Travail restant sur la simulation

Cette harmonisation ne termine pas la simulation : le moniteur, l’arrêt partagé, le cooldown, le debug et le refactor restent à implémenter. Un seul coder ne peut pas obtenir deux dongles : il attend encore indéfiniment tant que le burnout n’est pas géré. L’échec de création partielle et les erreurs système pendant l’exécution devront être raccordés au futur arrêt coordonné. `now_ms` ne propage pas encore un éventuel échec de `gettimeofday`.
