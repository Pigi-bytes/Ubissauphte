#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "../utils/fonctionManager.h"

typedef struct {
    char* text;
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Rect rect;
} t_text;

TTF_Font* loadFont(char* filename, int size);
SDL_bool initTextEngine();
t_text createTextOutline(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color, SDL_Color colorOutline, int outlineSize);
t_text createText(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color);
void drawText(SDL_Renderer* renderer, t_text* text);
void drawallTextcall(t_fonctionParam* fonction);
void drawAllText(t_objectManager* manager, SDL_Renderer* renderer);
void freeText(void* objet);

#endif