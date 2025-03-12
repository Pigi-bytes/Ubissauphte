#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../../../../debug.h"
#include "../../../../io/input.h"
#include "../../player.h"
#include "../../tiles.h"

typedef struct {
    SDL_FPoint normal;
    float depth;
} t_collisionData;

typedef struct {
    t_entity* entityA;
    t_entity* entityB;
    SDL_FPoint normal;
    float depth;
} t_entityCollision;

void resolveCollisions(t_entity* entity, t_grid* grid);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, float* deltaTime);
void updatePhysics(t_joueur* joueur, float* deltaTime, t_grid* grid);

#endif