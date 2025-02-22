#include "frame.h"

t_frameData* initFrameData(int targetFps) {
    t_frameData* frame = malloc(sizeof(t_frameData));

    frame->frameTimer = initTimer();
    frame->fpsTimer = initTimer();

    frame->frameCount = 0;
    frame->fps = 0;
    frame->deltaTime = 0.0f;
    frame->lastFrameTime = 0;

    frame->targetFPS = targetFps;
    frame->fpsUpdateInterval = SECONDE_EN_MS;

    startTimer(frame->fpsTimer);
    startTimer(frame->frameTimer);

    return frame;
}

void startFrame(t_frameData* frame) {
    // Calcul du deltaTime (temps ecoule depuis la derniere frame en secondes):
    // currentFrameTime: temps actuel en ms
    // lastFrameTime: temps de la dernière frame en ms
    // deltaTime = différence en ms convertie en secondes (divisé par 1000)
    //
    // Exemple: 16ms entre deux frames => deltaTime = 0.016s
    Uint32 currentFrameTime = getTicks(frame->frameTimer);
    frame->deltaTime = (currentFrameTime - frame->lastFrameTime) / (float)SECONDE_EN_MS;
    frame->lastFrameTime = currentFrameTime;
}

void capFrameRate(t_frameData* frame) {
    if (frame->targetFPS == 0) return;  // Pas de limite si targetFPS est 0

    // Temps que la frame actuelle a pris (en ms)
    Uint32 frameDuration = getTicks(frame->frameTimer) - frame->lastFrameTime;
    // Calcul du délai cible par frame en ms:
    // targetDelay = 1000 ms / targetFPS
    //
    // Exemple: 60 FPS => 1000/60 ~ 16.6ms par frame
    Uint32 targetDelay = SECONDE_EN_MS / frame->targetFPS;
    // Si la frame a ete traité plus vite que le delai voulu, on attend la difference pour cap
    if (frameDuration < targetDelay) {
        SDL_Delay(targetDelay - frameDuration);
    }
}

void updateFPS(t_frameData* frame) {
    frame->frameCount++;

    // Vérifie si l'intervalle de mise à jour du FPS est atteint
    if (getTicks(frame->fpsTimer) >= frame->fpsUpdateInterval) {
        // Calcul du FPS:
        // elapsed: temps écoulé en secondes depuis la derniere mise à jour
        // fps = nombre de frames / temps écoule
        //
        // Exemple: 60 frames en 1 seconde => 60 FPS

        float elapsed = getTicks(frame->fpsTimer) / (float)SECONDE_EN_MS;  // Conversion ms en secondes
        frame->fps = (Uint32)(frame->frameCount / elapsed);                // Arrondi a l'entier
        frame->frameCount = 0;
        resetTimer(frame->fpsTimer);
    }
}

void freeFrameData(t_frameData* frame) {
    if (frame) {
        freeTimer(frame->frameTimer);
        freeTimer(frame->fpsTimer);
        free(frame);
    }
}
