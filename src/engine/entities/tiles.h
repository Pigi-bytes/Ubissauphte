#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "../../debug.h"
#include "../core/camera.h"
#include "../tileset.h"
#include "components/animation/animation.h"
#include "entity.h"

#define MAX_LINE_LENGTH 2048

typedef struct {
    SDL_bool solide;  // Propriété de collision
    t_entity entity;  // Entité associée pour le rendu
} t_tile;

typedef struct {
    int width;
    int height;
    int depth;
    t_tile*** tiles;  // [depth][height][width]
} t_grid;

t_tile* getTile(t_grid* grid, int x, int y, int z);
t_grid* createGrid(int width, int height, int depth);
t_grid* loadMap(char* filename, t_tileset* tileset);
void renderGrid(SDL_Renderer* renderer, t_grid* grid, t_camera* camera);
void freeGrid(void* object);
void placeOnRandomTile(t_grid* grid, t_entity* entity);
void SDL_DestroyTextureWrapper(void* object);

#endif