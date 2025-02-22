#ifndef FPS_H
#define FPS_H

#include "ui/text.h"
#include "utils/timer.h"

#define TARGET_FPS 60
#define SECONDE_EN_MS 1000

typedef struct {
    t_timer* frameTimer;       // Timer entre chaque frame
    t_timer* fpsTimer;         // Timer pour le calcul de fps
    Uint32 frameCount;         // Compteur de frame ddepuis la derniere mise a jour de FPS
    Uint32 fps;                // Valeur actuelle des FPS
    float deltaTime;           // Temps (en s) ecoulé depuis la derniere frame
    Uint32 lastFrameTime;      // Temps (en ms) de la derniere frame
    Uint32 targetFPS;          // FPS vise, 0 signifie pas de limite
    Uint32 fpsUpdateInterval;  // Intervalle de mise à jour des fps (en ms). Par défaut: 1 seconde
} t_frameData;

t_frameData* initFrameData(int targetFps);
void startFrame(t_frameData* frame);
void capFrameRate(t_frameData* frame);
void updateFPS(t_frameData* frame);
void freeFrameData(t_frameData* frame);

#endif