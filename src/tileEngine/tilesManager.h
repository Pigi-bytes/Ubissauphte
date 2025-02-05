#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <SDL2/SDL.h>

#include "../io/imageLoader.h"
#include "../utils/objectManager.h"
#include "camera.h"

typedef struct {
    int width;
    int height;
    int tileSize;
    t_objectManager* textureTiles;
} t_tileset;

typedef struct {
    // X, Y et Z relative a la premiere tuile
    int x;
    int y;
    int z;
    SDL_Texture* texture;
    SDL_RendererFlip flip;
    SDL_bool solide;
} t_tile;

typedef struct {
    int width;
    int height;
    int depth;
    t_tile*** tiles;  // [depth][height][width]
} t_grid;

t_tileset* initTileset(SDL_Renderer* renderer, int width, int height, int tileSize, char* filename);
void freeTileset(t_tileset* tileset);
t_tile* getTile(t_grid* grid, int x, int y, int z);

t_grid* createGrid(int width, int height, int depth);
t_grid* loadMap(char* filename, t_tileset* tileset);
void renderGrid(SDL_Renderer* renderer, t_grid* grid, t_camera* camera, t_tileset* tileset);
void freeGrid(t_grid* grid);

void SDL_DestroyTextureWrapper(void* object);
void appliquerTextureNiveau(t_grid* grid, int z, void* textureV);

#endif