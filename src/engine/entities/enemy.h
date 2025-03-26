#ifndef ENEMY_H
#define ENEMY_H

#include "../../engine/world/genmap.h"
#include "../../utils/fscene.h"
#include "../../utils/timer.h"
#include "components/health/health.h"  // Include physics.h before defining t_entity
#include "components/physic/physics.h"
#include "entity.h"
#include "tiles.h"

typedef struct t_enemy {
    t_entity entity;
    t_healthSystem health;

    void (*update)(struct t_enemy*, float*, t_grid*, t_objectManager*);
} t_enemy;

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene);

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera);
void updateEnemy(t_enemy* enemy, float* deltaTime, t_salle* grid, t_objectManager* entities);
void freeEnemy(void* object);
void takeDamageAndCheckDeath(t_enemy* enemy, int damage);

#endif