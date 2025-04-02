#include "genmap.h"

t_salle* initRoom() {
    t_salle* room = malloc(sizeof(t_salle));
    if (room == NULL) {
        fprintf(stderr, "ERREUR MALLOC dans initRoom()\n");
        return NULL;
    }

    room->ID = 0;
    room->droite = NULL;
    room->gauche = NULL;
    room->haut = NULL;
    room->bas = NULL;

    return room;
}

void connectRoom(t_salle* origineRoom, t_salle* destinationRoom, int dir) {
    // Liste opposées des directions pour avoir une connection
    t_salle** doorOrigin[4] = {&origineRoom->droite, &origineRoom->gauche, &origineRoom->haut, &origineRoom->bas};
    t_salle** doorDestination[4] = {&destinationRoom->gauche, &destinationRoom->droite, &destinationRoom->bas, &destinationRoom->haut};

    *doorOrigin[dir] = destinationRoom;
    *doorDestination[dir] = origineRoom;
}

t_salle** genMap(int numberRoom, SDL_Rect* roomCoords) {
    int seed = time(NULL);
    srand(seed);
    printf("Seed : %d\n", seed);

    if (numberRoom < 3) {
        printf("Besoin d'au moins 3 salles.");
        return NULL;
    }

    t_salle** roomList = malloc(sizeof(t_salle*) * numberRoom);
    roomList[0] = initRoom();
    roomList[0]->ID = 0;
    roomCoords[0] = (SDL_Rect){0, 0, 0, 0};

    // Offsets de coord par direction (Droite gauche haut bas)
    int coordsX[4] = {1, -1, 0, 0};  // Deplacement horizontaux
    int coordsY[4] = {0, 0, -1, 1};  // Deplacement verticaux

    for (int i = 1; i < numberRoom - 2; i++) {
        t_salle* newRoom = NULL;

        do {
            // Selection d'une salle aléatoire dans la liste
            int randomRoomIndex = rand() % i;
            t_salle* currentRoom = roomList[randomRoomIndex];

            // On va analyser les 4 directions.
            // Une direction est libre si le pointeur est NULL
            int directionPossible[4];
            int directionTester = 0;
            for (int dir = 0; dir < 4; dir++) {
                // Conversion implicite :
                // currentRoom->droite = ((t_salle**)currentRoom)[0]
                // currentRoom->gauche = ((t_salle**)currentRoom)[1]
                // ...
                if (!*((t_salle**)currentRoom + dir)) {  // Check pointeur NULL
                    directionPossible[directionTester++] = dir;
                }
            }

            // Si au moins une des 4 direction est libre
            if (directionTester > 0) {
                // Selection d'une direction aléatoire
                int dir = directionPossible[rand() % directionTester];

                // coordsX/Y[dir] contiennent le déplacement relatif (-1, 0 ou 1)
                // Utilisé pour palcer les rectangles de maniere coherente sur la carte
                int newX = roomCoords[randomRoomIndex].x + coordsX[dir];
                int newY = roomCoords[randomRoomIndex].y + coordsY[dir];

                // Vérification des superpos
                int superpos = 0;
                for (int j = 0; j < i; j++) {
                    if (roomCoords[j].x == newX && roomCoords[j].y == newY) {
                        superpos = 1;
                        break;
                    }
                }

                // Si ya pas de superposition
                if (!superpos) {
                    newRoom = initRoom();
                    newRoom->ID = i;
                    connectRoom(currentRoom, newRoom, dir);
                    roomCoords[i] = (SDL_Rect){newX, newY, 0, 0};
                    roomList[i] = newRoom;
                }
            }
            currentRoom->scene = NULL;
        } while (!newRoom);  // newRoom point sur NULL si on genere pas -> on enleve la variable de control
    }

    // Après la génération des salles normales
    int marchant = (rand() % 4) + 1;
    int boss = 5;

    // Détermine la direction et les offsets selon le type de marchand
    int dir_marchant, dir_boss;
    switch (marchant) {
        case 1:
            dir_marchant = 2;
            break;  // Haut -> Bas du marchand
        case 2:
            dir_marchant = 0;
            break;  // Droite -> Gauche du marchand
        case 3:
            dir_marchant = 3;
            break;  // Bas -> Haut du marchand
        case 4:
            dir_marchant = 1;
            break;  // Gauche -> Droite du marchand
    }

    switch (boss) {
        case 5:
            dir_boss = 2;
            break;  // Haut -> Bas du marchand
    }

    int trouve = 0;
    while (trouve == 0) {
        int i = rand() % (numberRoom - 2);
        t_salle* salle = roomList[i];

        // Calcule les nouvelles coordonnées
        int newX = roomCoords[i].x + coordsX[dir_boss];
        int newY = roomCoords[i].y + coordsY[dir_boss];

        // Vérifie les superpositions
        int collision = 0;
        for (int j = 0; j < numberRoom - 1; j++) {
            if (roomCoords[j].x == newX && roomCoords[j].y == newY) {
                collision = 1;
                break;
            }
        }
        // Cherche une salle valide pour la connexion
        trouve = 0;
        while (trouve == 0) {
            int i = rand() % (numberRoom - 2);
            t_salle* salle = roomList[i];

            // Calcule les nouvelles coordonnées
            int newX = roomCoords[i].x + coordsX[dir_marchant];
            int newY = roomCoords[i].y + coordsY[dir_marchant];

            // Vérifie les superpositions
            int collision = 0;
            for (int j = 0; j < numberRoom - 2; j++) {
                if (roomCoords[j].x == newX && roomCoords[j].y == newY) {
                    collision = 1;
                    break;
                }
            }

            if (!collision) {
                // Crée et connecte la salle marchande
                t_salle* marchand = initRoom();
                marchand->ID = -marchant;
                connectRoom(salle, marchand, dir_marchant);

                roomCoords[numberRoom - 2] = (SDL_Rect){newX, newY, 0, 0};
                roomList[numberRoom - 2] = marchand;
                trouve = 1;
                break;
            }
        }

        if (!collision) {
            // Crée et connecte la salle du boss
            t_salle* salleBoss = initRoom();
            salleBoss->ID = -boss;
            connectRoom(salle, salleBoss, dir_boss);

            roomCoords[numberRoom - 1] = (SDL_Rect){newX, newY, 0, 0};
            roomList[numberRoom - 1] = salleBoss;
            trouve = 1;
            break;
        }
    }

    // Debug: affiche toutes les coordonnées
    for (int i = 0; i < numberRoom; i++) {
        printf("X[%d] et Y[%d] et ID : %d\n", roomCoords[i].x, roomCoords[i].y, roomList[i]->ID);
    }

    printf("Aff\n\n\n");

    return roomList;
}
