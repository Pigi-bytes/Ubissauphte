#ifndef MAIN_WORLD_H
#define MAIN_WORLD_H
#include "../commun.h"
#include "../context.h"
#include "../engine/entities/tilesEntity.h"

t_scene* createMainWord(t_context* context);

t_scene* createMapWord(SDL_Renderer* renderer, t_salle** salle, SDL_Rect* rectcord);
#endif