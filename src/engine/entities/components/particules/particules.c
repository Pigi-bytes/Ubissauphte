#include "particules.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY_CONST 40.0f
#define AIR_RESISTANCE 0.97f

float randomRangeF(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

t_particleEmitter* createParticleEmitter() {
    t_particleEmitter* emitter = malloc(sizeof(t_particleEmitter));
    if (!emitter) return NULL;

    resetParticleEmitter(emitter);
    return emitter;
}

void resetParticleEmitter(t_particleEmitter* emitter) {
    if (!emitter) return;

    memset(emitter->particles, 0, sizeof(emitter->particles));
    emitter->count = 0;
    emitter->firstFree = 0;

    for (int i = 0; i < MAX_ENTITY_PARTICLES - 1; i++) {
        emitter->particles[i].nextFree = i + 1;
    }
    emitter->particles[MAX_ENTITY_PARTICLES - 1].nextFree = -1;
}

void freeParticleEmitter(t_particleEmitter* emitter) {
    free(emitter);
}

t_entityParticle* getNextFreeParticle(t_particleEmitter* emitter) {
    if (!emitter || emitter->count >= MAX_ENTITY_PARTICLES || emitter->firstFree < 0)
        return NULL;

    int idx = emitter->firstFree;
    t_entityParticle* p = &emitter->particles[idx];
    emitter->firstFree = p->nextFree;
    p->active = SDL_TRUE;
    emitter->count++;

    return p;
}

void emitParticles(t_particleEmitter* emitter, float x, float y, SDL_Color color, int count, float sizeMin, float sizeMax, float speedMin, float speedMax, float lifetimeMin, float lifetimeMax) {
    if (!emitter) return;

    for (int i = 0; i < count; i++) {
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) return;

        float angle = randomRangeF(0, 2 * M_PI);
        float speed = randomRangeF(speedMin, speedMax);

        p->position = (SDL_FPoint){x + randomRangeF(-4.0f, 4.0f), y + randomRangeF(-2.0f, 2.0f)};
        p->velocity = (SDL_FPoint){cosf(angle) * speed, sinf(angle) * speed};
        p->lifetime = p->maxLifetime = randomRangeF(lifetimeMin, lifetimeMax);
        p->size = randomRangeF(sizeMin, sizeMax);
        p->color = color;
    }
}

void emitMovementParticles(t_particleEmitter* emitter, float x, float y, SDL_Color color) {
    if (!emitter) return;

    SDL_Color dustColor = {207, 130, 84, 230};
    emitParticles(emitter, x, y, dustColor, 2 + rand() % 2, 3.0f, 5.0f, 12.0f, 20.0f, 0.4f, 0.8f);
}

void emitLandingParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    position.y += radius * 0.9f;

    for (int i = 0; i < 6 + rand() % 5; i++) {
        float angle = (float)i / (6 + rand() % 5) * 2.0f * M_PI;

        SDL_Color particleColor = baseColor;
        float colorVar = sinf(angle * 2.0f) * 15.0f;
        particleColor.r = fmin(255, particleColor.r + colorVar);
        particleColor.g = fmin(255, particleColor.g + colorVar * 0.5f);
        particleColor.b = fmin(255, particleColor.b - colorVar * 0.2f);
        particleColor.a = randomRangeF(140.0f, 190.0f);

        float particleX = position.x + cosf(angle) * randomRangeF(0.3f, 1.0f) * radius;
        float particleY = position.y + sinf(angle) * randomRangeF(0.3f, 1.0f) * radius * 0.3f;

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) return;

        p->position = (SDL_FPoint){particleX, particleY};
        p->velocity = (SDL_FPoint){cosf(angle) * randomRangeF(8.0f, 20.0f), sinf(angle) * randomRangeF(4.0f, 10.0f) - randomRangeF(3.0f, 10.0f)};
        p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.7f);
        p->size = randomRangeF(2.0f, 4.0f);
        p->color = particleColor;
    }

    SDL_Color dustColor = baseColor;
    dustColor.a = randomRangeF(120.0f, 160.0f);

    for (int i = 0; i < 2 + rand() % 3; i++) {
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) return;

        p->position = (SDL_FPoint){position.x + randomRangeF(-5.0f, 5.0f), position.y + randomRangeF(-2.0f, 2.0f)};
        p->velocity = (SDL_FPoint){randomRangeF(-8.0f, 8.0f), randomRangeF(-25.0f, -10.0f)};
        p->lifetime = p->maxLifetime = randomRangeF(0.4f, 0.6f);
        p->size = randomRangeF(1.0f, 2.5f);
        p->color = dustColor;
        p->color.r = fmin(255, p->color.r + randomRangeF(-10.0f, 20.0f));
        p->color.g = fmin(255, p->color.g + randomRangeF(-10.0f, 20.0f));
        p->color.b = fmin(255, p->color.b + randomRangeF(-10.0f, 10.0f));
    }
}

