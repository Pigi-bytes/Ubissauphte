#ifndef PHYSIC_H
#define PHYSIC_H

#include "../core.h"

typedef struct {
    SDL_FPoint velocity;      // Vélocité actuelle
    SDL_FPoint acceleration;  // Accélération (forces appliquées)
    float mass;               // 0 = infinité (statique)
    float friction;           // Frottement (pour ralentir l'entité)
    float restitution;        // Elasticité des collisions (0.0 = pas elastique, 1.0 = elastique)
} t_physics;

SDL_bool checkCircleRectCollision(t_circle* circle, SDL_Rect* rect, t_collisionData* out);
SDL_bool checkCircleCircleCollision(t_circle* a, t_circle* b, t_collisionData* out);

#endif