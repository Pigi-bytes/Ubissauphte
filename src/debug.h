#ifndef DEBUG_H
#define DEBUG_H

#include <SDL2/SDL.h>
#include <stdio.h>

#define DEBUG_MODE 1

#define RECTANGLE_COLOR \
    (SDL_Color) { 41, 182, 246, 255 }

#if DEBUG_MODE
#define DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, rect, largeur)                                                      \
    {                                                                                                                 \
        SDL_Color currentColor;                                                                                       \
        SDL_GetRenderDrawColor(renderer, &currentColor.r, &currentColor.g, &currentColor.b, &currentColor.a);         \
        SDL_SetRenderDrawColor(renderer, RECTANGLE_COLOR.r, RECTANGLE_COLOR.g, RECTANGLE_COLOR.b, RECTANGLE_COLOR.a); \
        for (int i = 0; i < largeur; i++) {                                                                           \
            SDL_Rect debugRect = {rect.x - i, rect.y - i, rect.w + 2 * i, rect.h + 2 * i};                            \
            SDL_RenderDrawRect(renderer, &debugRect);                                                                 \
        }                                                                                                             \
        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);             \
    }
#else
#define DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, rect, largeur)
#endif

#if DEBUG_MODE
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] %s: " fmt "", __func__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#endif  // DEBUG_H
