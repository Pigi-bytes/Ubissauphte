#include "fpsDisplay.h"

t_fpsDisplay* initFPSDisplay(SDL_Renderer* renderer, TTF_Font* font) {
    t_fpsDisplay* display = malloc(sizeof(t_fpsDisplay));

    SDL_Color white = {255, 255, 255, 255};
    display->fpsText = createText(renderer, "FPS: %00 | Delta: %00.00fms", font, white);
    display->fpsText->rect.x = 10;
    display->fpsText->rect.y = 100;

    display->showFPS = SDL_TRUE;
    display->lastUpdateTime = 0;  // Initialisé à 0

    return display;
}

void updateFPSDisplay(t_fpsDisplay* display, t_frameData* frame, SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks();

    if (display->showFPS && (currentTime - display->lastUpdateTime) >= frame->fpsUpdateInterval) {
        char fpsStr[50];
        sprintf(fpsStr, "FPS: %d | Delta: %2.2fms", frame->fps, frame->deltaTime * SECONDE_EN_MS);
        updateText(&(display->fpsText), renderer, fpsStr, (SDL_Color){255, 255, 255, 255});
        display->lastUpdateTime = currentTime;
    }
}

void renderFPSDisplay(SDL_Renderer* renderer, t_fpsDisplay* display) {
    if (display->showFPS) {
        renderText(renderer, display->fpsText);
    }
}

void freeFPSDisplay(void* object) {
    t_fpsDisplay* display = (t_fpsDisplay*)object;
    if (display) {
        freeText(display->fpsText);
        free(display);
    }
}