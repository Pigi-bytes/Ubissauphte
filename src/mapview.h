#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <SDL2/SDL.h>

#include "debug.h"
#include "genmap.h"

typedef struct {
    int x1, y1, x2, y2;
} Line;

typedef struct {
    SDL_Rect *rooms;
    int numRooms;
    Line *lines;
    int numLines;
} t_mapAffichage;

void XYMinMax(int *zonneAff, int WW, int WH, SDL_Rect *roomCoords, int numberRoom);
void generateMap(SDL_Rect *roomCoords, t_salle **listeRoom, int numberRoom, t_mapAffichage *map, int WW, int WH);
void affichage(SDL_Renderer *pRendu, t_mapAffichage *map);
#endif