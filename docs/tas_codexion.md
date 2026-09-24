# Le tas dans Codexion — tableau, ajout et retrait

[Retour aux étapes d’implémentation](etapes_implementation.md)

Ce guide décrit le tas à construire. Le code actuel de `heap.c` utilise encore une liste chaînée. Les lettres A à F représentent des coders ; les nombres sont des deadlines fictives en millisecondes, sur une même horloge.

## 1. Deux tableaux qui ne contiennent pas la même chose

Le tableau du memory manager contient les **structures des coders**. Le tableau du tas peut contenir des **pointeurs vers ces coders**.

```text
MEMORY MANAGER : les coders restent à leur place

array_coder      [ coder A ][ coder B ][ coder C ][ coder D ] …
                     ▲          ▲          ▲
                     │          │          │
TABLEAU DU TAS       │          │          │
                  [ &A ]     [ &B ]     [ &C ]     [ libre ] …
                  indice 0   indice 1   indice 2
```

`&A` signifie ici « adresse du coder A », pas une seconde allocation du coder A. Changer l’ordre des pointeurs du tas ne déplace pas les structures d’origine, leurs champs `pthread_t` ou leurs liens vers les dongles.

Une structure peut parfaitement décrire le tas. Pour apprendre les opérations, une représentation simple serait :

```c
typedef struct s_heap
{
    t_coder **array_coder;
    int     size;
    int     capacity;
}   t_heap;
```

Ici, `heap->array_coder` désigne un tableau de pointeurs (`t_coder **`). `manager->array_coder` désigne le tableau des structures (`t_coder *`). Le même nom ne signifie donc pas le même contenu.

Cette version illustre le rangement. Pour FIFO et les égalités EDF, il faudra aussi conserver les informations de la demande, expliquées à la section 7.

## 2. Capacité et nombre d’éléments présents

```text
indices       0       1       2       3       4       5       6       7
           ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
tas        │  &A   │  &B   │  &C   │  &D   │  &E   │ libre │ libre │ libre │
           └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
           └────────── 5 éléments présents ────────┘
           └──────────────── 8 cases allouées ────────────────────────────┘

size = 5                         capacity = 8
```

Seules les cases d’indices `0` à `size - 1` appartiennent au tas actif. Pour ajouter un élément, on utilise la case d’indice `size`, à condition que `size < capacity`.

Le tableau est alloué une fois. Chaque ajout ou retrait modifie son contenu et `size`, sans appeler `malloc` ou `free`. Si le tableau est plein, il faut signaler cet état ou avoir prévu un agrandissement.

Une capacité égale au nombre de coders suffit pour une file **si chaque coder ne peut y avoir qu’une demande en attente**. Il faut garantir cette règle et empêcher les doublons.

## 3. Le même tableau représente un arbre

Avec EDF, une petite deadline donne une priorité plus élevée. Dans les schémas, `A:10` représente le pointeur vers A et sa deadline 10.

```text
TABLEAU :   [ A:10 ][ B:30 ][ C:20 ][ D:50 ][ E:40 ]
indice :       0      1      2      3      4

ARBRE :                     A:10 (0)
                           /        \
                     B:30 (1)      C:20 (2)
                     /      \
                D:50 (3)   E:40 (4)
```

La règle est simple : **un parent doit passer avant ses enfants, ou avoir la même priorité**. La racine, en case 0, est donc un élément de priorité maximale, ici la deadline minimale.

Le tableau n’est pas entièrement trié : B:30 peut se trouver avant C:20. Ils sont frères, et aucune règle n’impose leur ordre entre eux.

Pour une case d’indice `i` :

| Relation | Indice | Condition |
| --- | --- | --- |
| Parent | `(i - 1) / 2` | Seulement si `i > 0` |
| Enfant gauche | `2 * i + 1` | Existe si son indice est inférieur à `size` |
| Enfant droit | `2 * i + 2` | Existe si son indice est inférieur à `size` |

Les divisions sont des divisions entières. Aucun pointeur `next`, `left` ou `right` n’est nécessaire : les indices donnent les relations.

## 4. Déplacer un élément : échanger deux pointeurs

Une affectation copie une valeur dans une case. Pour échanger deux cases sans perdre leur contenu, on garde temporairement l’un des pointeurs :

```c
t_coder *temporary;

temporary = heap->array_coder[i];
heap->array_coder[i] = heap->array_coder[j];
heap->array_coder[j] = temporary;
```

```text
Avant                 Sauvegarde              Après
case i : &A           temporary : &A           case i : &C
case j : &C                                   case j : &A
```

On copie seulement des adresses. On n’échange pas les structures complètes des coders. Aucun coder n’est créé, détruit ou déplacé en mémoire.

## 5. Ajouter un coder : le placer à la fin, puis le remonter

Ajout de F, dont la deadline est 5. Le tas dispose d’une case libre.

```text
1. État initial — size = 5
   [ A:10 ][ B:30 ][ C:20 ][ D:50 ][ E:40 ][ libre ]

2. Placer &F à l’indice size, puis augmenter size — size = 6
   [ A:10 ][ B:30 ][ C:20 ][ D:50 ][ E:40 ][ F:5  ]
                      ▲                             ▲
                    parent 2                     enfant 5

3. F doit passer avant C : échanger les cases 5 et 2
   [ A:10 ][ B:30 ][ F:5  ][ D:50 ][ E:40 ][ C:20 ]
       ▲              ▲
    parent 0       enfant 2

4. F doit aussi passer avant A : échanger les cases 2 et 0
   [ F:5  ][ B:30 ][ A:10 ][ D:50 ][ E:40 ][ C:20 ]
```

