# Audit Codexion — Natah11037 — 21 octobre 2026

## Verdict

**Le projet n’est pas prêt à être rendu.** La simulation fonctionne sur plusieurs scénarios importants, mais le dépôt livré ne contient ni Makefile ni README et comporte des erreurs de norme. Le parseur provoque aussi un crash avec zéro codeur. Dans une évaluation appliquant les critères éliminatoires de la grille, la note serait **0 à ce stade**, sans que cela signifie que toute la simulation soit à refaire.

Dépôt : https://github.com/Natah11037/projects-42 — commit audité `7a0c17cd3f3f5290289ab984fe4725d9d818b8ed`, dossier `Codexion`. Références : sujet Codexion 1.5 conservé dans `docs/subject.md` et grille fournie `Intra Projects Codexion Edit.pdf`. Exécution sous Ubuntu/WSL. Aucun fichier source du dépôt original n’a été modifié ; le recode LIFO utilise une copie temporaire.

Tous les scénarios de commande explicitement présents dans la grille ont été exécutés. Les questions orales, l’explication personnelle du code et la capacité de l’auteur à réaliser lui-même le live coding restent à évaluer avec lui. Cet audit n’est pas une validation officielle de 42.

## Livraison et compilation

- **Makefile absent**, confirmé dans les fichiers suivis par Git. `make` échoue. Les règles obligatoires et l’absence de relink ne peuvent donc pas être validées.
- **README.md absent** : ajouter la première ligne en italique demandée par le sujet, Description, Instructions, Resources, l’usage de l’IA, les cas de blocage traités et les mécanismes de synchronisation.
- Pour poursuivre le diagnostic, tous les `.c` ont été compilés directement avec `cc -Wall -Wextra -Werror -pthread -g` : réussite. Ce contournement ne valide pas la livraison officielle.
- Norminette : **6 erreurs dans 3 fichiers** : `codexion.c:47` (fonction trop longue), `simulation.c:43,45` (tabulations), `simulation.c:56,101` (fonctions trop longues), `simulation_cleanup.c:69` (fonction trop longue).
- Aucune variable globale mutable ni appel explicitement interdit trouvé dans les sources examinées. Le symbole `fwrite` du binaire provient de l’optimisation d’un `fprintf` par le compilateur, et ne suffit pas à conclure à une fonction interdite dans le code.

## Tests de la grille : 23 exécutions

Chaque ligne ci-dessous représente les arguments de `./codexion`. Les exécutions utilisent le binaire compilé depuis les sources, pas l’exécutable déjà présent dans Git.

| Arguments | Répétitions | Résultat observé |
|---|---:|---|
| `1 800 200 200 200 10 0 fifo` | 3 | Burnout à 800 ms, aucune compilation, arrêt |
| `5 2000 200 200 200 10 0 fifo` | 3 | Les 5 codeurs atteignent chacun 10 compilations, aucun burnout |
| `5 2000 200 200 200 7 0 edf` | 3 | Les 5 codeurs atteignent chacun 7 compilations, aucun burnout |
| `5 500 200 200 200 10 0 fifo` | 5 | Burnout à 500–501 ms, dernière ligne du journal |
| `5 3000 200 200 200 10 400 fifo` | 3 | Quotas atteints, aucun burnout |
| `5 3000 200 200 200 10 800 fifo` | 3 | Burnout du codeur 3 vers 3000–3001 ms |
| `5 3000 200 200 200 10 800 edf` | 3 | Burnout du codeur 3 vers 3000–3001 ms |

Le dernier couple sert notamment à comparer les politiques dans la grille ; un résultat semblable sur ce scénario ne prouve pas que l’EDF est absent. Le code EDF compare effectivement les deadlines, et conserve l’ordre existant à égalité. Ces deux scénarios ne sont pas présentés comme des tests ayant atteint les quotas.

Sur ces journaux : format reconnu, deux messages de dongle immédiatement avant chaque compilation, aucune phase observée plus courte que sa durée, aucune violation de la borne minimale compilation + cooldown entre réutilisations d’un même dongle. Le code protège la réservation des deux dongles. Les journaux n’exposent pas la date exacte de libération : cette vérification temporelle constitue une borne nécessaire, pas une mesure exhaustive de chaque état interne.

Les burnouts des tests de référence sont annoncés dans la tolérance de 10 ms. Cette observation sous WSL ne constitue pas une garantie temps réel sous toute charge. Les durées sous Valgrind ne sont pas utilisées pour juger cette tolérance.

## Tests complémentaires

