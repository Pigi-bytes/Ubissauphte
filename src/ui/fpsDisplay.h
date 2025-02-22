#ifndef FPS_DISPLAY_H
#define FPS_DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../fps.h"
#include "text.h"

typedef struct fpsDisplay {
    t_text* fpsText;        // Texte affiché
    SDL_bool showFPS;       // Flag pour afficher ou non
    Uint32 lastUpdateTime;  // Dernière mise à jour de l'affichage
} t_fpsDisplay;

t_fpsDisplay* initFPSDisplay(SDL_Renderer* renderer, TTF_Font* font);
void updateFPSDisplay(t_fpsDisplay* display, t_frameData* frame, SDL_Renderer* renderer);
void renderFPSDisplay(t_fpsDisplay* display, SDL_Renderer* renderer);
void freeFPSDisplay(void* object);

#endif