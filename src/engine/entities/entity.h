#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

#include "../../debug.h"
#include "../animation.h"
#include "../core/camera.h"

typedef struct {
    SDL_Rect rect;  // Position et taille dans le monde
    SDL_Texture* texture;
    t_animation* animation;
} t_entity;

void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera);

#endif