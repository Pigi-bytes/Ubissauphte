#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL2/SDL.h>

#include "../../entity.h"

typedef struct {
    SDL_FPoint velocity;      // Vélocité actuelle
    SDL_FPoint acceleration;  // Accélération (forces appliquées)
    float mass;               // 0 = infinité (statique)
    float friction;           // Frottement (pour ralentir l'entité)
    float restitution;        // Elasticité des collisions (0.0 = pas elastique, 1.0 = elastique)
} t_physics;

#endif