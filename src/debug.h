#ifndef DEBUG_H
#define DEBUG_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE 1
#define DEBUG_MEMORY_MODE 0

#define RECTANGLE_COLOR (SDL_Color){41, 182, 246, 255}
#define CIRCLE_COLOR (SDL_Color){255, 0, 0, 255}

#if DEBUG_MODE

typedef struct DebugCommand DebugCommand;

void Debug_Init(SDL_Renderer *renderer);
void Debug_RenderAll(void);
void Debug_ClearStack(void);
void Debug_PushRect(const SDL_Rect *rect, int border_width);
void Debug_PushCircle(int x, int y, int radius);
void Debug_PushLine(int x1, int y1, int x2, int y2, SDL_Color color, int lineWidth);

#define DEBUG_DRAW_RECTANGLE_WITH_WIDTH(rect, largeur) Debug_PushRect(&(rect), largeur)
#define DEBUG_DRAW_CIRCLE(circle) Debug_PushCircle((circle).x, (circle).y, (circle).radius)

#else
#define DEBUG_DRAW_RECTANGLE_WITH_WIDTH(rect, largeur)
#define DEBUG_DRAW_CIRCLE(circle)
#define Debug_Init(renderer)
#define Debug_RenderAll()
#define Debug_ClearStack()
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

#define DEBUG_REALLOC(ptr, size)                                        \
    ({                                                                  \
        void *new_ptr = realloc(ptr, size);                             \
        DEBUG_PRINT("[REALLOC] Réalloué %zu octets à %p\n", size, ptr); \
        new_ptr;                                                        \
    })

#define DEBUG_FREE(ptr)                                    \
    do {                                                   \
        DEBUG_PRINT("[FREE] Mémoire libérée à %p\n", ptr); \
        free(ptr);                                         \
    } while (0)

#define DEBUG_SDL_CREATE_TEXTURE_FROM_SURFACE(renderer, surface)            \
    ({                                                                      \
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface); \
        if (tex) DEBUG_PRINT("[TEXTURE] Créée à %p\n", tex);                \
        tex;                                                                \
    })

#define DEBUG_SDL_DESTROY_TEXTURE(texture)                     \
    do {                                                       \
        if (texture) {                                         \
            DEBUG_PRINT("[TEXTURE] Détruite à %p\n", texture); \
            SDL_DestroyTexture(texture);                       \
        }                                                      \
    } while (0)

#define DEBUG_SDL_CREATE_TEXTURE(renderer, format, access, w, h)                  \
    ({                                                                            \
        SDL_Texture *texture = SDL_CreateTexture(renderer, format, access, w, h); \
        if (texture) {                                                            \
            DEBUG_PRINT("[MALLOC] Texture créée à %p\n", texture);                \
        }                                                                         \
        texture;                                                                  \
    })

#define malloc(size) DEBUG_MALLOC(size)
#define realloc(ptr, size) DEBUG_REALLOC(ptr, size)
#define free(ptr) DEBUG_FREE(ptr)
#define SDL_CreateTexture(renderer, format, access, w, h) DEBUG_SDL_CREATE_TEXTURE(renderer, format, access, w, h)
#define SDL_CreateTextureFromSurface(renderer, surface) DEBUG_SDL_CREATE_TEXTURE_FROM_SURFACE(renderer, surface)
#define SDL_DestroyTexture(texture) DEBUG_SDL_DESTROY_TEXTURE(texture)

#else
#define DEBUG_MALLOC(size) malloc(size)
#define DEBUG_REALLOC(ptr, size) realloc(ptr, size)
#define DEBUG_FREE(ptr) free(ptr)
#endif

#else
#define DEBUG_PRINT(fmt, ...)
#endif  // DEBUG_MODE

#endif  // DEBUG_H