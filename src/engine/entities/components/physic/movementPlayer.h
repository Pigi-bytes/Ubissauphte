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

void resolveCollisions(t_entity* entity, t_grid* grid);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, float*);

void updatePhysics(t_joueur* joueur, float* deltaTime, t_grid* grid);

#endif