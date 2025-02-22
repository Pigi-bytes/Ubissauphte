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

    t_salle** roomList = malloc(sizeof(t_salle) * numberRoom);
    roomList[0] = initRoom();
    roomList[0]->ID = 0;
    roomCoords[0] = (SDL_Rect){0, 0, 0, 0};

    // Offsets de coord par direction (Droite gauche haut bas)
    int coordsX[4] = {1, -1, 0, 0};  // Deplacement horizontaux
    int coordsY[4] = {0, 0, -1, 1};  // Deplacement verticaux

    for (int i = 1; i < numberRoom; i++) {
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
        } while (!newRoom);  // newRoom point sur NULL si on genere pas -> on enleve la variable de control
    }

    return roomList;
}