#ifndef MAP_H
#define MAP_H

typedef struct t_salle {
    int ID;                  // ID/Numéro de la salle
    struct t_salle *droite;  // Porte droite
    struct t_salle *gauche;  // Porte gauche
    struct t_salle *haut;    // Porte haut
    struct t_salle *bas;     // Porte bas
} t_salle;

t_salle *initRoom();
int isRoomAllConnected(t_salle *room);
void connectRoomRandom(t_salle *origineRoom, t_salle *destinationRoom);
t_salle **genMap(int numberRoom, int *tabCoordX, int *tabCoordY);

#endif