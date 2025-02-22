#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../../debug.h"
#include "../../io/input.h"
#include "../entities/player.h"
#include "../world/tilesManager.h"

SDL_bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2);
void movePlayer(t_joueur* player, int dx, int dy, t_grid* grid);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid);

#endif