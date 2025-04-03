#ifndef ENTITY_PARTICLES_H
#define ENTITY_PARTICLES_H

#include <SDL2/SDL.h>

#include "../core.h"

#define MAX_ENTITY_PARTICLES 100  // Particules maximales par entité

typedef struct s_entityParticle {
    SDL_bool active;
    SDL_FPoint position;  // Position en x,y avec précision flottante
    SDL_FPoint velocity;  // Vitesse en x,y avec précision flottante
    float lifetime, maxLifetime;
    float size;
    SDL_Color color;
    int nextFree;
    int shape;  // 0 = carré, 1 = rond
} t_entityParticle;

typedef struct s_particleEmitter {
    t_entityParticle particles[MAX_ENTITY_PARTICLES];
    int count;
    int firstFree;
} t_particleEmitter;

t_particleEmitter* createParticleEmitter();
void resetParticleEmitter(t_particleEmitter* emitter);
void freeParticleEmitter(t_particleEmitter* emitter);
t_entityParticle* getNextFreeParticle(t_particleEmitter* emitter);

void emitParticles(t_particleEmitter* emitter, float x, float y, SDL_Color color, int count, float sizeMin, float sizeMax, float speedMin, float speedMax, float lifetimeMin, float lifetimeMax);
void emitMovementParticles(t_particleEmitter* emitter, float x, float y, SDL_Color color);
void emitBossMovementParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);

void updateEntityParticles(t_particleEmitter* emitter, float deltaTime);
void renderEntityParticles(SDL_Renderer* renderer, t_particleEmitter* emitter);

void emitLandingParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);
void emitImpactParticles(t_particleEmitter* emitter, SDL_FPoint position, SDL_FPoint hitDirection, float radius, SDL_Color baseColor);
void emitDeathParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);

void emitBarrelHitParticles(t_particleEmitter* emitter, SDL_FPoint position, SDL_FPoint hitDirection, float radius);
void emitBarrelExplosionParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius);

float randomRangeF(float min, float max);

#endif