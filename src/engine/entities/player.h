#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "../../debug.h"
#include "../../io/input.h"
#include "entity.h"
#include "systems/physicsSystem.h"
#include "tiles.h"

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} t_control;

typedef struct {
    t_entity entity;
    t_control* control;

    SDL_bool isAttacking;
    float attackAngle;
    float attackDuration;
    float attackTimer;
    t_circle attackHitbox;
} t_joueur;

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset);

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities);
void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp);
void freePlayer(void* object);

#endif