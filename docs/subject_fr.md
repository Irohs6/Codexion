# Codexion

> Maîtrisez la course aux ressources avant que l’échéance ne vous impose sa loi.

**Résumé :** Lancez-vous dans une course contre la montre avec ce défi de programmation concurrente ! Coordonnez plusieurs codeurs qui se disputent un nombre limité de dongles USB à l’aide de threads POSIX, de mutex et d’un ordonnancement intelligent. Maîtrisez la synchronisation des ressources avant que le burnout ne frappe.

**Version : 1.5**

*Traduction française du [sujet fourni en anglais](subject.md). Les identifiants, commandes, noms de fonctions et messages de logs imposés sont conservés en anglais. Cette traduction n’ajoute pas de règles au sujet original.*

## I. Introduction

La programmation informatique, souvent simplement appelée « codage », consiste à concevoir et à construire des logiciels exécutables pour accomplir des tâches informatiques précises. Elle comprend l’analyse, la conception d’algorithmes, l’évaluation de leur exactitude et de leur consommation de ressources, ainsi que leur implémentation dans un langage de programmation choisi. Les premiers ordinateurs étaient programmés directement en langage machine : des suites d’instructions binaires comprises par le matériel. Au fil du temps, des langages de plus haut niveau ont été développés, permettant aux codeurs d’exprimer leurs idées de manière plus abstraite.

La culture du code dépasse les aspects purement techniques. Elle associe résolution de problèmes, créativité et collaboration dans des environnements partagés, des communautés open source aux espaces de coworking et aux hackathons. Dans ces lieux, les codeurs ne font pas qu’écrire du code : ils déboguent, refactorisent et partagent des outils. Certaines ressources y sont abondantes, comme le café, tandis que d’autres, comme les dongles matériels spécialisés ou les environnements de développement sous licence, sont limitées et doivent être partagées avec soin pour éviter les goulots d’étranglement.

Tout comme les philosophes de l’Antiquité débattaient de la vérité et de l’existence, les codeurs modernes débattent des paradigmes de programmation, des patrons de conception et des pratiques éthiques du logiciel. Vaut-il mieux optimiser la vitesse ou la maintenabilité ? Faut-il refactoriser dès maintenant ou attendre que le produit fasse ses preuves ? Comment une équipe peut-elle garantir un accès équitable aux ressources de développement partagées sans bloquer sa progression ?

Historiquement, la programmation était souvent une activité solitaire. Mais l’évolution des outils collaboratifs — systèmes de gestion de versions, éditeurs en temps réel et outils distribués de suivi des problèmes — en a fait une activité de plus en plus sociale et coopérative. Parallèlement, la concurrence et la synchronisation sont devenues des compétences essentielles : savoir permettre à plusieurs développeurs ou processus de travailler sans se gêner est aussi important que l’écriture du code elle-même.

Aujourd’hui, avec des équipes réparties sur plusieurs fuseaux horaires et des projets utilisant du matériel rare ou accessible pour une durée limitée, le défi ne consiste pas seulement à écrire du code correct, mais aussi à concevoir des protocoles de partage des ressources équitables et efficaces. Cette simulation représente une telle situation : des codeurs travaillent dans un espace partagé et ont besoin de deux dongles USB rares pour compiler leur code quantique. Ils doivent se coordonner, éviter les interblocages et prévenir le burnout, car dans le monde du développement collaboratif, comme dans la vie, l’accès aux ressources et le bon timing sont essentiels.

## II. Consignes relatives à l’IA

### Contexte

Pendant votre apprentissage, l’IA peut vous aider dans de nombreuses tâches. Prenez le temps d’explorer les capacités des outils d’IA et la manière dont ils peuvent soutenir votre travail. Toutefois, utilisez-les toujours avec prudence et évaluez leurs résultats avec un regard critique. Qu’il s’agisse de code, de documentation, d’idées ou d’explications techniques, vous ne pouvez jamais être totalement certain que votre question était bien formulée ni que le contenu généré est exact. Vos pairs sont une ressource précieuse pour éviter les erreurs et les angles morts.

### Message principal

- Utilisez l’IA pour réduire les tâches répétitives ou fastidieuses.
- Développez vos compétences de formulation de requêtes, pour le code comme pour d’autres usages : elles vous seront utiles dans votre future carrière.
- Apprenez comment fonctionnent les systèmes d’IA pour mieux anticiper et éviter les risques courants, les biais et les problèmes éthiques.
- Continuez à développer vos compétences techniques et humaines en travaillant avec vos pairs.
- N’utilisez que du contenu généré par l’IA que vous comprenez entièrement et dont vous pouvez assumer la responsabilité.

