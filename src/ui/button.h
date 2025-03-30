#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define SCALE_FACTOR 1.1f;

#include "../context.h"
#include "../io/input.h"
#include "../utils/fonctionManager.h"
#include "text.h"

typedef struct {
    SDL_Rect rect;
    SDL_Rect rectDefault;
    SDL_Color color;
    SDL_Color colorOnClick;
    SDL_Color colorDefault;
    t_text* label;
    t_fonctionParam* OnClick;
    SDL_bool isClicked;
    SDL_bool isHovered; 
} t_button;

t_button* createButton(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick);
void renderButton(t_context* ctx, t_button* button);
void handleInputButton(t_context* ctx, t_button* button);
SDL_bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);
void freeButton(void* object);

#endif  // BUTTON_H