| Vérification | Résultat |
|---|---|
| 14 simulations, 2/3/4/7/31/100/200 codeurs, FIFO et EDF ; autres arguments `10000 60 60 60 3 60` | Aucun burnout ni échec observé |
| 100 simulations de stress, `200 120 60 60 60 3 60`, alternance FIFO/EDF | Aucune ligne après le burnout observée |
| Memcheck, fin normale et burnout | 0 erreur, 0 octet restant alloué |
| Helgrind, fin normale et burnout | 0 erreur détectée |
| DRD, fin normale | 0 erreur détectée |
| 14 injections de panne : 2 allocations, 7 initialisations de mutex, 1 condition, 4 créations de thread | Arrêt avec code 1 ; Memcheck sans erreur ni fuite ; aucun mutex/condition initialisé restant dans le compteur d’instrumentation |
| 71 entrées invalides de parsing | 63 rejets, 6 acceptations incorrectes, 2 crashes |

Les injections incluent l’échec de création du moniteur et plusieurs échecs de création de codeurs. Le nettoyage des initialisations partielles est donc un point positif confirmé sur ces chemins. Un outil sans erreur sur ces tests ne prouve pas l’absence de tout entrelacement problématique.

## Corrections prioritaires

### 1. Refuser réellement zéro codeur et les chaînes vides