### Règles pour les apprenants

- Prenez le temps d’explorer les outils d’IA et de comprendre leur fonctionnement afin de les utiliser de manière éthique et de limiter les biais potentiels.
- Réfléchissez à votre problème avant de formuler une requête : cela vous aidera à écrire des demandes plus claires, détaillées et pertinentes, avec un vocabulaire précis.
- Prenez l’habitude de vérifier, relire, remettre en question et tester systématiquement tout contenu généré par l’IA.
- Demandez toujours une relecture par vos pairs : ne vous fiez pas uniquement à votre propre validation.

### Résultats attendus de cette phase

- Développer des compétences de formulation de requêtes générales et spécialisées.
- Améliorer votre productivité grâce à une utilisation efficace des outils d’IA.
- Continuer à renforcer votre pensée informatique, votre capacité à résoudre des problèmes, votre adaptabilité et votre aptitude à collaborer.

### Commentaires et exemples

- Vous rencontrerez régulièrement des situations — examens, évaluations et autres — où vous devrez démontrer une compréhension réelle. Préparez-vous et continuez à développer vos compétences techniques et relationnelles.
- Expliquer votre raisonnement et en discuter avec vos pairs révèle souvent des lacunes dans votre compréhension. Faites de l’apprentissage entre pairs une priorité.
- Les outils d’IA manquent souvent de votre contexte précis et ont tendance à fournir des réponses génériques. Vos pairs, qui partagent votre environnement, peuvent apporter des éclairages plus pertinents et plus précis.
- Là où l’IA tend à générer la réponse la plus probable, vos pairs peuvent proposer d’autres points de vue et des nuances utiles. Appuyez-vous sur eux pour vérifier la qualité de votre travail.

**✓ Bonne pratique :**

Je demande à l’IA : « Comment tester une fonction de tri ? » Elle me donne quelques idées. Je les essaie et j’examine les résultats avec un pair. Nous affinons ensemble notre approche.

**✗ Mauvaise pratique :**

Je demande à l’IA d’écrire une fonction entière et je la copie-colle dans mon projet. Pendant l’évaluation par les pairs, je ne peux pas expliquer ce qu’elle fait ni pourquoi. Je perds en crédibilité et je ne valide pas mon projet.

**✓ Bonne pratique :**

J’utilise l’IA pour m’aider à concevoir un parseur. Puis j’en examine la logique avec un pair. Nous trouvons deux bugs et le réécrivons ensemble : il est meilleur, plus propre et entièrement compris.

**✗ Mauvaise pratique :**

Je laisse Copilot générer le code d’une partie essentielle de mon projet. Il compile, mais je ne peux pas expliquer comment il gère les pipes. Pendant l’évaluation, je ne parviens pas à justifier mon code et je ne valide pas mon projet.

## III. Consignes communes

- Votre projet doit être écrit en C.
- Votre projet doit respecter la Norme. Si vous avez des fichiers ou des fonctions bonus, ils sont également soumis à la vérification de la norme. Une erreur de norme entraîne la note de 0.
- Vos fonctions ne doivent pas s’arrêter de manière inattendue : erreur de segmentation, erreur de bus, double libération, etc., sauf en cas de comportement indéfini. Si cela se produit, votre projet sera considéré comme non fonctionnel et recevra la note de 0 lors de l’évaluation.
- Toute mémoire allouée sur le tas doit être correctement libérée lorsque cela est nécessaire. Aucune fuite mémoire ne sera tolérée.
- Si le sujet l’exige, vous devez rendre un Makefile qui compile vos fichiers sources pour produire la sortie demandée, avec `cc` et les options `-Wall`, `-Wextra` et `-Werror`. Votre Makefile ne doit pas effectuer d’édition de liens inutile.
- Votre Makefile doit contenir au minimum les règles `$(NAME)`, `all`, `clean`, `fclean` et `re`.
- Pour rendre des bonus, vous devez inclure une règle `bonus` dans votre Makefile. Elle ajoutera les différents headers, bibliothèques ou fonctions qui ne sont pas autorisés dans la partie principale du projet. Les bonus doivent se trouver dans des fichiers `_bonus.{c/h}`, sauf indication contraire du sujet. Les parties obligatoire et bonus sont évaluées séparément.
- Si votre projet autorise l’utilisation de votre libft, vous devez copier ses sources et son Makefile dans un dossier `libft`. Le Makefile du projet doit compiler cette bibliothèque à l’aide de son Makefile, puis compiler le projet.
- Nous vous encourageons à créer des programmes de test, même s’ils ne doivent pas être rendus et ne seront pas notés. Ils vous permettront de tester facilement votre travail et celui de vos pairs. Ces tests seront particulièrement utiles pendant la soutenance : vous pourrez utiliser vos propres tests et/ou ceux du pair que vous évaluez.
- Rendez votre travail dans le dépôt Git attribué. Seul le travail présent dans ce dépôt sera noté. Si Deepthought est chargé d’évaluer votre travail, il interviendra après les évaluations par les pairs. Si une erreur survient dans une partie de votre travail pendant son évaluation, celle-ci s’arrêtera.

