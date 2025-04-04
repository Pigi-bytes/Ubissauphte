#ifndef GHOST_H
#define GHOST_H

#include "../enemy.h"



typedef struct {
    t_enemy base;

    float moveSpeed;
    SDL_FPoint targetPos;
    float targetChangeTime;

    float verticalAmplitude, horizontalAmplitude;
    float verticalFrequency, horizontalFrequency;
    float verticalOffset, horizontalOffset;

    t_deltaTimer* timer;
    float elapsedTime;

    int transparency;
    SDL_bool isBoosting;
    float boostTimer;
    float boostCooldown;
} t_ghost;

t_enemy* createGhost(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);
void updateGhost(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);

#endif