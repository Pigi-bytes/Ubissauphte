#include "genmap.h"
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

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

int isRoomAllConnected(t_salle *room) {
    return (room->droite != NULL && room->gauche != NULL && room->haut != NULL && room->bas != NULL);
}

void connectRoomRandom(t_salle *origineRoom, t_salle *destinationRoom) {
    t_salle **doorOrigin[4] = {&origineRoom->droite, &origineRoom->gauche, &origineRoom->haut, &origineRoom->bas};
    t_salle **doorDestination[4] = {&destinationRoom->gauche, &destinationRoom->droite, &destinationRoom->bas, &destinationRoom->haut};

    int randomDoor;
    do {
        randomDoor = rand() % 4;
    } while (*doorOrigin[randomDoor] != NULL);

    *doorOrigin[randomDoor] = destinationRoom;
    *doorDestination[randomDoor] = origineRoom;
}

t_salle **genMap(int numberRoom, int *tabCoordsX, int *tabCoordsY) {
    int seed = time(NULL);
    srand(seed);
    printf("Seed : %d\n", seed);

    t_salle **roomList = malloc(sizeof(t_salle)*numberRoom);
    roomList[0] = initRoom();

    tabCoordsX[0] = 0;
    tabCoordsY[0] = 0;
    printf("Début de la génération des salles :\n");
    for (int i = 1; i < numberRoom; i++) {  // Premiere salle deja initialisé
        t_salle *currentRoom, *newRoom = NULL;
        printf("\nGénère la salle n°%d\n", i);
        do {
            int randomRoom = rand() % i;
            currentRoom = roomList[randomRoom];
            printf("Random : %d\n",randomRoom);
            if (!isRoomAllConnected(currentRoom)) {
                int superpos = 0;
                for (int k = 0; k < i; k++){
                    printf("\nk : %d\n",k);
                    printf("i : %d\n",i);
                    printf("tabX[RDM] : %d\n",tabCoordsX[randomRoom]);
                    printf("tabX[k] : %d\n",tabCoordsX[k]);
                    printf("tabY[RDM] : %d\n",tabCoordsY[randomRoom]);
                    printf("tabY[k] : %d\n",tabCoordsY[k]);
                    if (randomRoom != k){
                        if (currentRoom->droite == NULL && tabCoordsX[randomRoom]+1 == tabCoordsX[k] && tabCoordsY[randomRoom] == tabCoordsY[k]){
                            superpos = 1;
                            printf("1Superpos donc break !!!\n");
                            break;
                        }else if (currentRoom->gauche == NULL && tabCoordsX[randomRoom]-1 == tabCoordsX[k] && tabCoordsY[randomRoom] == tabCoordsY[k]){
                            superpos = 1;
                            printf("2Superpos donc break !!!\n");
                            break;
                        }else if (currentRoom->haut == NULL && tabCoordsX[randomRoom] == tabCoordsX[k] && tabCoordsY[randomRoom]+1 == tabCoordsY[k]){
                            superpos = 1;
                            printf("3Superpos donc break !!!\n");
                            break;
                        }else if (currentRoom->bas == NULL && tabCoordsX[randomRoom] == tabCoordsX[k] && tabCoordsY[randomRoom]-1 == tabCoordsY[k]){
                            superpos = 1;
                            printf("4Superpos donc break !!!\n");
                            break;
                        }
                    }
                }
                if (superpos == 0){
                    newRoom = initRoom();
                    roomList[i] = newRoom;
                    connectRoomRandom(currentRoom, newRoom);
                }
                ///////// J'aimerais prendre l'ID de la currentRoom pour voir si les coord X+1, Y+1, X-1, Y-1 ne sont pas déjà dans le tab ////////
                
                printf("A");
            }
            printf("W");
        } while (!newRoom);  // newRoom point sur NULL si on genere pas -> on enleve la variable de control

        roomList[i]->ID = i; // Ancienneté de la salle


        printf("%d ",i);
        int direction;
        if (newRoom->haut != NULL && (newRoom->haut->ID < newRoom->ID)){ // regarde les voisines plus vielles
            direction = 1;
            tabCoordsX[i] = tabCoordsX[newRoom->haut->ID];
            tabCoordsY[i] = tabCoordsY[newRoom->haut->ID]-1;
            printf("N°%d ", newRoom->haut->ID);
            printf("Haut\n");
        }else if (newRoom->droite != NULL && (newRoom->droite->ID < newRoom->ID)){
            direction = 2;
            tabCoordsX[i] = tabCoordsX[newRoom->droite->ID]-1;
            tabCoordsY[i] = tabCoordsY[newRoom->droite->ID];
            printf("N°%d ", newRoom->droite->ID);
            printf("Droite\n");
        }else if (newRoom->bas != NULL && (newRoom->bas->ID < newRoom->ID)){
            direction = 3;
            tabCoordsX[i] = tabCoordsX[newRoom->bas->ID];
            tabCoordsY[i] = tabCoordsY[newRoom->bas->ID]+1;
            printf("N°%d ", newRoom->bas->ID);
            printf("Bas\n");
        }else if (newRoom->gauche != NULL && (newRoom->gauche->ID < newRoom->ID)){
            direction = 4;
            tabCoordsX[i] = tabCoordsX[newRoom->gauche->ID]+1;
            printf("N°%d ", newRoom->gauche->ID);
            tabCoordsY[i] = tabCoordsY[newRoom->gauche->ID];
            printf("Gauche\n");
        }else{
            printf("\n\n///////////////ERREURE/////////////// \n\n");
            if (newRoom->gauche != NULL){
                printf("gauche : %d ID\n", newRoom->gauche->ID);
            }if (newRoom->droite != NULL){
                printf("droite : %d ID\n", newRoom->droite->ID);
            }if (newRoom->haut != NULL){
                printf("haut : %d ID\n", newRoom->haut->ID);
            }if (newRoom->bas != NULL){
                printf("bas : %d ID\n", newRoom->bas->ID);
            }
        }
        printf("tabCoordX [%d] : %d X et %d Y\n", i, tabCoordsX[i], tabCoordsY[i]);
    }

    printf("Les %d salles ont été ajoutées\n", numberRoom);
    return roomList;
}