F est arrivé à la racine : la remontée est terminée. Sinon, elle s’arrête dès que son parent a déjà une priorité au moins aussi élevée.

```text
                             F:5
                           /     \
                        B:30     A:10
                        /  \     /
                     D:50 E:40 C:20
```

Cette opération s’appelle souvent `sift_up`. Elle suit seulement les parents de l’élément ajouté.

## 6. Retirer le premier : remplacer la racine, puis descendre

On souhaite extraire F du tas précédent.

```text
1. Sauvegarder le pointeur à renvoyer : selected = &F
   [ F:5  ][ B:30 ][ A:10 ][ D:50 ][ E:40 ][ C:20 ]
      ▲                                               ▲
    racine                                      dernier élément

2. Mettre le dernier élément à la racine et diminuer size
   [ C:20 ][ B:30 ][ A:10 ][ D:50 ][ E:40 ] | ancienne case
   └────────────── size = 5 ──────────────┘ | hors du tas actif

3. Comparer les deux enfants : A:10 passe avant B:30
   Comparer C:20 à A:10 : échanger les cases 0 et 2
   [ A:10 ][ B:30 ][ C:20 ][ D:50 ][ E:40 ]

4. C n’a plus d’enfant : terminer et renvoyer selected (&F)
```

L’ancienne dernière case peut être remise à `NULL` pour rendre l’état plus lisible. Cela ne libère rien : c’est `size` qui délimite les cases actives.

La descente, souvent appelée `sift_down`, choisit à chaque étape **l’enfant le plus prioritaire**, puis échange avec lui si nécessaire. Choisir systématiquement l’enfant gauche pourrait laisser une priorité plus élevée sous la racine.

Cas particuliers :

- Tas vide : rien à extraire ; renvoyer `NULL` ou le statut prévu par l’interface.
- Un seul élément : sauvegarder son pointeur, passer `size` à zéro, puis le renvoyer. Pas de descente.
- Un seul enfant pendant la descente : comparer uniquement cet enfant existant.

**Extraire F ne libère pas F.** Cela retire sa demande de cette file. Le coder reste dans le tableau du memory manager et son thread peut continuer. L’extraction seule ne lui attribue pas automatiquement les deux dongles.

## 7. FIFO et EDF : même tas, comparaison différente

Les échanges, la remontée et la descente restent les mêmes. La fonction qui décide quelle demande passe en premier change.

| Mode | Première comparaison | En cas d’égalité |
| --- | --- | --- |
| FIFO | Ordre d’arrivée de la demande | Utiliser un ordre d’arrivée unique dans la file |
| EDF | Deadline de burnout la plus petite | Par exemple, ordre d’arrivée, puis ID du coder si nécessaire |

Le départage EDF ci-dessus est un choix possible pour obtenir un ordre déterministe.

L’ordre d’arrivée concerne la **demande actuelle**, pas l’ID du coder. Le coder 4 peut demander un dongle avant le coder 1.

Pour aller au-delà du tableau pédagogique de pointeurs, une entrée peut représenter une demande :

```text
Une demande
┌────────────────────────────────────────────┐
│ coder          → adresse du coder concerné  │
│ arrival_order  → ordre d’arrivée dans la file│
│ deadline       → échéance de cette demande  │
└────────────────────────────────────────────┘
```

Un tableau de ces petites structures peut également être alloué une seule fois. On échange alors les entrées complètes pour conserver ensemble le pointeur et ses informations de priorité. Les coders eux-mêmes restent à leur place.

Si un coder a une demande dans chacune de deux files de dongles, conserver l’ordre d’arrivée dans chaque demande évite qu’une insertion écrase les informations de l’autre.

Une priorité utilisée par le tas doit rester cohérente pendant l’attente. Modifier directement une deadline déjà rangée peut casser l’ordre du tas. On peut conserver l’échéance dans la demande ; si elle doit réellement changer, il faut aussi rétablir l’ordre du tas.

## 8. Quand allouer et quand libérer ?

```text
DÉBUT                  PENDANT LA SIMULATION              FIN

Allouer les coders     Ils restent à la même adresse      Attendre les threads
Allouer les files      Ajouter / extraire / échanger      Libérer les files
                       Modifier size                     Libérer les coders
```

Le propriétaire de chaque allocation la libère une seule fois. Ne pas appeler `free` sur un coder extrait du tas : il s’agit d’une case du tableau des coders, pas d’une allocation individuelle.

Une fois les files utilisées par plusieurs threads, leurs opérations doivent être protégées. Cela inclut la comparaison, les échanges et la modification de `size`, pas seulement l’écriture d’une case. La règle de verrouillage sera définie lors de l’intégration avec les dongles.

## 9. Ordre conseillé pour écrire les fonctions toi-même

1. Initialiser un tas vide avec une capacité définie.
2. Écrire la comparaison entre deux demandes.
3. Écrire l’échange de deux cases.
4. Écrire la remontée, puis l’ajout.
5. Écrire la descente, puis l’extraction de la racine.
6. Libérer le tableau du tas à la fin de son utilisation.
7. Intégrer les opérations à la simulation avec la synchronisation prévue.

Pour vérifier le résultat, extraire successivement toutes les demandes : en EDF, les deadlines doivent sortir dans l’ordre croissant ; en FIFO, les demandes doivent sortir dans leur ordre d’arrivée. Vérifier aussi les égalités, le tas vide, un seul élément et la capacité atteinte.

Le tableau intermédiaire n’a pas besoin d’être entièrement trié. Après chaque opération, chaque parent doit simplement garder la priorité sur ses enfants selon la comparaison choisie.
