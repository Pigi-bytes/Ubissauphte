#include "health.h"

#include <math.h>

#define HEALTH_ANIMATION_SPEED 0.25f
#define HEALTH_ANIMATION_EPSILON 0.001f

void initHealthSystem(t_healthSystem* health, int maxHealth) {
    health->currentHealth = maxHealth;
    health->maxHealth = maxHealth;

    health->displayedHealthRatio = 1.0f;
    health->targetHealthRatio = 1.0f;

    health->showHealthBar = SDL_FALSE;
    health->healthBarTimer = initDeltaTimer();
    health->healthBarDuration = 3.0f;
    health->healthBareRender = renderStandardHealthBar;
    startDeltaTimer(health->healthBarTimer);

    health->isFlashing = SDL_FALSE;
    health->flashTimer = initDeltaTimer();
    health->flashDuration = 0.17f;
    startDeltaTimer(health->flashTimer);

    health->isInvincible = SDL_FALSE;
    health->invincibilityTimer = initDeltaTimer();
    health->invincibilityDuration = 1.0f;
    startDeltaTimer(health->invincibilityTimer);

    health->isDead = SDL_FALSE;
    health->onDeathCallback = NULL;
}

void applyDamage(t_healthSystem* health, int damage, void* entity, t_context* context) {
    if (!health || health->isInvincible || health->isDead) return;

    health->showHealthBar = SDL_TRUE;
    resetDeltaTimer(health->healthBarTimer);

    health->isFlashing = SDL_TRUE;
    resetDeltaTimer(health->flashTimer);

    health->currentHealth -= damage;
    if (health->currentHealth <= 0) {
        health->currentHealth = 0;
        health->showHealthBar = SDL_FALSE;
        health->isDead = SDL_TRUE;

        health->targetHealthRatio = 0.0f;

        if (health->onDeathCallback) {
            printf("%p", context->sceneController);
            health->onDeathCallback(context, entity);
        }
        return;
    }

    health->targetHealthRatio = (float)health->currentHealth / (float)health->maxHealth;

    health->isInvincible = SDL_TRUE;
    resetDeltaTimer(health->invincibilityTimer);
}

void renderStandardHealthBar(SDL_Renderer* renderer, t_healthSystem* health, SDL_Rect entityRect, t_camera* camera) {
    float healthRatio = health->displayedHealthRatio;

    float barWidth = entityRect.w * 0.8f;
    float barHeight = 3.0f;
    float barX = entityRect.x + (entityRect.w - barWidth) / 2;
    float barY = entityRect.y - 12.0f;
    float fillWidth = barWidth * healthRatio;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &(SDL_Rect){barX, barY + 1, barWidth, barHeight});

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 180);
    SDL_RenderFillRect(renderer, &(SDL_Rect){barX, barY, barWidth, barHeight});

    float pulseIntensity = 0.85f + (sinf(SDL_GetTicks() * 0.005f) * 0.15f);

    Uint8 r = 0, g = 0, b = 0;
    if (healthRatio > 0.75f) {
        // Bleu/turquoise -> vert
        r = 0;
        g = 255;
        b = (Uint8)(255 * (1.0f - (healthRatio - 0.75f) * 4.0f));
    } else if (healthRatio > 0.5f) {
        // Vert -> jaune
        r = (Uint8)(255 * (healthRatio - 0.5f) * 4.0f);
        g = 255;
        b = 0;
    } else if (healthRatio > 0.25f) {
        // Jaune -> orange
        r = 255;
        g = (Uint8)(255 * (healthRatio - 0.25f) * 4.0f);
        b = 0;
    } else {
        // Orange -> rouge
        r = 255;
        g = (Uint8)(128 * healthRatio * 4.0f);
        b = 0;
    }

    SDL_SetRenderDrawColor(renderer, (Uint8)(r * pulseIntensity), (Uint8)(g * pulseIntensity), (Uint8)(b * pulseIntensity), 255);
    SDL_RenderFillRect(renderer, &(SDL_Rect){barX, barY, fillWidth, barHeight});

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void updateHealthSystem(t_healthSystem* health, float deltaTime) {
    if (!health) return;

    health->displayedHealthRatio += (health->targetHealthRatio - health->displayedHealthRatio) * HEALTH_ANIMATION_SPEED;
    if (fabs(health->displayedHealthRatio - health->targetHealthRatio) < HEALTH_ANIMATION_EPSILON) {
        health->displayedHealthRatio = health->targetHealthRatio;
    }

    if (health->isFlashing) {
        updateDeltaTimer(health->flashTimer, deltaTime);
        if (getDeltaTimer(health->flashTimer) >= health->flashDuration) {
            health->isFlashing = SDL_FALSE;
        }
    }

    if (health->isInvincible) {
        updateDeltaTimer(health->invincibilityTimer, deltaTime);
        if (getDeltaTimer(health->invincibilityTimer) >= health->invincibilityDuration) {
            health->isInvincible = SDL_FALSE;
        }
    }

    if (health->showHealthBar) {
        updateDeltaTimer(health->healthBarTimer, deltaTime);
        if (getDeltaTimer(health->healthBarTimer) >= health->healthBarDuration) {
            health->showHealthBar = SDL_FALSE;
        }
    }
}

void freeHealthSystem(t_healthSystem* health) {
    if (!health) return;

    free(health->healthBarTimer);
    free(health->flashTimer);
    free(health->invincibilityTimer);
}