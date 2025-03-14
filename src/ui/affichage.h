#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <SDL2/SDL.h>

#include "../io/imageLoader.h"
#include "../io/input.h"

#define window_width 1280
#define window_height 960

void calculPlayer(SDL_Rect* Player, t_input* input, char* nom);
void calculSlot(SDL_Rect* slot, SDL_Rect* comp, t_input* input, char* nom1, char* nom2);
void calculDescrPlayer(SDL_Rect* slot1, SDL_Rect* slot2, SDL_Rect* player, SDL_Rect* descr, t_input* input);
void calculInventaire(SDL_Rect* Inv, SDL_Rect* descr, t_input* input);
void calculDescrItem(SDL_Rect* inv, SDL_Rect* descrItem, t_input* input);
void caculDescr(SDL_Rect* descrItem, SDL_Rect* descr, t_input* input);
void afficherInventaire(SDL_Renderer* renderer, t_input* input);

#endif