## IV. Vue d’ensemble

Voici ce que vous devez savoir pour réussir ce projet :

- Un ou plusieurs codeurs sont assis dans un espace de coworking inclusif organisé en cercle. Au centre se trouve un compilateur quantique partagé.
- Les codeurs alternent entre compilation, débogage et refactorisation. Pendant qu’ils compilent, ils ne déboguent ni ne refactorisent ; pendant qu’ils déboguent, ils ne compilent ni ne refactorisent ; et pendant qu’ils refactorisent, ils ne compilent ni ne déboguent.
- Des dongles USB sont posés sur la table. Il y a autant de dongles que de codeurs.
- Compiler du code quantique nécessite deux dongles branchés simultanément, un dans chaque main : un codeur prend les dongles situés à sa gauche et à sa droite pour compiler.
- Lorsqu’un codeur termine sa compilation, il repose les deux dongles sur la table et commence à déboguer. Une fois le débogage terminé, il commence à refactoriser. La simulation s’arrête lorsqu’un codeur subit un burnout faute d’avoir compilé.
- Chaque codeur doit compiler régulièrement et ne devrait jamais subir de burnout.
- Les codeurs ne communiquent pas entre eux.
- Les codeurs ne savent pas si un autre codeur est sur le point de subir un burnout.
- Inutile de le préciser : les codeurs doivent éviter le burnout !

## V. Règles générales

Vous devez écrire un programme respectant les règles suivantes.

**Les variables globales sont interdites !**

Votre programme doit recevoir les arguments suivants, tous obligatoires, dans cet ordre :

```text
number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- **`number_of_coders`** : nombre de codeurs et, également, nombre de dongles.
- **`time_to_burnout`**, en millisecondes : si un codeur n’a pas commencé à compiler dans les `time_to_burnout` millisecondes suivant le début de sa dernière compilation ou le début de la simulation, il subit un burnout.
- **`time_to_compile`**, en millisecondes : durée nécessaire à un codeur pour compiler. Pendant toute cette durée, il doit détenir deux dongles.
- **`time_to_debug`**, en millisecondes : durée passée par un codeur à déboguer.
- **`time_to_refactor`**, en millisecondes : durée passée par un codeur à refactoriser. Une fois cette phase terminée, il tente immédiatement d’acquérir des dongles et de recommencer à compiler.
- **`number_of_compiles_required`** : si tous les codeurs ont compilé au moins ce nombre de fois, la simulation s’arrête. Sinon, elle s’arrête lorsqu’un codeur subit un burnout.
- **`dongle_cooldown`**, en millisecondes : après sa libération, un dongle reste indisponible jusqu’à l’expiration de son délai de récupération.
- **`scheduler`** : politique d’arbitrage utilisée par les dongles pour décider à qui les attribuer lorsque plusieurs codeurs les demandent. La valeur doit être exactement `fifo` ou `edf`.
  - `fifo` signifie *First In, First Out*, « premier arrivé, premier servi » : le dongle est accordé au codeur dont la demande est arrivée en premier.
  - `edf` signifie *Earliest Deadline First*, « échéance la plus proche en premier », avec `deadline = last_compile_start + time_to_burnout`.

Chaque codeur porte un numéro compris entre `1` et `number_of_coders`.

Le codeur numéro `1` est assis à côté du codeur numéro `number_of_coders`. Tout autre codeur numéro `N` se trouve entre les codeurs numéro `N - 1` et `N + 1`.

**Rappel :** tous les arguments sont obligatoires. Rejetez les entrées invalides, comme les nombres négatifs, les valeurs non entières ou un scheduler différent de `fifo` et `edf`.

### Logs du programme

Tout changement d’état d’un codeur doit respecter l’un des formats suivants :

```text
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

