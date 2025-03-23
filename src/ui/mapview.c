#include "mapview.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

void XYMinMax(int *zoneAffichage, SDL_Rect *roomCoords, int numberRoom) {
    zoneAffichage[0] = roomCoords[0].x;
    zoneAffichage[1] = roomCoords[0].x;
    zoneAffichage[2] = roomCoords[0].y;
    zoneAffichage[3] = roomCoords[0].y;

    for (int i = 1; i < numberRoom; i++) {
        if (roomCoords[i].x > zoneAffichage[1]) {
            zoneAffichage[1] = roomCoords[i].x;
        } else if (roomCoords[i].x < zoneAffichage[0]) {
            zoneAffichage[0] = roomCoords[i].x;
        }
        if (roomCoords[i].y > zoneAffichage[3]) {
            zoneAffichage[3] = roomCoords[i].y;
        } else if (roomCoords[i].y < zoneAffichage[2]) {
            zoneAffichage[2] = roomCoords[i].y;
        }
    }

    return;
}

void generateMap(SDL_Rect *roomCoords, t_salle **listeRoom, int numberRoom, t_mapAffichage *map, int windowWidth, int windowHeight) {
    map->numRooms = numberRoom;

    // zoneAffichage prend les min et max de X et Y avec la fonction
    int zoneAffichage[4];
    XYMinMax(zoneAffichage, roomCoords, numberRoom);

    // calcul la taille des salles X et Y (le X n'est pas utilisé ensuite car on se base sur le plus petit, donc Y)
    //  int distX = abs(zoneAffichage[0])+zoneAffichage[1]+1;
    int distY = abs(zoneAffichage[2]) + zoneAffichage[3] + 1;

    // Scale (affichage d'UNE salle en carré)
    // Le calcul : 80% de Windows Height, divisé par la taille des salles Y, 0.75 représente la taille de la salle
    // le 0.25 c'est le reste pour l'espacement, ça donne un ration de 75% de taille et 25% d'espace entre les salles
    int scale = ((windowHeight / 10) * 8 / (distY)) * 0.75;
    int spacing = ((windowHeight / 10) * 8 / (distY)) * 0.25;

    // Sert à faire une marge pour ne pas que ça soit collé au haut et à gauche
    // le calcul, ça prend la coord de la plus petite salle, et sa pousse tout pour ne pas que ça rentre dans le mur
    // et on ajoute 10%, pour avoir un truc propre
    zoneAffichage[0] = (abs(zoneAffichage[0])) * (scale + spacing) + windowWidth / 10;
    zoneAffichage[2] = (abs(zoneAffichage[2])) * (scale + spacing) + windowHeight / 10;

    for (int i = 0; i < numberRoom; i++) {
        roomCoords[i].x = roomCoords[i].x * scale + zoneAffichage[0] + spacing * roomCoords[i].x;
        roomCoords[i].y = roomCoords[i].y * scale + zoneAffichage[2] + spacing * roomCoords[i].y;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < map->numRooms; i++) {
        SDL_RenderDrawRect(renderer, &map->rooms[i]);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < map->numLines; i++) {
        SDL_RenderDrawLine(renderer, map->lines[i].x1, map->lines[i].y1, map->lines[i].x2, map->lines[i].y2);
    }
}

void freeMapAffiche(void *elt) {
    free((t_mapAffichage *)elt);
}