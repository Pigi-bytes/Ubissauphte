#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;
    float radius;
} t_circle;

typedef struct {
    SDL_FPoint normal;
    float depth;
} t_collisionData;

#endif