#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../debug.h"

SDL_Texture* loadImage(char* filename, SDL_Renderer* renderer);

#endif