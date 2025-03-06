#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../../debug.h"
#include "../../io/input.h"
#include "../entities/entity.h"
#include "../entities/player.h"
#include "../world/tilesManager.h"

SDL_bool checkCircleRectCollision(t_circle* circle, SDL_Rect* rect, float* penetrationDepth, SDL_FPoint* normal);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, float*);

#endif