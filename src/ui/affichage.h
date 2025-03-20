#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <SDL2/SDL.h>

#include "../engine/entities/systems/equipementsManager.h"
#include "../io/imageLoader.h"
#include "../io/input.h"
#include "button.h"
#include "text.h"

#define window_width 1280
#define window_height 960

void calculCasePlayer(SDL_Rect* casePlayer, t_input* input, char* nom);
void calculCaseSlots(SDL_Rect* comp, SDL_Rect* slot, t_input* input, char* nom1, char* nom2);
void calculDescrStatsPlayer(SDL_Rect* slotDroit, SDL_Rect* slotBas, SDL_Rect* casePlayer, SDL_Rect* caseStatsPlayer, t_input* input);
void calculInventaire(SDL_Rect* Inv, SDL_Rect* statsPlayer, t_input* input);
void calculStatsItem(SDL_Rect* inv, SDL_Rect* statsItem, t_input* input);
void caculDescrItem(SDL_Rect* statsItem, SDL_Rect* descrItem, t_input* input);
void afficherInventaire(SDL_Renderer* renderer, t_input* input, t_character* c, t_item* item);

#endif