[parser.c](https://github.com/Natah11037/projects-42/blob/7a0c17cd3f3f5290289ab984fe4725d9d818b8ed/Codexion/parser.c#L17) : `verify_if_valid_int()` accepte une chaîne vide, puisque la boucle ne s’exécute pas. Elle devient ensuite zéro.

À la ligne 55, `save_config()` affiche une erreur pour zéro codeur mais retourne seulement de sa propre fonction, sans informer `parser()`. Le champ `config->nb_coders` reste non initialisé et le lancement continue.

Reproduction :

```sh
./codexion 0 2000 60 60 60 2 60 fifo
./codexion "" 2000 60 60 60 2 60 fifo
```

Les deux commandes ont produit SIGSEGV. Memcheck sur zéro codeur signale des valeurs non initialisées puis des accès invalides. Les chaînes vides aux positions 2 à 7 sont également acceptées.

**À faire :** rejeter la chaîne vide avant conversion ; faire remonter l’échec de validation de `save_config()` vers `parser()` puis `main()` ; ne pas lancer la simulation après cette erreur. Initialiser la configuration est utile, mais ne remplace pas la propagation de l’échec.

### 2. Uniformiser les conventions booléennes

`utils.h` définit `FALSE = -1` et `TRUE = 0`. C’est une convention possible si tous les appels la respectent. Mais [simulation.c:17–24](https://github.com/Natah11037/projects-42/blob/7a0c17cd3f3f5290289ab984fe4725d9d818b8ed/Codexion/simulation.c#L17) retourne le résultat natif d’une comparaison, donc 0 ou 1.

La condition de la ligne 88, `monitor_is_running(...) == FALSE`, compare donc 0 ou 1 à -1 : **elle ne peut jamais être vraie**. D’autres vérifications arrêtent ensuite le traitement, mais cette branche de sortie ne remplit pas son rôle. Les helpers de `simulation_externals_utils.c` doivent être examinés avec la même convention.

**À faire :** choisir une convention cohérente et vérifier chaque appel ; ne pas simplement remplacer les constantes sans adapter les tests.

### 3. Coordonner l’arrêt avec les logs

Risque identifié à la lecture, **non reproduit sur les 100 répétitions de stress** : le contrôle de `stop` est séparé de la prise du mutex d’affichage. Dans [compilation.c:66–76](https://github.com/Natah11037/projects-42/blob/7a0c17cd3f3f5290289ab984fe4725d9d818b8ed/Codexion/compilation.c#L66), un codeur peut être suspendu après le contrôle, laisser le moniteur annoncer le burnout, puis reprendre et afficher ses messages. Debug/refactor présentent une séparation similaire.

**À faire :** centraliser l’affichage normal et la décision d’autoriser le message, en coordonnant le verrou de logs et l’état d’arrêt avec un ordre de verrouillage unique. Le burnout doit pouvoir être affiché malgré l’arrêt, puis interdire les messages normaux suivants. Le mutex de logs seul évite les lignes mélangées, pas ce problème logique.

### 4. Rendre les pauses interruptibles

Compilation/debug/refactor utilisent un `usleep` pour toute la phase. Le moniteur peut annoncer rapidement le burnout, mais la fin du processus attend le réveil des codeurs avant leurs `join`. Sur le scénario burnout à 500 ms, le processus termine vers 603 ms.

**Amélioration :** attendre jusqu’à une échéance tout en permettant un réveil à l’arrêt. Distinguer le délai d’annonce du burnout et le délai réel de terminaison ; ce dernier ne doit pas être confondu avec une annonce hors tolérance.

## Live coding LIFO : essai sur une copie

Modification isolée de `heap_add_back()` : lorsqu’une file contient déjà une demande, une nouvelle demande FIFO passe devant l’ancienne ; EDF conserve la comparaison des deadlines. Aucune manipulation artificielle des logs.

| Arguments | Résultat LIFO, une exécution par ligne |
|---|---|
| `5 3000 200 200 200 10 800 fifo` | Burnout codeur 1 à 3000 ms, 6 débuts de compilation |
| `5 3000 200 200 200 10 400 fifo` | Burnout codeur 1 à 3000 ms, 10 débuts de compilation ; l’original FIFO atteint les quotas sur ses 3 essais |
| `5 10000 60 60 60 3 60 fifo` | 15 compilations, aucun burnout |
| `5 10000 200 60 60 3 0 fifo` | Burnout codeur 1 à 10000 ms, seulement 8 débuts de compilation |
| `5 2000 200 200 200 7 0 edf` | 35 compilations, aucun burnout |

**Le changement de comparaison seul n’est pas une solution LIFO robuste.** Le code garde la demande du codeur en cours de compilation dans la heap et appelle `heap_pop()` seulement à la libération ([compilation.c:80–86](https://github.com/Natah11037/projects-42/blob/7a0c17cd3f3f5290289ab984fe4725d9d818b8ed/Codexion/compilation.c#L80)). Si une nouvelle demande passe devant pendant la compilation, ce pop supprime la demande du voisin et laisse celle du codeur qui vient de finir. Ce mécanisme est déduit du code ; les journaux ne tracent pas directement le contenu des files.

**À faire avant de valider le recode :** retirer la demande lors de l’acquisition réussie des deux ressources, ou retirer explicitement la demande du bon codeur à la libération. Ajouter aussi une protection contre une insertion dans une heap pleine. Refaire ensuite les tests de fonctionnement, de famine et de concurrence. Les outils mémoire/concurrence du tableau précédent concernent l’original, pas cette copie LIFO.

Un burnout sous LIFO ne suffit donc pas à déclarer le live coding réussi : il faut distinguer famine liée à la politique et mauvaise suppression dans la file. La grille demande aussi un comportement correct avec des paramètres faisables. La capacité de l’auteur à effectuer et expliquer le recode reste non évaluée.

## Ce qui est déjà solide et ce qui peut être simplifié

- Réservation des deux ressources protégée ; contrôle de priorité et de cooldown présent.
- Attente sur condition dans une boucle : le réveil est suivi d’une nouvelle vérification, comme nécessaire.
- Moniteur et mise à jour des deadlines emploient un mutex commun ; le début de compilation refuse une deadline déjà expirée.
- Nettoyage fondé sur le nombre de threads réellement créés et les objets effectivement initialisés : utile et confirmé par les injections.
- Extraire les opérations de réservation/libération et centraliser les logs réduirait les répétitions et aiderait à respecter les 25 lignes.
- Documenter les conventions de retour et l’ordre des verrous rendrait les fonctions plus faciles à maintenir. Ne pas multiplier les mutex pour corriger un problème qui vient seulement d’une décision prise hors de la section protégée.

## Preuves et reproduction

[Script des tests principaux](audit_natah_2026-10-21/run_tests.py) ; [wrapper d’injection](audit_natah_2026-10-21/fault_wrapper.c.txt).

Résultats : [références](audit_natah_2026-10-21/evidence/benchmarks.json), [parsing](audit_natah_2026-10-21/evidence/parsing.json), [charges](audit_natah_2026-10-21/evidence/loads.json), [stress](audit_natah_2026-10-21/evidence/stress.json), [Valgrind](audit_natah_2026-10-21/evidence/tools.json), [pannes](audit_natah_2026-10-21/evidence/faults.json), [LIFO](audit_natah_2026-10-21/evidence/lifo.json), [crash sous Memcheck](audit_natah_2026-10-21/evidence/zero_coders_memcheck.log), [norme](audit_natah_2026-10-21/evidence/norminette.log), [make](audit_natah_2026-10-21/evidence/make.log).

```sh
python3 run_tests.py /chemin/projects-42/Codexion /tmp/resultats-codexion
```

Le script principal reproduit compilation diagnostique, références, outils, stress, parsing et charges. Les injections et le recode sont des expériences séparées décrites ci-dessus. Les fichiers de résultats distinguent les commandes et leurs sorties ; aucun test non exécuté n’est présenté comme réussi.