Remplacez `timestamp_in_ms` par l’horodatage courant en millisecondes et `X` par le numéro du codeur.

- Un message d’état affiché ne doit pas se mélanger avec un autre message.
- Le message annonçant le burnout d’un codeur doit être affiché au plus tard **10 ms après le burnout réel**.
- Encore une fois, les codeurs doivent éviter le burnout !

Exemple de format de logs attendu :

```text
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
402 2 has taken a dongle
403 2 has taken a dongle
403 2 is compiling
603 2 is debugging
803 2 is refactoring
1204 3 burned out
```

**Exigence de précision :** les logs de burnout doivent être affichés dans les 10 ms suivant l’instant réel du burnout. Prévoyez une tolérance minimale lors des tests, car le matériel et l’ordonnancement du système d’exploitation peuvent légèrement influer sur les mesures.

**Considération temporelle :** pour réduire l’influence du matériel sur les mesures de performance, envisagez d’utiliser le temps d’utilisation du processeur plutôt que l’horloge de temps réel lorsque cela est possible. Toutefois, pour ce projet, les mesures de temps réel avec `gettimeofday()` sont acceptables et recommandées pour leur simplicité.

## VI. Partie obligatoire

| Élément | Exigence |
| --- | --- |
| Nom du programme | `codexion` |
| Fichiers à rendre | Makefile à la racine, fichiers `*.c` et `*.h` dans le dossier de votre choix |
| Makefile | `NAME`, `all`, `clean`, `fclean`, `re` |
| Arguments | `number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler` |
| Fonctions externes | `pthread_create`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_mutex_destroy`, `pthread_cond_init`, `pthread_cond_wait`, `pthread_cond_timedwait`, `pthread_cond_signal`, `pthread_cond_broadcast`, `pthread_cond_destroy`, `gettimeofday`, `clock_gettime`, `usleep`, `write`, `malloc`, `free`, `printf`, `fprintf`, `strcmp`, `strlen`, `atoi`, `memset` |
| Libft autorisée | Non |
| Description | Des codeurs avec des threads et des mutex, en C |

Les règles propres à la partie obligatoire sont les suivantes :

- Chaque codeur doit être représenté par un thread, créé avec `pthread_create`.
- Il y a un dongle entre chaque paire de codeurs voisins. S’il y a plusieurs codeurs, chacun dispose donc d’un dongle à sa gauche et d’un autre à sa droite. S’il n’y a qu’un seul codeur, il ne doit y avoir qu’un seul dongle sur la table.
- Pour empêcher les codeurs de dupliquer les dongles, vous devez protéger l’état de chaque dongle avec un mutex, de type `pthread_mutex_t`. Une variable de condition, de type `pthread_cond_t`, peut être utilisée pour gérer les files d’attente.
- **Le délai de récupération des dongles est obligatoire** : après qu’un codeur a libéré un dongle, celui-ci ne peut pas être repris avant que `dongle_cooldown` millisecondes se soient écoulées.
- **L’arbitrage équitable est obligatoire** : lorsque plusieurs codeurs demandent le même dongle, celui-ci doit accorder l’accès selon `scheduler`. Avec `fifo`, servez les demandes dans leur ordre d’arrivée. Avec `edf`, servez le codeur dont l’échéance de burnout est la plus proche, c’est-à-dire `last_compile_start + time_to_burnout`.

**Remarque :** en raison de la précision des horodatages, il peut arriver, rarement en pratique, que des échéances soient égales. Une règle de départage est obligatoire pour garantir une politique EDF entièrement déterministe, même dans les cas limites.

- Le programme doit garantir la progression (*liveness*) : aucun codeur ne doit être privé de dongles au point de subir un burnout sous l’ordonnancement `edf`, à condition que les paramètres permettent une exécution viable.
- Un thread de surveillance séparé doit détecter précisément le burnout et arrêter la simulation. Le log de burnout doit être affiché dans les 10 ms suivant l’instant réel du burnout.
- L’écriture des logs doit être sérialisée afin que deux messages ne s’entremêlent jamais sur une même ligne. Utilisez un mutex pour protéger la sortie.
- La simulation s’arrête soit lorsqu’un codeur subit un burnout, soit lorsque chaque codeur a compilé au moins `number_of_compiles_required` fois.
- Votre code doit compiler avec `-Wall -Wextra -Werror -pthread`.
- Vous devez implémenter une file de priorité reposant sur un tas (*heap*) pour l’ordonnancement FIFO/EDF. Aucune file de priorité de bibliothèque standard ne peut être utilisée.
- Toute mémoire doit être correctement allouée et libérée : aucune fuite mémoire.

Exemple d’exécution de la simulation :

```text
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
405 2 has taken a dongle
406 2 has taken a dongle
406 2 is compiling
606 2 is debugging
806 2 is refactoring
900 3 has taken a dongle
902 3 has taken a dongle
902 3 is compiling
1102 3 is debugging
1302 3 is refactoring
1505 4 burned out
```

Cet exemple illustre l’enchaînement des actions de plusieurs codeurs. Remarquez que chaque action `is compiling` est précédée de deux lignes `has taken a dongle`, et que le message `burned out` apparaît au moment où un codeur manque son échéance.

## VII. Exigences du README

Un fichier `README.md` doit être présent à la racine de votre dépôt Git. Il doit permettre à une personne qui ne connaît pas le projet — pairs, équipe pédagogique, recruteurs, etc. — de comprendre rapidement son objectif, la manière de l’exécuter et où trouver davantage d’informations sur le sujet.

Le `README.md` doit contenir au minimum :

- Une toute première ligne en italique, reprenant le texte suivant :

  ```text
  This project has been created as part of the 42 curriculum by <login1>[, <login2>[, <login3>[...]]].
  ```

- Une section **« Description »** présentant clairement le projet, son objectif et un bref aperçu.
- Une section **« Instructions »** contenant les informations pertinentes sur la compilation, l’installation et/ou l’exécution.
- Une section **« Resources »** listant des références classiques sur le sujet — documentation, articles, tutoriels, etc. — ainsi qu’une description de l’utilisation de l’IA, en précisant pour quelles tâches et dans quelles parties du projet elle a été utilisée.

Des sections supplémentaires peuvent être exigées selon le projet : exemples d’utilisation, liste des fonctionnalités, choix techniques, etc. Les ajouts obligatoires sont explicitement indiqués ci-dessous.

Pour ce projet, le `README.md` doit également contenir :

- Une section **« Blocking cases handled »** décrivant tous les problèmes de concurrence traités par votre solution : par exemple la prévention des interblocages et les conditions de Coffman, la prévention de la famine, la gestion du délai de récupération, la détection précise du burnout et la sérialisation des logs.
- Une section **« Thread synchronization mechanisms »** expliquant les mécanismes de synchronisation utilisés dans votre implémentation — `pthread_mutex_t`, `pthread_cond_t`, implémentation personnalisée d’événements — et la manière dont ils coordonnent l’accès aux ressources partagées : dongles, logs et état du moniteur. Incluez des exemples montrant comment les conditions de concurrence sont évitées et comment les codeurs et le moniteur communiquent de manière sûre entre threads.

**Votre README doit être rédigé en anglais.**

## VIII. Rendu et évaluation par les pairs

Rendez votre travail dans votre dépôt Git comme d’habitude. Seul le contenu du dépôt sera évalué pendant la soutenance. N’hésitez pas à vérifier les noms de vos fichiers pour vous assurer qu’ils sont corrects.

### Consignes de modification du code

Pendant l’évaluation, une courte modification du projet peut parfois vous être demandée. Il peut s’agir d’un changement mineur de comportement, de quelques lignes de code à écrire ou à réécrire, ou d’une fonctionnalité simple à ajouter.

Cette étape ne s’applique pas nécessairement à tous les projets, mais vous devez vous y préparer si elle est mentionnée dans les consignes d’évaluation.

Elle vise à vérifier votre compréhension réelle d’une partie précise du projet.

La modification peut être réalisée dans l’environnement de développement de votre choix, par exemple votre environnement habituel, et doit pouvoir être effectuée en quelques minutes, sauf si une durée précise est fixée dans le cadre de l’évaluation.

On peut, par exemple, vous demander une petite modification d’une fonction ou d’un script, un changement d’affichage ou une adaptation d’une structure de données pour y stocker de nouvelles informations.

Les détails — périmètre, cible, etc. — seront indiqués dans les consignes d’évaluation et pourront varier d’une évaluation à l’autre pour un même projet.
