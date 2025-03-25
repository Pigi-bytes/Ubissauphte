#ifndef ENEMY_H
#define ENEMY_H

#include "../../utils/fscene.h"
#include "../../utils/timer.h"
#include "components/physic/physics.h"  // Include physics.h before defining t_entity
#include "entity.h"
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

    SDL_bool isFlashing;
    t_deltaTimer* flashTimer;
    float flashDuration;

    SDL_bool isDead;
} t_enemy;

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene);

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera);
void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
void freeEnemy(void* object);
void takeDamageAndCheckDeath(t_enemy* enemy, int damage);

#endif