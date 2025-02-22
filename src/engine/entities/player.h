#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "../../debug.h"
#include "entity.h"

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} t_control;

typedef struct {
    t_entity entity;
    t_control* control;
} t_joueur;

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect);
void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera);
void freePlayer(void* object);

#endif