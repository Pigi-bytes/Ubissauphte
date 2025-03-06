#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

#include "../../debug.h"
#include "../animation.h"
#include "../core/camera.h"

typedef struct {
    float x, y;
    float radius;
} t_circle;

typedef struct {
    SDL_Rect displayRect;  // Position et taille pour l'affichage

    SDL_Rect collisionRect;       // Position et taille pour les collisions rectangulaires
    t_circle collisionCircle;     // Position et taille pour les collisions circulaires
    SDL_bool useCircleCollision;  // Indicateur pour utiliser le cercle de collision

    SDL_Texture* texture;
    t_animationController* animationController;
    SDL_RendererFlip flip;
    SDL_bool debug;
} t_entity;

void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera);

#endif