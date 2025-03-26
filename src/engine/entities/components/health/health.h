#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include <SDL2/SDL.h>

#include "../../../../context.h"
#include "../../../../utils/timer.h"
#include "../../../core/camera.h"

typedef struct s_healthSystem {
    int currentHealth;
    int maxHealth;

    SDL_bool showHealthBar;
    t_deltaTimer* healthBarTimer;
    float healthBarDuration;

    SDL_bool isFlashing;
    t_deltaTimer* flashTimer;
    float flashDuration;

    SDL_bool isInvincible;
    t_deltaTimer* invincibilityTimer;
    float invincibilityDuration;

    SDL_bool isDead;
    void (*onDeathCallback)(t_context* context, void* entity);
    void (*healthBareRender)(SDL_Renderer*, struct s_healthSystem*, SDL_Rect, t_camera*);
} t_healthSystem;

void initHealthSystem(t_healthSystem* health, int maxHealth);
void applyDamage(t_healthSystem* health, int damage, void* entity, t_context* context);
void updateHealthSystem(t_healthSystem* health, float deltaTime);
void freeHealthSystem(t_healthSystem* health);

void renderStandardHealthBar(SDL_Renderer* renderer, t_healthSystem* health, SDL_Rect entityRect, t_camera* camera);

#endif