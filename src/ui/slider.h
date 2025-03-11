

#ifndef SLIDER_H
#define SLIDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "../io/input.h"
#include "../utils/fonctionManager.h"
#include "button.h"

typedef struct {
    SDL_Rect barre;
    float volumme;
    SDL_Rect curseur;
    SDL_Rect curseurDefault;
    SDL_Color colorBarre;
    SDL_Color colorCurseur;
    SDL_bool isClicked;
    t_fonctionParam* OnClick;

} t_barreVolumme;

t_barreVolumme* CreerBarreVolume(SDL_Rect barre, SDL_Rect curseur, SDL_Color couleur, SDL_Color couleurCurseur, t_fonctionParam* OnClick);
void renderBarreVolumme(SDL_Renderer* renderer, t_barreVolumme* barre);
void handleInputButtonVolumme(t_input* input, t_barreVolumme* barre);

void freeBv(void* elt);

#endif