#ifndef FPS_MENU_H
#define FPS_MENU_H
#include "../commun.h"

void afficherFps(t_fonctionParam* fonction);
void modifierFps(t_fonctionParam* fonction);
t_scene* createFpsMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, t_fpsDisplay* fpsDisplay, SDL_Window* window, t_option* option, t_sceneController* sceneController);

#endif