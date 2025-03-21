#ifndef COMMANDE_MENU_H
#define COMMANDE_MENU_H
#include "../commun.h"

t_scene* createCommandeMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, SDL_Window* window, t_option* option, t_control* control, t_sceneController* sceneController);

#endif