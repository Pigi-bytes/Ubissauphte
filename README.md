# Ubissauphte
Ubissauphte

Bienvenue sur le projet Lo Poti Donjon crée par Ubissauphte, un projet de fin d'année 2024-2025 pour la L2 informatique.

Contributeurs:
- Souleymane Barry (souley-b)
- Barthelemy Derouault (Pigi-bytes)
- Victor Fombertasse (Michele530)
- Baptiste Carfantan (Baptcarf)

Principe du jeu:
Notre jeu est un jeu de type rogue-like où le joueur devra se préparer pour résister le plus longtemps dans le donjon rempli de dangers et de monstres

Histoire du jeu :
Lo Poti Donjon est un jeu centré sur un aventurier égaré dans un vaste donjon, celui de Pablo, privé de toutes ses capacités. Pour récupérer ce qui lui revient de droit, il devra parcourir un long périple dans un donjon rempli de pièges et de monstres déterminés à protéger leur domaine.

Dans ce donjon réside un intagoniste puissant et impitoyable, il incarne l'ultime défi du donjon. Le périple s'achève une fois qu'il est vaincu.

Pour surmonter ces épreuves, notre aventurier devra allier force et stratégie afin d'augmenter sa puissance et mettre fin au règne de Pablo. Il débute avec une simple dague. Pour progresser, il peut ouvrir des coffres contenant divers objets ou accumuler des pièces pour acheter des armes de différentes raretés auprès des marchands, qu'il stockera dans son inventaire. Avec un peu de chance, il pourra même dénicher la Double Hache de Guerrier Légendaire.


Fonctionnalités du jeu:
Lorsqu'on lance le jeu, le programme génère un niveau et les salles de celui-ci en fonction du niveau de difficulté choisi par le joueur : 
    - easy : 7 salles avec 5 monstres par salles
    - normal : 17 salles avec 10 monstres pas salles
    - hard : 22 salles avec 20 enemis par salles
    - demonic : 32 salles avec 30 enemis par salles
    - legendaire : 102 salles avec 50 enemis par salle

Puis, quand le joueur clique sur le bouton jouer, le programme va générer les salles demandées, ce qui crée un petit temps d'attente si le joueur décide de jouer en mode légendaire.
Nous avons préféré que le joueur attende en début de partie plutôt que d'avoir de la latence quand il change de map car il faut la créer.

Une fois les cartes générées, il est placé aléatoirement sur la première. Le but du joueur : tuer les monstres des différentes salles pour pouvoir passer à la prochaine, 
Trouver des équipements grâce aux coffres et au marchand  est donc essentiel  pour aller se battre contre le boss du jeu et le tuer.

Le jeu possède un système de niveau, puisque le jeu est assez punitif, si le joueur monte d'un niveau il regagne toute ça vie



lien vers le Gantt Prévus: https://docs.google.com/spreadsheets/d/1izmjEU3AdizAlb6oVq4sjDTDMpQiwE0Ea9UwfN2fawg/edit?usp=sharing
lien vers le Gantt Réel : https://docs.google.com/spreadsheets/d/1kdkzD_GTPsmrwCW2jH0_2ET8rOUeqwklMZoLPcJabdQ/edit?usp=sharing

lien vers la documentation Doxygen : https://pigi-bytes.github.io/Ubissauphte/index.html


Fonts :
- https://www.jetbrains.com/lp/mono/
- https://www.zone38.net/font/

Imgs : 
- https://kenney-assets.itch.io/tiny-dungeon

Installation du jeu:
Avant de compiler le projet, assurez-vous que les dépendances suivantes sont installées sur votre système :
- SDL2 : Bibliothèque principale.
- SDL2_mixer : Pour la gestion des sons.
- SDL2_ttf : Pour l'affichage des polices.

Sur un système Linux (par exemple Ubuntu), vous pouvez installer ces dépendances avec la commande suivante :
- sudo apt-get install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev

Une fois les dépendances installées, suivez les étapes ci-dessous pour compiler et exécuter le projet :

- Clonez le dépôt GitHub :
    - git clone https://github.com/Pigi-bytes/Ubissauphte.git
- Accédez au répertoire du projet :
    - cd Ubissauphte
- Compilez le projet avec make :
    - make
- Lancez le jeu :
    - ./lo-poti-donjon


