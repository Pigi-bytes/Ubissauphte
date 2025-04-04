#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

/**
 * @struct t_timer
 * @brief Timer basé sur les ticks SDL
 */
typedef struct {
    Uint32 startTicks;   ///< Moment où le timer a été démarré (en ticks)
    SDL_bool isStarted;  ///< Indique si le timer est en cours d'exécution
} t_timer;

/**
 * @struct t_deltaTimer
 * @brief Timer accumulant un temps delta
 */
typedef struct {
    float accumulatedTime;  ///< Temps total accumulé
    SDL_bool isStarted;     ///< Indique si le timer est en cours d'exécution
} t_deltaTimer;

/**
 * @brief Initialise un nouveau timer
 * @return Pointeur vers le timer initialisé
 */
t_timer *initTimer();

/**
 * @brief Démarre le timer
 * @param timer Pointeur vers le timer à démarrer
 */
void startTimer(t_timer *timer);

/**
 * @brief Récupère le temps écoulé depuis le démarrage du timer
 * @param timer Pointeur vers le timer
 * @return Temps écoulé en millisecondes depuis le démarrage
 */
Uint32 getTicks(t_timer *timer);

/**
 * @brief Libère la mémoire allouée pour un timer
 * @param timer Pointeur vers le timer à libérer
 */
void freeTimer(t_timer *timer);

/**
 * @brief Réinitialise le timer au temps actuel
 * @param timer Pointeur vers le timer à réinitialiser
 */
void resetTimer(t_timer *timer);

/**
 * @brief Initialise un nouveau timer delta
 * @return Pointeur vers le timer delta initialisé
 */
t_deltaTimer *initDeltaTimer();

/**
 * @brief Démarre le timer delta et réinitialise son temps accumulé
 * @param timer Pointeur vers le timer delta
 */
void startDeltaTimer(t_deltaTimer *timer);

/**
 * @brief Met à jour le temps accumulé du timer delta
 * @param timer Pointeur vers le timer delta
 * @param deltaTime Temps à ajouter au temps accumulé
 */
void updateDeltaTimer(t_deltaTimer *timer, float deltaTime);

/**
 * @brief Récupère le temps accumulé par le timer delta
 * @param timer Pointeur vers le timer delta
 * @return Temps total accumulé
 */
float getDeltaTimer(t_deltaTimer *timer);

/**
 * @brief Réinitialise le temps accumulé du timer delta
 * @param timer Pointeur vers le timer delta à réinitialiser
 */
void resetDeltaTimer(t_deltaTimer *timer);

/**
 * @brief Libère la mémoire allouée pour un timer delta
 * @param timer Pointeur vers le timer delta à libérer
 */
void freeDeltaTimer(t_deltaTimer *timer);

#endif