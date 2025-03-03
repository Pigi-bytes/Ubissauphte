#ifndef TILESET_H
#define TILESET_H

#include <SDL2/SDL.h>

#include "../io/imageLoader.h"
#include "../utils/objectManager.h"

typedef struct {
    int width;
    int height;
    int tileSize;
    t_objectManager* textureTiles;
} t_tileset;

t_tileset* initTileset(SDL_Renderer* renderer, int width, int height, int tileSize, char* filename);
void freeTileset(t_tileset* tileset);
void SDL_DestroyTextureWrapper(void* object);

#endif
