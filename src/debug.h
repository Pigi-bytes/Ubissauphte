#ifndef DEBUG_H
#define DEBUG_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

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
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] : " fmt "", ##__VA_ARGS__)

#define DEBUG_MALLOC(size)                                           \
    ({                                                               \
        void *ptr = malloc(size);                                    \
        DEBUG_PRINT("[MALLOC] Alloué %zu octets à %p\n", size, ptr); \
        ptr;                                                         \
    })

#define DEBUG_REALLOC(ptr, size)                                                  \
    ({                                                                            \
        void *new_ptr = realloc(ptr, size);                                       \
        DEBUG_PRINT("[REALLOC] Réalloué %zu octets à %p \n", size, ptr, new_ptr); \
        new_ptr;                                                                  \
    })

#define DEBUG_FREE(ptr)                                    \
    do {                                                   \
        DEBUG_PRINT("[FREE] Mémoire libérée à %p\n", ptr); \
        free(ptr);                                         \
    } while (0)

#define DEBUG_FREE_SURFACE(surface)                            \
    do {                                                       \
        DEBUG_PRINT("[FREE] Surface libérée à %p\n", surface); \
        SDL_FreeSurface(surface);                              \
    } while (0)

#define malloc(size) DEBUG_MALLOC(size)
#define realloc(ptr, size) DEBUG_REALLOC(ptr, size)
#define free(ptr) DEBUG_FREE(ptr)

#else
#define DEBUG_PRINT(fmt, ...)
#define malloc(size) malloc(size)
#define realloc(ptr, size) realloc(ptr)
#define free(ptr) free(ptr)
#endif

#endif  // DEBUG_H
