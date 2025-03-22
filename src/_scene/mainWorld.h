#ifndef MAIN_WORLD_H
#define MAIN_WORLD_H
#include "../commun.h"
#include "../engine/entities/tilesEntity.h"

t_scene* createMainWord(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, t_control* contr, t_sceneController* sceneController);
t_scene* createMapWord(SDL_Renderer* renderer, t_salle** salle, SDL_Rect* rectcord);
#endif