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
} t_enemy;

typedef enum {
    SLIME,
    CRABE,
    FANTOME
} EnemyType;

#define SLIME_PHYSICS enemyPhysics = {.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 1.0f, .friction = 0.05f, .restitution = 0.9f};
#define CRABE_PHYSICS enemyPhysics = {.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 5.0f, .friction = 0.06f, .restitution = 0.02f};
#define FANTOME_PHYSICS enemyPhysics = {.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 0.05f, .friction = 0.02f, .restitution = 0.05f};

#define SLIME_IDLE createAnimation(tileset, (int[]){1, 2}, 2, 240, true, "idle")
#define SLIME_WALK createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 240, true, "walk")

t_enemy* createEnemy(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, EnemyType type);
t_enemy* createRandomEnemy(SDL_Texture* texture, SDL_Rect rect, t_tileset* slimeTileSet, t_tileset* fantomTileSet, t_tileset* crabeTileSet);
void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera);
void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
void freeEnemy(void* object);

#endif