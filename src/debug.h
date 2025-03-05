#ifndef DEBUG_H
#define DEBUG_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE 1
#define DEBUG_MEMORY_MODE 1
#define RECTANGLE_COLOR (SDL_Color){41, 182, 246, 255}
#define CIRCLE_COLOR (SDL_Color){255, 0, 0, 255}

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
#define DEBUG_DRAW_CIRCLE(renderer, circle)                                                                   \
    {                                                                                                         \
        SDL_Color currentColor;                                                                               \
        SDL_GetRenderDrawColor(renderer, &currentColor.r, &currentColor.g, &currentColor.b, &currentColor.a); \
        SDL_SetRenderDrawColor(renderer, CIRCLE_COLOR.r, CIRCLE_COLOR.g, CIRCLE_COLOR.b, CIRCLE_COLOR.a);     \
        for (int w = 0; w < circle.radius * 2; w++) {                                                         \
            for (int h = 0; h < circle.radius * 2; h++) {                                                     \
                int dx = circle.radius - w;                                                                   \
                int dy = circle.radius - h;                                                                   \
                if ((dx * dx + dy * dy) <= (circle.radius * circle.radius)) {                                 \
                    SDL_RenderDrawPoint(renderer, circle.x + dx, circle.y + dy);                              \
                }                                                                                             \
            }                                                                                                 \
        }                                                                                                     \
        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);     \
    }

#else
#define DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, rect, largeur)
#define DEBUG_DRAW_CIRCLE(renderer, circle)
#endif

#if DEBUG_MODE
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] %s : " fmt, __func__, ##__VA_ARGS__)

#if DEBUG_MEMORY_MODE
#define DEBUG_MALLOC(size)                                           \
    ({                                                               \
        void *ptr = malloc(size);                                    \
        DEBUG_PRINT("[MALLOC] Alloué %zu octets à %p\n", size, ptr); \
        ptr;                                                         \
    })

#define DEBUG_REALLOC(ptr, size)                                         \
    ({                                                                   \
        void *new_ptr = realloc(ptr, size);                              \
        DEBUG_PRINT("[REALLOC] Réalloué %zu octets à %p \n", size, ptr); \
        new_ptr;                                                         \
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

#define DEBUG_SDL_CREATE_TEXTURE_FROM_SURFACE(renderer, surface)                \
    ({                                                                          \
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); \
        if (texture) {                                                          \
            DEBUG_PRINT("[MALLOC] Texture créée à %p\n", texture);              \
        }                                                                       \
        texture;                                                                \
    })

#define DEBUG_SDL_DESTROY_TEXTURE(texture)                          \
    do {                                                            \
        if (texture) {                                              \
            DEBUG_PRINT("[FREE] Texture détruite à %p\n", texture); \
            SDL_DestroyTexture(texture);                            \
        }                                                           \
    } while (0)

#define DEBUG_SDL_CREATE_TEXTURE(renderer, format, access, w, h)                  \
    ({                                                                            \
        SDL_Texture *texture = SDL_CreateTexture(renderer, format, access, w, h); \
        if (texture) {                                                            \
            DEBUG_PRINT("[MALLOC] Texture créée à %p\n", texture);                \
        }                                                                         \
        texture;                                                                  \
    })

#else
#define DEBUG_MALLOC(size) malloc(size)
#define DEBUG_REALLOC(ptr, size) realloc(ptr, size)
#define DEBUG_FREE(ptr) free(ptr)
#define DEBUG_FREE_SURFACE(surface) SDL_FreeSurface(surface)
#define DEBUG_SDL_CREATE_TEXTURE_FROM_SURFACE(renderer, surface) SDL_CreateTextureFromSurface(renderer, surface)
#define DEBUG_SDL_DESTROY_TEXTURE(texture) SDL_DestroyTexture(texture)
#define DEBUG_SDL_CREATE_TEXTURE(renderer, format, access, w, h) SDL_CreateTexture(renderer, format, access, w, h)
#endif

#define malloc(size) DEBUG_MALLOC(size)
#define realloc(ptr, size) DEBUG_REALLOC(ptr, size)
#define free(ptr) DEBUG_FREE(ptr)
#define SDL_CreateTexture(renderer, format, access, w, h) DEBUG_SDL_CREATE_TEXTURE(renderer, format, access, w, h)
#define SDL_CreateTextureFromSurface(renderer, surface) DEBUG_SDL_CREATE_TEXTURE_FROM_SURFACE(renderer, surface)
#define SDL_DestroyTexture(texture) DEBUG_SDL_DESTROY_TEXTURE(texture)

#else
#define DEBUG_PRINT(fmt, ...)
#endif

#endif  // DEBUG_H
