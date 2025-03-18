#include "timer.h"

#include "../debug.h"

t_timer *initTimer() {
    t_timer *timer = malloc(sizeof(t_timer));
    timer->isStarted = SDL_FALSE;
    timer->startTicks = 0;
    return timer;
}

void startTimer(t_timer *timer) {
    timer->isStarted = SDL_TRUE;
    timer->startTicks = SDL_GetTicks();
}

Uint32 getTicks(t_timer *timer) {
    Uint32 time = 0;
    if (timer->isStarted) {
        time = SDL_GetTicks() - timer->startTicks;
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
    }
}

t_deltaTimer *initDeltaTimer() {
    t_deltaTimer *timer = malloc(sizeof(t_deltaTimer));
    timer->isStarted = SDL_FALSE;
    timer->accumulatedTime = 0;
    return timer;
}

void startDeltaTimer(t_deltaTimer *timer) {
    timer->isStarted = SDL_TRUE;
    timer->accumulatedTime = 0;
}

void updateDeltaTimer(t_deltaTimer *timer, float deltaTime) {
    if (timer->isStarted) {
        timer->accumulatedTime += deltaTime;
    }
}

float getDeltaTimer(t_deltaTimer *timer) {
    return timer->accumulatedTime;
}

void resetDeltaTimer(t_deltaTimer *timer) {
    timer->accumulatedTime = 0;
}

void freeDeltaTimer(t_deltaTimer *timer) {
    if (timer != NULL) {
        free(timer);
    }
}