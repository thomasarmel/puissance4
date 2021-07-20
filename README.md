# puissance4
### Un jeu de puissance 4 amélioré écrit en C

Un jeu de puissance 4 en mode console écrit en C.

En plus des coups traditionnels, il est possible de pivoter le plateau à gauche, à droite et verticalement.

Le programme est doté d'un bot de type "min-max". Il existe 3 modes de jeu possible:
* Humain vs humain
* Bot vs humain
* Bot vs bot

Il est possible de sauvegarder l'état du jeu à tout moment quand c'est à votre tour de jouer.

Pour régler la profondeur (et donc "l'intelligence") du bot min-max, ajouter l'option **-DPROFONDEUR_MIN_MAX=[profondeur]** au compilateur (défaut: 4).

Pour changer la taille du plateau, ajouter l'option **-DTAILLE_JEU=[taille]** au compilateur (défaut: 7)

**ATTENTION:** Si vous compiler avec gcc, pensez à ajouter l'option **-lm** au compilateur, puisque le programme utilise math.h.

De plus, à partir d'une certaine profondeur de bot min-max, il n'est plus possible de compiler pour une architecture 32 bits. En effet le bot utilise des nombres entiers pour évaluer ses actions possibles. Ces nombres peuvent atteindre une valeur assez conséquente si la profondeur est élevée et par conséquent ne plus être stockables sur 32 bits.
