#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

typedef struct {
    t_entity entity;
} t_ennemi;

t_ennemi* createEnemy(SDL_Texture* texture, SDL_Rect rect);
void renderEnemy(SDL_Renderer* renderer, t_ennemi* enemy, t_camera* camera);
void freeEnemy(void* object);

#endif