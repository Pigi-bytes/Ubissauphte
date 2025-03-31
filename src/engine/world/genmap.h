#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../debug.h"
#include "../../utils/fscene.h"
#include "../../utils/objectManager.h"
#include "../entities/tiles.h"

typedef struct t_salle {
    int ID;                  // ID/Numéro de la salle
    struct t_salle *droite;  // Porte droite
    struct t_salle *gauche;  // Porte gauche
    struct t_salle *haut;    // Porte haut
    struct t_salle *bas;     // Porte bas
    t_scene *scene;
    t_grid *grille;
    t_objectManager *entities;
} t_salle;

typedef struct {
    t_salle **salles;
    int nbSalles;
    SDL_Rect *rectcord;

} t_level;

t_salle *
initRoom();
void connectRoom(t_salle *origineRoom, t_salle *destinationRoom, int dir);
t_salle **genMap(int numberRoom, SDL_Rect *roomCoords);
void freeSalle(t_salle *salle);

#endif
