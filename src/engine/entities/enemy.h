#ifndef ENEMY_H
#define ENEMY_H

#include "../../utils/timer.h"
#include "entity.h"
#include "systems/physicsSystem.h"
#include "tiles.h"

typedef struct t_enemy {
    t_entity entity;
    void (*update)(struct t_enemy*, float*, t_grid*, t_objectManager*);

    int health;
    int maxHealth;
    SDL_bool isInvincible;
    float invincibilityDuration;
    t_deltaTimer* invincibilityTimer;

    SDL_bool showHealthBar;
    t_deltaTimer* healthBarTimer;
} t_enemy;

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect);

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera);
void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
void freeEnemy(void* object);
void takeDamage(t_enemy* enemy, int damage);

#endif