#include "genmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int genMap(int numberRoom) {
    srand(time(NULL));

    t_salle *roomList[numberRoom];
    roomList[0] = initRoom();

    for (int i = 1; i < numberRoom; i++) {  // Premiere salle deja initialisé
        t_salle *currentRoom, *newRoom = NULL;

        do {
            int randomRoom = rand() % i;
            currentRoom = roomList[randomRoom];

            if (!isRoomAllConnected(currentRoom)) {
                newRoom = initRoom();
                roomList[i] = newRoom;
                connectRoomRandom(currentRoom, newRoom);
            }
        } while (!newRoom);  // newRoom point sur NULL si on genere pas -> on enleve la variable de control
    }

    printf("Les %d salles ont été ajoutées\n", numberRoom);
    return 0;
}