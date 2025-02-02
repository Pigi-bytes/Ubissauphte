#include "mapview.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#define SCALE 50
#define OFFSET_X 700
#define OFFSET_Y 700
#define SPACING 25
#define DOOR_LENGTH 15

void generateMap(SDL_Rect *roomCoords, t_salle **listeRoom, int numberRoom, t_mapAffichage *map) {
    map->numRooms = numberRoom;

    for (int i = 0; i < numberRoom; i++) {
        roomCoords[i].x = roomCoords[i].x * SCALE + OFFSET_X;
        roomCoords[i].y = roomCoords[i].y * SCALE + OFFSET_Y;
        roomCoords[i].w = SCALE - SPACING;
        roomCoords[i].h = SCALE - SPACING;
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
            {DOOR_LENGTH, 0},   // Droite : ligne horizontale vers la droite (X + DOOR_LENGTH, Y + 0)
            {-DOOR_LENGTH, 0},  // Gauche : ligne horizontale vers la gauche (X - DOOR_LENGTH, Y + 0)
            {0, -DOOR_LENGTH},  // Haut : ligne verticale vers le haut (X + 0, Y - DOOR_LENGTH)
            {0, DOOR_LENGTH}    // Bas : ligne verticale vers le bas (X + 0, Y + DOOR_LENGTH)
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