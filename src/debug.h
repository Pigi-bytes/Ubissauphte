#ifndef DEBUG_H
#define DEBUG_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE 1
#define DEBUG_MEMORY_MODE 0

#define COLORDEFAULT (SDL_Color){0, 0, 0, 0}

#define SDL_COLOR_RED (SDL_Color){255, 0, 0, 255}        // Rouge
#define SDL_COLOR_GREEN (SDL_Color){0, 255, 0, 255}      // Vert
#define SDL_COLOR_BLUE (SDL_Color){0, 0, 255, 255}       // Bleu
#define SDL_COLOR_YELLOW (SDL_Color){255, 255, 0, 255}   // Jaune
#define SDL_COLOR_MAGENTA (SDL_Color){255, 0, 255, 255}  // Magenta
#define SDL_COLOR_CYAN (SDL_Color){0, 255, 255, 255}     // Cyan
#define SDL_COLOR_WHITE (SDL_Color){255, 255, 255, 255}  // Blanc
#define SDL_COLOR_BLACK (SDL_Color){0, 0, 0, 255}        // Noir

#define SDL_COLOR_GRAY (SDL_Color){169, 169, 169, 255}      // Gris
#define SDL_COLOR_ORANGE (SDL_Color){255, 165, 0, 255}      // Orange
#define SDL_COLOR_PURPLE (SDL_Color){128, 0, 128, 255}      // Violet
#define SDL_COLOR_BROWN (SDL_Color){139, 69, 19, 255}       // Marron
#define SDL_COLOR_LIME (SDL_Color){0, 255, 0, 255}          // Lime
#define SDL_COLOR_TURQUOISE (SDL_Color){64, 224, 208, 255}  // Turquoise
#define SDL_COLOR_PINK (SDL_Color){255, 105, 180, 255}      // Rose
#define SDL_COLOR_INDIGO (SDL_Color){75, 0, 130, 255}       // Indigo

#if DEBUG_MODE

typedef struct DebugCommand DebugCommand;

void Debug_Init(SDL_Renderer *renderer);
void Debug_RenderAll();
void Debug_ClearStack();
void Debug_PushRect(const SDL_Rect *rect, int border_width, SDL_Color color);
void Debug_PushCircle(int x, int y, int radius, SDL_Color color);
void Debug_PushLine(int x1, int y1, int x2, int y2, int lineWidth, SDL_Color color);
void Debug_PushSector(float originX, float originY, float startAngle, float endAngle,
                      float radius, int segments, int lineWidth, SDL_Color color);
void Debug_RenderCircle(int x, int y, int radius);

#else
#define Debug_Init(a)
#define Debug_RenderAll()
#define Debug_ClearStack()
#define Debug_PushRect(a, b, c)
#define Debug_PushCircle(a, b, c, d)
#define Debug_PushLine(a, b, c, d, e, f)
#define Debug_PushSector(a, b, c, d, e, f, g, h)
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