#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    Uint32 startTicks;
    Uint32 pausedTicks;
    bool isPaused;
    bool isStarted;
} t_timer;

t_timer *initTimer();
void startTimer(t_timer *timer);
void stopTimer(t_timer *timer);
void pauseTimer(t_timer *timer);
void unpauseTimer(t_timer *timer);
Uint32 getTicks(t_timer *timer);
void freeTimer(t_timer *timer);
void resetTimer(t_timer *timer);
#endif