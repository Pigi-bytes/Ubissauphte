#ifndef ENEMY_H
#define ENEMY_H

#include "../../utils/timer.h"
#include "entity.h"
#include "systems/physicsSystem.h"
#include "tiles.h"

typedef struct t_enemy {
    t_entity entity;
    void (*update)(struct t_enemy*, float*, t_grid*, t_objectManager*);
    t_timer* wanderTimer;
    SDL_FPoint targetAcceleration;
} t_enemy;

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect);

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera);
void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
void freeEnemy(void* object);

#endif