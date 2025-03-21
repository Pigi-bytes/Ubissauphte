#ifndef OPTION_MENU_H
#define OPTION_MENU_H

#include "../commun.h"

t_option* creeOption();
void freeOption(void* elt);
void changementAffichage(t_fonctionParam* fonction);
t_scene* createOptionMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, SDL_Window* window, t_option* option, t_control* control, t_sceneController* sceneController);

#endif