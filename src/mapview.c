#include "mapview.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define OFFSET_X 700
#define OFFSET_Y 700

void XYMinMax(int *zoneAff, int WW, int WH, SDL_Rect *roomCoords, int numberRoom){
    zoneAff[0] = roomCoords[0].x;
    zoneAff[1] = roomCoords[0].x;
    zoneAff[2] = roomCoords[0].y;
    zoneAff[3] = roomCoords[0].y;

    for (int i = 1; i < numberRoom; i++) {
        if (roomCoords[i].x > zoneAff[1]){
            zoneAff[1] = roomCoords[i].x;
        }else if(roomCoords[i].x < zoneAff[0]){
            zoneAff[0] = roomCoords[i].x;
        }if (roomCoords[i].y > zoneAff[3]){
            zoneAff[3] = roomCoords[i].y;
        }else if(roomCoords[i].y < zoneAff[2]){
            zoneAff[2] = roomCoords[i].y;
        }
    }

    
    printf("Min X : %d, Max X : %d, Min Y : %d, Max Y : %d\n", zoneAff[0], zoneAff[1], zoneAff[2], zoneAff[3]);
    return;
}

void generateMap(SDL_Rect *roomCoords, t_salle **listeRoom, int numberRoom, t_mapAffichage *map, int WW, int WH) {
    map->numRooms = numberRoom;
    int zoneAff[4];
    XYMinMax(zoneAff, WW, WH, roomCoords, numberRoom);

    int nbX = abs(zoneAff[0])+zoneAff[1]+1;
    int nbY = abs(zoneAff[2])+zoneAff[3]+1;
    printf("Nb X : %d et nb Y : %d\n", nbX, nbY);
    
    
    int scale = ((WH/10)*8/(nbY))*0.75;
    int spacing = ((WH/10)*8/(nbY))*0.25;

    printf("Scale : %d\n", scale);

    zoneAff[0] = (abs(zoneAff[0]))*(scale + spacing) + WW/10;
    zoneAff[2] = (abs(zoneAff[2]))*(scale + spacing) + WH/10;

    for (int i = 0; i < numberRoom; i++) {
        roomCoords[i].x = roomCoords[i].x * scale + zoneAff[0] + spacing*roomCoords[i].x;
        roomCoords[i].y = roomCoords[i].y * scale + zoneAff[2] + spacing*roomCoords[i].y;
        roomCoords[i].w = scale;
        roomCoords[i].h = scale;
    }

    map->rooms = roomCoords;
    map->numLines = 0;
    map->lines = malloc(numberRoom * 4 * sizeof(Line));  // 4 porte par salle max

    for (int i = 0; i < numberRoom; i++) {
        t_salle *neighbors[4] = {listeRoom[i]->droite, listeRoom[i]->gauche, listeRoom[i]->haut, listeRoom[i]->bas};

        // Tableau des points de depart des lignes pour chaque direction
        // Ces points sont calcules en fonction de la position et de la taille de la salle actuelle
        int directions[4][2] = {
            {roomCoords[i].x + roomCoords[i].w, roomCoords[i].y + roomCoords[i].h / 2},  // Droite : milieu du cote droit
            {roomCoords[i].x, roomCoords[i].y + roomCoords[i].h / 2},                    // Gauche : milieu du cote gauche
            {roomCoords[i].x + roomCoords[i].w / 2, roomCoords[i].y},                    // Haut : milieu du cote haut
            {roomCoords[i].x + roomCoords[i].w / 2, roomCoords[i].y + roomCoords[i].h}   // Bas : milieu du cote bas
        };

        // Tableau des offsets pour dessiner les lignes dans chaque direction
        // Les offsets definissent la longueur et la direction de la ligne a partir du point de depart
        int offsets[4][2] = {
            {spacing, 0},   // Droite : ligne horizontale vers la droite (X + spacing, Y + 0)
            {-spacing, 0},  // Gauche : ligne horizontale vers la gauche (X - spacing, Y + 0)
            {0, -spacing},  // Haut : ligne verticale vers le haut (X + 0, Y - spacing)
            {0, spacing}    // Bas : ligne verticale vers le bas (X + 0, Y + spacing)
        };

        for (int j = 0; j < 4; j++) {
            if (neighbors[j]) {
                // Définit les coordonnees de la ligne dans la structure map
                // - (x1, y1) : point de depart de la ligne (milieu du cote de la salle)
                // - (x2, y2) : point d arrivee de la ligne (point de depart + offset)
                map->lines[map->numLines].x1 = directions[j][0];                  // Depart x
                map->lines[map->numLines].y1 = directions[j][1];                  // Depart y
                map->lines[map->numLines].x2 = directions[j][0] + offsets[j][0];  // Arrive x
                map->lines[map->numLines].y2 = directions[j][1] + offsets[j][1];  // Arrive y
                map->numLines++;
            }
        }
    }
}

void affichage(SDL_Renderer *renderer, t_mapAffichage *map) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < map->numRooms; i++) {
        SDL_RenderDrawRect(renderer, &map->rooms[i]);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < map->numLines; i++) {
        SDL_RenderDrawLine(renderer, map->lines[i].x1, map->lines[i].y1, map->lines[i].x2, map->lines[i].y2);
    }
}