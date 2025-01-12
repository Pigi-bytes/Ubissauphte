#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* text;
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Color color;
    SDL_Rect rect;
} t_text;