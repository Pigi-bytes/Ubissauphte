#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL2/SDL.h>

#include "../../entity.h"

typedef struct {
    SDL_FPoint velocity;      // Vélocité actuelle
    SDL_FPoint acceleration;  // Accélération (forces appliquées)
    float mass;               // 0 = infinité (statique)
    float friction;           // Frottement (pour ralentir l'entité)
} t_physics;

typedef struct {
    t_entity* entityA;
    t_entity* entityB;
    SDL_FPoint normal;
    float depth;
} t_entityCollision;

#endif