#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "genmap.h"

t_salle *initRoom() {
    t_salle *room = malloc(sizeof(t_salle));
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

int genMap(int stage) {
    t_salle *firstRoom = initRoom();

    srand(time(NULL));
    int nbRoom = 5 + rand() % 10; // entre 5 et 14 salles à ajouter (firstRoom pas inclu donc c'est entre 6 et 15 salles), à revoir pour chaque étage
    nbRoom = 100; // Fixer temporairement à 10

    t_salle *tabRoomTemp[nbRoom];
    tabRoomTemp[0] = firstRoom;


    for (int i = 0; i < nbRoom - 1; i++) {
        int door = 1;
        while (door == 1) {
            int selectRandRoom = rand() % (i + 1); // [0,i] prend une case aléatoire du tableau des salles
            int selectRandomDoor = rand() % 4 + 1; // [1,4] prend une porte aléatoire d'une salle du tableau des salles

            t_salle *currentRoom = tabRoomTemp[selectRandRoom];

            if (selectRandomDoor == 1 && currentRoom->droite == NULL) {
                currentRoom->droite = initRoom();
                currentRoom->droite->gauche = currentRoom;
                tabRoomTemp[i + 1] = currentRoom->droite;
                door = 0;
            } else if (selectRandomDoor == 2 && currentRoom->gauche == NULL) {
                currentRoom->gauche = initRoom();
                currentRoom->gauche->droite = currentRoom;
                tabRoomTemp[i + 1] = currentRoom->gauche;
                door = 0;
            } else if (selectRandomDoor == 3 && currentRoom->haut == NULL) {
                currentRoom->haut = initRoom();
                currentRoom->haut->bas = currentRoom;
                tabRoomTemp[i + 1] = currentRoom->haut;
                door = 0;
            } else if (selectRandomDoor == 4 && currentRoom->bas == NULL) {
                currentRoom->bas = initRoom();
                currentRoom->bas->haut = currentRoom;
                tabRoomTemp[i + 1] = currentRoom->bas;
                door = 0;
            }

            if (door == 0){
                printf("Salle n°%d, porte ", selectRandRoom);
                if (selectRandomDoor == 1){
                    printf("de droite");
                }else if (selectRandomDoor == 2){
                    printf("de gauche");
                }else if (selectRandomDoor == 3){
                    printf("du haut");
                }else if (selectRandomDoor == 4){
                    printf("du bas");
                }
                printf(" : ajout de la salle n°%d\n", i+1);
            }
        }
    }
    printf("Les %d salles ont été ajoutées\n", nbRoom);

    return 0;
}
