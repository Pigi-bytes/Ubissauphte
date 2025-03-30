#ifndef TOUCHE_H
#define TOUCHE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "../color.h"
#include "../io/input.h"
#include "../utils/fonctionManager.h"
#include "button.h"
#include "text.h"

typedef struct {
    t_button* button;
    SDL_Scancode* scancode;
    SDL_bool flagCommande;
    char nom[10];
} t_touche;

void renderTouche(t_context* ctx, t_touche* touche);
void handleInputTouche(t_context* ctx, t_touche* touche);
t_touche* createTouche(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick, SDL_Scancode* scancode, char* nom);
void miseAjourCommande(t_fonctionParam* fonction);
void freeTouche(void* elt);

#endif