void emitImpactParticles(t_particleEmitter* emitter, SDL_FPoint position, SDL_FPoint hitDirection, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    // Position d'impact
    SDL_FPoint impactPos = {position.x + hitDirection.x * (radius * 0.8f), position.y + hitDirection.y * (radius * 0.8f)};

    float oppositeAngle = atan2f(hitDirection.y, hitDirection.x) + M_PI;

    SDL_Color brightColor = baseColor;
    brightColor.r = fmin(255, brightColor.r + 150);
    brightColor.g = fmin(255, brightColor.g + 150);
    brightColor.b = fmin(255, brightColor.b + 150);
    brightColor.a = 230;

    emitParticles(emitter, impactPos.x, impactPos.y, brightColor, 6 + rand() % 3, 1.0f, 3.5f, 50.0f, 100.0f, 0.1f, 0.25f);

    // Orienter les particules dans la direction opposée à l'impact
    float coneAngle = M_PI * 0.67f;  // Angle du cone d'éjection
    int startIdx = emitter->firstFree > 8 ? emitter->firstFree - 8 : 0;

    for (int i = startIdx; i < MAX_ENTITY_PARTICLES && i < startIdx + 8; i++) {
        t_entityParticle* p = &emitter->particles[i];
        if (!p->active || p->color.r != brightColor.r) continue;

        float angleOffset = randomRangeF(-coneAngle / 2, coneAngle / 2);
        float angle = oppositeAngle + angleOffset;
        float speed = randomRangeF(50.0f, 100.0f);

        p->velocity = (SDL_FPoint){cosf(angle) * speed, sinf(angle) * speed};
    }

    // Gouttes plus grosses
    SDL_Color dropColor = baseColor;
    dropColor.a = randomRangeF(180.0f, 230.0f);

    emitParticles(emitter, impactPos.x, impactPos.y, dropColor, 4 + rand() % 3, 2.5f, 6.0f, 30.0f, 60.0f, 0.2f, 0.4f);
    startIdx = emitter->firstFree > 6 ? emitter->firstFree - 6 : 0;

    for (int i = startIdx; i < MAX_ENTITY_PARTICLES && i < startIdx + 6; i++) {
        t_entityParticle* p = &emitter->particles[i];
        if (!p->active || p->color.r != dropColor.r) continue;

        float angleOffset = randomRangeF(-coneAngle * 0.7f, coneAngle * 0.7f);
        float angle = oppositeAngle + angleOffset;
        float centerFactor = 1.0f - (fabsf(angleOffset) / (coneAngle * 0.7f)) * 0.7f;
        float speed = randomRangeF(30.0f, 60.0f) * centerFactor;

        p->velocity = (SDL_FPoint){cosf(angle) * speed, sinf(angle) * speed};

        p->color.r = fmin(255, fmax(0, p->color.r + randomRangeF(-30.0f, 30.0f) * 0.5f));
        p->color.g = fmin(255, fmax(0, p->color.g + randomRangeF(-30.0f, 30.0f) * 0.3f));
        p->color.b = fmin(255, fmax(0, p->color.b + randomRangeF(-30.0f, 30.0f) * 0.2f));
    }
}

void emitDeathParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    for (int i = 0; i < 12; i++) {
        float angle = (float)i / 12.0f * M_PI * 2.0f;
        float particleX = position.x + cosf(angle) * radius * randomRangeF(0.5f, 0.85f);
        float particleY = position.y + sinf(angle) * radius * randomRangeF(0.5f, 0.85f);

        SDL_Color particleColor = baseColor;
        particleColor.a = 140 + rand() % 60;

        emitParticles(emitter, particleX, particleY, particleColor, 1, 2.0f, 5.0f, 10.0f, 30.0f, 0.5f, 1.0f);
        t_entityParticle* p = &emitter->particles[emitter->firstFree > 0 ? emitter->firstFree - 1 : 0];
        if (p->active) {
            p->velocity.x = cosf(angle) * randomRangeF(10.0f, 30.0f);
            p->velocity.y = sinf(angle) * randomRangeF(3.0f, 9.0f) - randomRangeF(0.0f, 5.0f);
        }
    }

    SDL_Color coreColor = baseColor;
    coreColor.a = 100 + rand() % 50;
    emitParticles(emitter, position.x, position.y, coreColor, 5, 5.0f, 8.0f, 2.0f, 5.0f, 0.8f, 1.5f);
}

void updateEntityParticles(t_particleEmitter* emitter, float deltaTime) {
    if (!emitter) return;

    for (int i = 0; i < MAX_ENTITY_PARTICLES; i++) {
        t_entityParticle* p = &emitter->particles[i];
        if (!p->active) continue;

        // Vérifier si la particule est expirée
        if ((p->lifetime -= deltaTime) <= 0) {
            p->active = SDL_FALSE;
            p->nextFree = emitter->firstFree;
            emitter->firstFree = i;
            emitter->count--;
            continue;
        }

        p->position.x += p->velocity.x * deltaTime;
        p->position.y += p->velocity.y * deltaTime;

        p->velocity.y += GRAVITY_CONST * deltaTime;
        p->velocity.x *= AIR_RESISTANCE;
    }
}

void renderEntityParticles(SDL_Renderer* renderer, t_particleEmitter* emitter) {
    if (!emitter || !renderer) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < MAX_ENTITY_PARTICLES; i++) {
        t_entityParticle* p = &emitter->particles[i];
        if (!p->active) continue;

        float lifetimeRatio = p->lifetime / p->maxLifetime;
        float alpha = lifetimeRatio * 0.7f + 0.3f;  // Fade-out progressif
        float particleSize = p->size * (0.7f + lifetimeRatio * 0.3f);

        int x = p->position.x - particleSize / 2;
        int y = p->position.y - particleSize / 2;
        int size = particleSize;

        SDL_SetRenderDrawColor(renderer, p->color.r, p->color.g, p->color.b, (Uint8)(p->color.a * alpha));

        SDL_Rect rect = {x, y, size, size};
        SDL_RenderFillRect(renderer, &rect);

        // Contour plus foncé
        SDL_SetRenderDrawColor(renderer, p->color.r * 0.7f, p->color.g * 0.7f, p->color.b * 0.7f, (Uint8)(p->color.a * alpha));
        SDL_RenderDrawRect(renderer, &rect);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}