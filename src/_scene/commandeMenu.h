#ifndef COMMANDE_MENU_H
#define COMMANDE_MENU_H
#include "../commun.h"

void renderTouche(SDL_Renderer* renderer, t_touche* touche);
void handleInputTouche(t_input* input, t_touche* touche, SDL_Renderer* renderer);
t_touche* createTouche(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick, SDL_Scancode* scancode, char* nom);
void miseAjourCommande(t_fonctionParam* fonction);
void freeTouche(void* elt);
t_scene* createCommandeMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, SDL_Window* window, t_option* option, t_control* control, t_sceneController* sceneController);

#endif