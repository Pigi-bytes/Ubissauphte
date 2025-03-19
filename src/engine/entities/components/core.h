#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;
    float radius;
} t_circle;

typedef struct {
    SDL_FPoint origin;
    float startAngle;  // Angle de départ en radians
    float endAngle;    // Angle de fin en radians
    float radius;      // Portée du secteur
} t_sector;

typedef struct {
    SDL_FPoint normal;
    float depth;
} t_collisionData;

#define M_PI 3.14159265358979323846
#define EPSILON 0.001f

#endif