#include "timer.h"

#include <stdbool.h>

#include "../debug.h"

t_timer *initTimer() {
    t_timer *timer = malloc(sizeof(t_timer));
    timer->startTicks = 0;
    timer->pausedTicks = 0;
    timer->isPaused = false;
    timer->isStarted = false;
    return timer;
}

void startTimer(t_timer *timer) {
    timer->isStarted = true;
    timer->isPaused = false;
    timer->startTicks = SDL_GetTicks();
    timer->pausedTicks = 0;
    DEBUG_PRINT("Timer start à l'adresse : %p\n", timer);
}

void stopTimer(t_timer *timer) {
    timer->isStarted = false;
    timer->isPaused = false;
    timer->startTicks = 0;
    timer->pausedTicks = 0;
    DEBUG_PRINT("Timer stop à l'adresse : %p\n", timer);
}

void pauseTimer(t_timer *timer) {
    if (timer->isStarted && !timer->isPaused) {
        timer->isPaused = true;
        timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
        timer->startTicks = 0;
    }
    DEBUG_PRINT("Timer mit en pause à l'adresse : %p\n", timer);
}

void unpauseTimer(t_timer *timer) {
    if (timer->isStarted && timer->isPaused) {
        timer->isPaused = false;
        timer->startTicks = SDL_GetTicks() - timer->pausedTicks;
        timer->pausedTicks = 0;
    }
    DEBUG_PRINT("Timer remit en marche à l'adresse : %p\n", timer);
}

Uint32 getTicks(t_timer *timer) {
    Uint32 time = 0;
    if (timer->isStarted) {
        if (timer->isPaused) {
            time = timer->pausedTicks;
        } else {
            time = SDL_GetTicks() - timer->startTicks;
        }
    }

    return time;
}

void freeTimer(t_timer *timer) {
    if (timer != NULL) {
        free(timer);
    }
}

void resetTimer(t_timer *timer) {
    if (timer->isStarted) {
        timer->startTicks = SDL_GetTicks();
        timer->pausedTicks = 0;
    }
}