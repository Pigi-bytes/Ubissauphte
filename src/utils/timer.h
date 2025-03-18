#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

typedef struct {
    Uint32 startTicks;
    SDL_bool isStarted;
} t_timer;

typedef struct {
    float accumulatedTime;
    SDL_bool isStarted;
} t_deltaTimer;

t_timer *initTimer();
void startTimer(t_timer *timer);
Uint32 getTicks(t_timer *timer);
void freeTimer(t_timer *timer);
void resetTimer(t_timer *timer);

t_deltaTimer *initDeltaTimer();
void startDeltaTimer(t_deltaTimer *timer);
void updateDeltaTimer(t_deltaTimer *timer, float deltaTime);
float getDeltaTimer(t_deltaTimer *timer);
void resetDeltaTimer(t_deltaTimer *timer);
void freeDeltaTimer(t_deltaTimer *timer);

#endif