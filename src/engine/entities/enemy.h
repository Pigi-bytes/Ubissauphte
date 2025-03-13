#ifndef ENEMY_H
#define ENEMY_H

#include "../../utils/timer.h"
#include "entity.h"
#include "systems/physicsSystem.h"
#include "tiles.h"

typedef struct {
    t_entity entity;
    t_timer* wanderTimer;
    SDL_FPoint targetAcceleration;
} t_ennemi;

t_ennemi* createEnemy(SDL_Texture* texture, SDL_Rect rect);
void renderEnemy(SDL_Renderer* renderer, t_ennemi* enemy, t_camera* camera);
void updateEnemy(t_ennemi* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
void freeEnemy(void* object);

#endif