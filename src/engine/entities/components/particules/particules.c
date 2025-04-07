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

void emitPhaseChangeParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    // Explosion principale - Particules qui partent du centre
    for (int i = 0; i < 25 + rand() % 10; i++) {
        float angle = randomRangeF(0, 2.0f * M_PI);
        float speed = randomRangeF(50.0f, 120.0f);

        SDL_Color particleColor = baseColor;
        particleColor.a = randomRangeF(200.0f, 255.0f);

        // Variation de couleur pour effet dynamique
        float intensity = randomRangeF(0.7f, 1.3f);
        particleColor.r = fmin(255, particleColor.r * intensity);
        particleColor.g = fmin(255, particleColor.g * intensity);
        particleColor.b = fmin(255, particleColor.b * intensity);

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        p->position = (SDL_FPoint){
            position.x + cosf(angle) * radius * 0.3f,
            position.y + sinf(angle) * radius * 0.3f};

        p->velocity = (SDL_FPoint){
            cosf(angle) * speed,
            sinf(angle) * speed};

        p->lifetime = p->maxLifetime = randomRangeF(0.8f, 1.5f);
        p->size = randomRangeF(6.0f, 14.0f);
        p->color = particleColor;
        p->shape = rand() % 4 == 0 ? 0 : 1;  // Mélange de carrés et ronds avec prédominance ronde
    }

    // Effet d'onde de choc (cercles concentriques)
    for (int i = 0; i < 3; i++) {
        SDL_Color waveColor = baseColor;
        waveColor.a = 180;

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        p->position = position;
        p->velocity = (SDL_FPoint){0, 0};  // Reste fixe
        p->lifetime = p->maxLifetime = randomRangeF(0.6f, 1.0f);
        p->size = radius * (0.4f + i * 0.3f);  // Différentes tailles pour créer des cercles concentriques
        p->color = waveColor;
        p->shape = 1;  // Cercle
    }

    // Petits éclats pixelisés (effet rétro)
    for (int i = 0; i < 15 + rand() % 10; i++) {
        float angle = randomRangeF(0, 2.0f * M_PI);
        float distance = radius * randomRangeF(0.1f, 0.4f);
        float speed = randomRangeF(80.0f, 150.0f);

        SDL_Color sparkColor = {
            fmin(255, baseColor.r + randomRangeF(30.0f, 80.0f)),
            fmin(255, baseColor.g + randomRangeF(30.0f, 80.0f)),
            fmin(255, baseColor.b + randomRangeF(30.0f, 80.0f)),
            randomRangeF(180.0f, 230.0f)};

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        p->position = (SDL_FPoint){
            position.x + cosf(angle) * distance,
            position.y + sinf(angle) * distance};

        p->velocity = (SDL_FPoint){
            cosf(angle) * speed,
            sinf(angle) * speed};

        p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.5f);
        p->size = randomRangeF(2.0f, 4.0f);
        p->color = sparkColor;
        p->shape = 0;  // Carré pour effet rétro pixelisé
    }

    // Effet de "fumée" ou d'énergie qui monte (pour l'effet dramatique)
    for (int i = 0; i < 10; i++) {
        SDL_Color smokeColor = baseColor;
        smokeColor.r = fmin(255, smokeColor.r * 0.7f);
        smokeColor.g = fmin(255, smokeColor.g * 0.7f);
        smokeColor.b = fmin(255, smokeColor.b * 0.7f);
        smokeColor.a = randomRangeF(100.0f, 160.0f);

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        float angle = randomRangeF(0, 2.0f * M_PI);
        float distance = radius * randomRangeF(0.5f, 0.9f);

        p->position = (SDL_FPoint){
            position.x + cosf(angle) * distance,
            position.y + sinf(angle) * distance};

        p->velocity = (SDL_FPoint){
            cosf(angle) * randomRangeF(5.0f, 15.0f),
            sinf(angle) * randomRangeF(5.0f, 15.0f) - randomRangeF(20.0f, 40.0f)  // Tendance à monter
        };

        p->lifetime = p->maxLifetime = randomRangeF(0.8f, 1.2f);
        p->size = randomRangeF(8.0f, 16.0f);
        p->color = smokeColor;
        p->shape = 1;  // Rond pour effet nuageux
    }
}

void emitChargeAttackParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor, float phaseProgress, SDL_FPoint chargeDirection) {
    if (!emitter) return;

    // Définition des couleurs constantes - facilement modifiables
    const SDL_Color CHARGE_PREPARE_COLOR = {0, 238, 194, 100};    // Orange vif pour préparation
    const SDL_Color CHARGE_GLOW_COLOR = {255, 69, 0, 150};        // Rouge-orangé pour l'aura
    const SDL_Color CHARGE_TRAIL_COLOR = {30, 144, 255, 170};     // Bleu vif pour la traînée
    const SDL_Color CHARGE_SPARK_COLOR = {255, 215, 0, 220};      // Or pour les étincelles
    const SDL_Color CHARGE_EXHAUST_COLOR = {176, 196, 222, 160};  // Bleu clair pour l'épuisement

    // Limiter le nombre de particules pour éviter la surcharge
    int particleLimit = 20;

    // Phase de préparation (0.0 - 0.8)
    if (phaseProgress < 0.8f) {
        // Phase d'accumulation d'énergie
        float prepProgress = phaseProgress / 0.8f;  // 0 à 1 pour cette phase

        // Pulsation d'énergie autour du boss
        if ((int)(phaseProgress * 20) % 3 == 0) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (p) {
                float pulseSize = 0.8f + 0.3f * sinf(phaseProgress * 15.0f);

                p->position = position;
                p->velocity = (SDL_FPoint){0, 0};
                p->lifetime = p->maxLifetime = 0.12f;
                p->size = radius * pulseSize;
                p->color = CHARGE_GLOW_COLOR;
                p->shape = 1;  // Cercle
            }
        }

        // Particules qui convergent vers le boss depuis la direction cible
        if ((int)(phaseProgress * 15) % 2 == 0) {
            for (int i = 0; i < 3 && i < particleLimit; i++) {
                t_entityParticle* p = getNextFreeParticle(emitter);
                if (!p) continue;

                float distanceAhead = radius * (2.0f + prepProgress * 4.0f);  // Distance qui augmente avec le temps
                float spread = radius * 1.5f * (1.0f - prepProgress * 0.5f);  // Éventail qui se resserre

                // Position devant le boss dans la direction de la charge
                p->position = (SDL_FPoint){
                    position.x + chargeDirection.x * distanceAhead + randomRangeF(-spread, spread),
                    position.y + chargeDirection.y * distanceAhead + randomRangeF(-spread, spread)};

                // Vitesse qui converge vers le boss
                p->velocity = (SDL_FPoint){
                    (position.x - p->position.x) * randomRangeF(4.0f, 8.0f) * prepProgress,
                    (position.y - p->position.y) * randomRangeF(4.0f, 8.0f) * prepProgress};

                p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.4f);
                p->size = randomRangeF(2.0f, 4.0f);
                p->color = CHARGE_PREPARE_COLOR;
                p->shape = rand() % 2;  // Mélange de carrés et cercles
            }
        }

        // Étincelles d'énergie autour du boss à mesure que la charge se prépare
        if (prepProgress > 0.5f && (int)(phaseProgress * 30) % 2 == 0) {
            for (int i = 0; i < 2 && i < particleLimit; i++) {
                t_entityParticle* p = getNextFreeParticle(emitter);
                if (!p) continue;

                float angle = randomRangeF(0, 2.0f * M_PI);
                float dist = radius * randomRangeF(0.8f, 1.1f);

                p->position = (SDL_FPoint){
                    position.x + cosf(angle) * dist,
                    position.y + sinf(angle) * dist};

                // Étincelles qui s'éloignent légèrement
                p->velocity = (SDL_FPoint){
                    cosf(angle) * randomRangeF(10.0f, 40.0f) * prepProgress,
                    sinf(angle) * randomRangeF(10.0f, 40.0f) * prepProgress};

                p->lifetime = p->maxLifetime = randomRangeF(0.05f, 0.2f);
                p->size = randomRangeF(1.0f, 3.0f);
                p->color = CHARGE_SPARK_COLOR;
                p->shape = 0;  // Carré pour effet pixelisé
            }
        }
    }
    // Phase d'initiation du dash (0.8 - 1.0)
    else if (phaseProgress < 1.0f) {
        float initiateProgress = (phaseProgress - 0.8f) / 0.2f;  // 0 à 1 pour cette phase

        // Flash lumineux au début de la charge
        if (initiateProgress < 0.3f) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (p) {
                p->position = position;
                p->velocity = (SDL_FPoint){0, 0};
                p->lifetime = p->maxLifetime = 0.15f;
                p->size = radius * (1.5f + initiateProgress * 1.0f);

                SDL_Color flashColor = CHARGE_PREPARE_COLOR;
                flashColor.a = (Uint8)(220 * (1.0f - initiateProgress * 3.0f));
                p->color = flashColor;
                p->shape = 1;  // Cercle
            }
        }

        // Particules d'énergie projetées dans la direction du dash
        for (int i = 0; i < 5 && i < particleLimit; i++) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (!p) continue;

            float angleVar = randomRangeF(-0.5f, 0.5f);  // Légère variation d'angle

            // Calcul de la direction avec variation
            float dirX = chargeDirection.x * cosf(angleVar) - chargeDirection.y * sinf(angleVar);
            float dirY = chargeDirection.x * sinf(angleVar) + chargeDirection.y * cosf(angleVar);

            // Position autour du boss
            p->position = (SDL_FPoint){
                position.x + randomRangeF(-radius * 0.5f, radius * 0.5f),
                position.y + randomRangeF(-radius * 0.5f, radius * 0.5f)};

            // Vitesse vers l'arrière pour effet de propulsion
            p->velocity = (SDL_FPoint){
                -dirX * randomRangeF(60.0f, 100.0f) * initiateProgress,
                -dirY * randomRangeF(60.0f, 100.0f) * initiateProgress};

            p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.2f);
            p->size = randomRangeF(3.0f, 6.0f);

            // Variation de couleur entre préparation et impact
            if (rand() % 3 == 0) {
                p->color = CHARGE_SPARK_COLOR;
            } else {
                p->color = CHARGE_PREPARE_COLOR;
            }

            p->shape = rand() % 2;
        }
    }
    // Phase de dash actif (1.0 - 1.5)
    else if (phaseProgress < 1.5f) {
        float dashProgress = (phaseProgress - 1.0f) / 0.5f;  // 0 à 1 pour cette phase

        // Traînée derrière le boss pendant le dash
        for (int i = 0; i < 4 && i < particleLimit; i++) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (!p) continue;

            float trailDistance = randomRangeF(0.1f, 1.0f) * radius * 2.0f;
            float spreadFactor = trailDistance * 0.2f;

            // Position à l'arrière du boss
            p->position = (SDL_FPoint){
                position.x - chargeDirection.x * trailDistance + randomRangeF(-spreadFactor, spreadFactor),
                position.y - chargeDirection.y * trailDistance + randomRangeF(-spreadFactor, spreadFactor)};

            // Particules quasi-statiques pour la traînée
            p->velocity = (SDL_FPoint){
                randomRangeF(-10.0f, 10.0f),
                randomRangeF(-10.0f, 10.0f)};

            p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.25f);
            p->size = randomRangeF(4.0f, 8.0f) * (1.0f - trailDistance / (radius * 2.0f));

            // Mélange de couleurs pour la traînée
            if (rand() % 3 == 0) {
                p->color = CHARGE_TRAIL_COLOR;
            } else {
                p->color = CHARGE_PREPARE_COLOR;
                p->color.a = 150;
            }

            p->shape = 1;  // Cercle pour effet fluide
        }

        // Petites particules d'étincelles sur les côtés pendant le dash
        if ((int)(phaseProgress * 20) % 2 == 0) {
            float sideAngle = atan2f(chargeDirection.y, chargeDirection.x) + (M_PI * 2);  // 90 degrés par rapport à la direction

            for (int side = -1; side <= 1; side += 2) {  // -1 puis +1 pour les deux côtés
                t_entityParticle* p = getNextFreeParticle(emitter);
                if (!p) continue;

                float currentSideAngle = sideAngle + side * randomRangeF(-0.2f, 0.2f);

                p->position = (SDL_FPoint){
                    position.x + cosf(currentSideAngle) * radius * randomRangeF(0.6f, 1.0f),
                    position.y + sinf(currentSideAngle) * radius * randomRangeF(0.6f, 1.0f)};

                // Projection vers l'extérieur et légèrement en arrière
                p->velocity = (SDL_FPoint){
                    cosf(currentSideAngle) * randomRangeF(20.0f, 40.0f) - chargeDirection.x * randomRangeF(10.0f, 20.0f),
                    sinf(currentSideAngle) * randomRangeF(20.0f, 40.0f) - chargeDirection.y * randomRangeF(10.0f, 20.0f)};

                p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.2f);
                p->size = randomRangeF(2.0f, 4.0f);
                p->color = CHARGE_SPARK_COLOR;
                p->shape = rand() % 2;
            }
        }
    }
    // Phase de récupération (1.5 - 2.0)
    else if (phaseProgress < 2.0f) {
        float recoveryProgress = (phaseProgress - 1.5f) / 0.5f;  // 0 à 1 pour cette phase

        // Particules d'épuisement - "fumée" qui monte
        if ((int)(phaseProgress * 12) % 2 == 0) {
            for (int i = 0; i < 3 && i < particleLimit; i++) {
                t_entityParticle* p = getNextFreeParticle(emitter);
                if (!p) continue;

                float angle = randomRangeF(0, 2.0f * M_PI);
                float dist = radius * randomRangeF(0.5f, 0.9f);

                p->position = (SDL_FPoint){
                    position.x + cosf(angle) * dist,
                    position.y + sinf(angle) * dist};

                // Vitesse principalement vers le haut
                p->velocity = (SDL_FPoint){
                    randomRangeF(-5.0f, 5.0f),
                    randomRangeF(-25.0f, -15.0f)};

                p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.6f);
                p->size = randomRangeF(3.0f, 7.0f);
                p->color = CHARGE_EXHAUST_COLOR;
                p->shape = 1;  // Rond pour effet nuageux/fumée
            }
        }

        // Petites étoiles d'épuisement
        if (recoveryProgress > 0.3f && (int)(phaseProgress * 8) % 2 == 0) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (p) {
                float angle = randomRangeF(0, 2.0f * M_PI);
                float dist = radius * 0.7f;

                p->position = (SDL_FPoint){
                    position.x + cosf(angle) * dist,
                    position.y + sinf(angle) * dist};

                p->velocity = (SDL_FPoint){
                    randomRangeF(-2.0f, 2.0f),
                    randomRangeF(-10.0f, -2.0f)};

                p->lifetime = p->maxLifetime = randomRangeF(0.4f, 0.7f);
                p->size = randomRangeF(1.0f, 3.0f);
                p->color = CHARGE_EXHAUST_COLOR;
                p->color.a = 120;
                p->shape = 0;  // Carré pour effet pixelisé
            }
        }
    }
}

void emitGroundPoundParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor, float phaseProgress) {
    if (!emitter) return;

    // Définition des couleurs constantes - facilement modifiables
    const SDL_Color PULSE_CIRCLE_COLOR = {0, 238, 194, 100};   // Cercle de pulsation
    const SDL_Color PREP_DROPS_COLOR = {255, 51, 54, 200};     // Gouttes de préparation
    const SDL_Color CONVERGE_COLOR = {255, 51, 54, 200};       // Particules qui convergent
    const SDL_Color WAVE_COLOR = {173, 255, 239, 200};         // Onde de choc
    const SDL_Color DEBRIS_COLOR = {0, 238, 194, 180};         // Débris suivant l'onde
    const SDL_Color CENTER_DEBRIS_COLOR = {0, 238, 194, 200};  // Débris du centre

    // Limiter le nombre de particules pour éviter la surcharge
    int particleLimit = 20;

    // phaseProgress détermine la phase de l'attaque
    if (phaseProgress < 0.8f) {
        // Phase de préparation

        // Créer seulement quelques particules pour la pulsation
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            float pulseSize = 0.5f + 0.5f * sinf(phaseProgress * 10.0f);

            p->position = position;
            p->velocity = (SDL_FPoint){0, 0};
            p->lifetime = p->maxLifetime = 0.1f;
            p->size = radius * 1.5f * pulseSize;
            p->color = PULSE_CIRCLE_COLOR;
            p->shape = 1;  // Cercle
        }

        // Quelques gouttelettes qui tombent
        for (int i = 0; i < 3 && i < particleLimit; i++) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (!p) continue;

            float angle = randomRangeF(0, 2.0f * M_PI);
            float dist = radius * 0.8f;

            p->position = (SDL_FPoint){
                position.x + cosf(angle) * dist,
                position.y + sinf(angle) * dist};

            p->velocity = (SDL_FPoint){
                randomRangeF(-10.0f, 10.0f),
                randomRangeF(15.0f, 25.0f)};

            p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.4f);
            p->size = randomRangeF(3.0f, 5.0f);
            p->color = PREP_DROPS_COLOR;
            p->shape = rand() % 2;
        }
    } else if (phaseProgress < 1.0f) {
        // Phase de compression
        for (int i = 0; i < 5 && i < particleLimit; i++) {
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (!p) continue;

            float angle = randomRangeF(0, 2.0f * M_PI);
            float dist = radius * randomRangeF(0.5f, 1.2f);

            p->position = (SDL_FPoint){
                position.x + cosf(angle) * dist,
                position.y + sinf(angle) * dist};

            // Converger vers le centre
            p->velocity = (SDL_FPoint){
                -cosf(angle) * randomRangeF(30.0f, 50.0f),
                -sinf(angle) * randomRangeF(30.0f, 50.0f)};

            p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.2f);
            p->size = randomRangeF(2.0f, 4.0f);

            p->color = CONVERGE_COLOR;
            p->shape = rand() % 2;
        }
    } else {
        // Phase d'explosion - après 1.0
        float timeInPhase = phaseProgress - 1.0f;

        if (timeInPhase < 0.7f) {
            // Onde de choc qui s'étend
            float waveRadius = radius * 5.0f * (timeInPhase / 0.7f);

            // Un seul cercle d'onde de choc pour éviter la surcharge
            t_entityParticle* p = getNextFreeParticle(emitter);
            if (p) {
                p->position = position;
                p->velocity = (SDL_FPoint){0, 0};
                p->lifetime = p->maxLifetime = 0.15f;
                p->size = waveRadius;

                // Ajustement de l'alpha en fonction du temps
                SDL_Color adjustedWaveColor = WAVE_COLOR;
                adjustedWaveColor.a = (Uint8)(WAVE_COLOR.a - timeInPhase * 100);
                p->color = adjustedWaveColor;

                p->shape = 1;  // Cercle
            }

            // Quelques débris qui suivent l'onde
            for (int i = 0; i < 3 && i < particleLimit; i++) {
                t_entityParticle* p = getNextFreeParticle(emitter);
                if (!p) continue;

                float angle = randomRangeF(0, 2.0f * M_PI);

                p->position = (SDL_FPoint){
                    position.x + cosf(angle) * waveRadius * randomRangeF(0.9f, 1.0f),
                    position.y + sinf(angle) * waveRadius * randomRangeF(0.9f, 1.0f)};

                float speed = waveRadius / (0.15f * randomRangeF(0.9f, 1.1f));
                p->velocity = (SDL_FPoint){
                    cosf(angle) * speed,
                    sinf(angle) * speed};

                p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.2f);
                p->size = randomRangeF(3.0f, 5.0f);

                p->color = DEBRIS_COLOR;
                p->shape = rand() % 2;
            }

            // Quelques particules de débris à proximité du centre
            if ((int)(timeInPhase * 15) % 3 == 0 && timeInPhase < 0.3f) {
                for (int i = 0; i < 4 && i < particleLimit; i++) {
                    t_entityParticle* p = getNextFreeParticle(emitter);
                    if (!p) continue;

                    float angle = randomRangeF(0, 2.0f * M_PI);
                    float dist = radius * randomRangeF(0.2f, 0.8f);

                    p->position = (SDL_FPoint){
                        position.x + cosf(angle) * dist,
                        position.y + sinf(angle) * dist};

                    p->velocity = (SDL_FPoint){
                        cosf(angle) * randomRangeF(20.0f, 60.0f),
                        sinf(angle) * randomRangeF(20.0f, 60.0f) - randomRangeF(0.0f, 30.0f)};

                    p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.5f);
                    p->size = randomRangeF(2.0f, 4.0f);

                    p->color = CENTER_DEBRIS_COLOR;
                    p->shape = rand() % 2;
                }
            }
        }
    }
}

void emitBossMovementParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    // Particules de "bave" sous le slime (effet glissant)
    for (int i = 0; i < 5 + rand() % 3; i++) {
        float angle = randomRangeF(0, 2.0f * M_PI);  // Position aléatoire sous le slime
        float distance = randomRangeF(0.3f, 0.8f) * radius;

        SDL_Color slimeColor = baseColor;
        slimeColor.a = randomRangeF(160.0f, 200.0f);

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        p->position = (SDL_FPoint){
            position.x + cosf(angle) * distance,
            position.y + sinf(angle) * distance + radius * 0.1f  // Légèrement en dessous
        };

        // Particule qui reste en place (vitesse faible)
        p->velocity = (SDL_FPoint){
            randomRangeF(-3.0f, 3.0f),
            randomRangeF(-1.0f, 1.0f)};

        p->lifetime = p->maxLifetime = randomRangeF(0.8f, 1.5f);
        p->size = randomRangeF(4.0f, 10.0f);
        p->color = slimeColor;
        p->shape = 1;  // Particules rondes
    }

    // Éclaboussures rétro pixelisées (quand le slime se déplace)
    float movementDirection = randomRangeF(0, 2.0f * M_PI);  // Direction du mouvement
    for (int i = 0; i < 3 + rand() % 2; i++) {
        float offsetAngle = movementDirection + randomRangeF(-0.8f, 0.8f);
        float speed = randomRangeF(15.0f, 35.0f);

        SDL_Color splashColor = baseColor;
        splashColor.r = fmin(255, splashColor.r + randomRangeF(-20.0f, 20.0f));
        splashColor.g = fmin(255, splashColor.g + randomRangeF(-20.0f, 20.0f));
        splashColor.b = fmin(255, splashColor.b + randomRangeF(-20.0f, 20.0f));
        splashColor.a = randomRangeF(180.0f, 220.0f);

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        // Position sur le bord du slime
        p->position = (SDL_FPoint){
            position.x + cosf(offsetAngle + M_PI) * radius * 0.8f,
            position.y + sinf(offsetAngle + M_PI) * radius * 0.8f};

        // Éjection dans la direction opposée au mouvement
        p->velocity = (SDL_FPoint){
            cosf(offsetAngle) * speed,
            sinf(offsetAngle) * speed};

        p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.7f);
        p->size = randomRangeF(3.0f, 6.0f);
        p->color = splashColor;
        p->shape = randomRangeF(0, 1) > 0.5f ? 1 : 0;  // Mélange de formes carrées et rondes pour effet rétro
    }

    // Petits "yeux" ou bulles qui apparaissent dans le slime (effet visqueux)
    if (rand() % 6 == 0) {  // Occasionnellement
        SDL_Color bubbleColor = {
            fmin(255, baseColor.r + 70),
            fmin(255, baseColor.g + 70),
            fmin(255, baseColor.b + 70),
            220};

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            float innerAngle = randomRangeF(0, 2.0f * M_PI);
            float innerRadius = radius * randomRangeF(0.2f, 0.6f);

            p->position = (SDL_FPoint){
                position.x + cosf(innerAngle) * innerRadius,
                position.y + sinf(innerAngle) * innerRadius};

            p->velocity = (SDL_FPoint){
                cosf(innerAngle) * randomRangeF(1.0f, 5.0f),
                sinf(innerAngle) * randomRangeF(1.0f, 5.0f)};

            p->lifetime = p->maxLifetime = randomRangeF(0.4f, 0.8f);
            p->size = randomRangeF(2.0f, 4.0f);
            p->color = bubbleColor;
            p->shape = 1;  // Bulle ronde
        }
    }
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
        p->shape = 0;

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
        p->shape = 0;
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
        p->shape = 0;
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
        p->shape = 0;
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

void emitBarrelHitParticles(t_particleEmitter* emitter, SDL_FPoint position, SDL_FPoint hitDirection, float radius) {
    if (!emitter) return;

    SDL_FPoint impactPos = {position.x + hitDirection.x * radius * 0.8f, position.y + hitDirection.y * radius * 0.8f};
    float oppositeAngle = atan2f(hitDirection.y, hitDirection.x) + M_PI;
    float coneAngle = M_PI * 0.65f;

    SDL_Color woodColor = {139, 69, 19, 220};        // Brun foncé
    SDL_Color splinterColor = {205, 170, 125, 180};  // Bois clair
    SDL_Color dustColor = {210, 180, 140, 150};      // Poussière

    emitParticles(emitter, impactPos.x, impactPos.y, woodColor, 5, 1.5f, 3.5f, 50.0f, 90.0f, 0.1f, 0.25f);
    emitParticles(emitter, impactPos.x, impactPos.y, splinterColor, 3, 1.0f, 2.5f, 60.0f, 120.0f, 0.2f, 0.4f);
    emitParticles(emitter, impactPos.x, impactPos.y, dustColor, 4, 2.0f, 4.0f, 30.0f, 60.0f, 0.3f, 0.6f);

    int startIdx = emitter->firstFree > 12 ? emitter->firstFree - 12 : 0;
    for (int i = startIdx; i < MAX_ENTITY_PARTICLES && i < startIdx + 12; i++) {
        t_entityParticle* p = &emitter->particles[i];
        if (!p->active) continue;

        float angleOffset = randomRangeF(-coneAngle / 2, coneAngle / 2);
        float angle = oppositeAngle + angleOffset;

        float centerFactor = 1.0f - fabsf(angleOffset) / (coneAngle * 0.5f) * 0.7f;
        float speed = randomRangeF(30.0f, 90.0f) * centerFactor;

        p->velocity.x = cosf(angle) * speed;
        p->velocity.y = sinf(angle) * speed;

        float colorVar = randomRangeF(-15.0f, 15.0f);
        p->color.r = fmin(255, p->color.r + colorVar);
        p->color.g = fmin(255, p->color.g + colorVar);
        p->color.b = fmin(255, p->color.b + colorVar);
    }
}
void emitBarrelExplosionParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius) {
    if (!emitter) return;

    // Couleurs du bois
    SDL_Color woodColor1 = {139, 69, 19, 255};   // Brun foncé
    SDL_Color woodColor2 = {160, 82, 45, 255};   // Brun moyen
    SDL_Color woodColor3 = {181, 101, 29, 220};  // Brun clair
    SDL_Color dustColor = {210, 180, 140, 150};  // Poussière

    for (int i = 0; i < 6; i++) {
        float angle = (float)i / 6.0f * M_PI * 2.0f;
        float distance = randomRangeF(radius * 1.0f, radius * 2.5f);

        // Couleur selon l'indice
        SDL_Color color = (i % 3 == 0) ? woodColor1 : (i % 3 == 1) ? woodColor2
                                                                   : woodColor3;
        color.a = 140 + rand() % 60;

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            p->position.x = position.x + cosf(angle) * distance;
            p->position.y = position.y + sinf(angle) * distance;
            p->velocity = (SDL_FPoint){0, 0};
            p->lifetime = p->maxLifetime = randomRangeF(15.0f, 30.0f);
            p->size = randomRangeF(3.5f, 6.5f);
            p->color = color;
            p->shape = 0;
        }
    }

    for (int i = 0; i < 8; i++) {
        float angle = (float)i / 8.0f * M_PI * 2.0f;
        float particleDistance = randomRangeF(radius * 0.3f, radius * 0.8f);
        SDL_Color color = (i % 3 == 0) ? woodColor1 : (i % 3 == 1) ? woodColor2
                                                                   : woodColor3;
        color.a = 180 + rand() % 75;

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            p->shape = 0;
            p->position.x = position.x + cosf(angle) * particleDistance;
            p->position.y = position.y + sinf(angle) * particleDistance;
            p->velocity = (SDL_FPoint){0, 0};
            p->lifetime = p->maxLifetime = randomRangeF(0.5f, 1.0f);
            p->size = randomRangeF(2.0f, 5.0f);
            p->color = color;
        }
    }

    for (int i = 0; i < 6; i++) {
        float angle = (float)i / 6.0f * M_PI * 2.0f;
        float dustDistance = randomRangeF(radius * 0.4f, radius * 1.0f);

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            p->position.x = position.x + cosf(angle) * dustDistance;
            p->position.y = position.y + sinf(angle) * dustDistance;
            p->velocity = (SDL_FPoint){0, 0};
            p->lifetime = p->maxLifetime = randomRangeF(0.7f, 1.2f);
            p->size = randomRangeF(3.0f, 6.0f);
            p->color = dustColor;
            p->shape = 0;
        }
    }

    for (int i = 0; i < 3; i++) {
        float angle = randomRangeF(0, 2.0f * M_PI);
        float chunkDistance = randomRangeF(radius * 0.5f, radius * 1.5f);
        SDL_Color chunkColor = woodColor1;
        chunkColor.a = 200 + rand() % 55;

        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            p->position.x = position.x + cosf(angle) * chunkDistance;
            p->position.y = position.y + sinf(angle) * chunkDistance;
            p->velocity = (SDL_FPoint){0, 0};
            p->lifetime = p->maxLifetime = randomRangeF(10.0f, 20.0f);
            p->size = randomRangeF(6.0f, 9.0f);
            p->color = chunkColor;
            p->shape = 0;
        }
    }
}

void emitSpellCastParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    SDL_FPoint hands[2] = {{position.x - radius * 0.6f, position.y - radius * 0.2f}, {position.x + radius * 0.6f, position.y - radius * 0.2f}};

    for (int hand = 0; hand < 2; hand++) {
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (p) {
            p->position = hands[hand];
            p->velocity = (SDL_FPoint){0, 0};
            p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.5f);
            p->size = radius * 0.4f;
            p->color = (SDL_Color){baseColor.r, baseColor.g, baseColor.b, 100};
            p->shape = 1;  // Circle
        }

        // Orbiting particles
        for (int i = 0; i < 6; i++) {
            float angle = (float)i / 6.0f * 2.0f * M_PI;
            p = getNextFreeParticle(emitter);
            if (!p) continue;

            p->position = (SDL_FPoint){hands[hand].x + cosf(angle) * (radius * 0.3f), hands[hand].y + sinf(angle) * (radius * 0.3f)};

            p->velocity = (SDL_FPoint){cosf(angle + M_PI / 2) * 25.0f, sinf(angle + M_PI / 2) * 25.0f};
            p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.4f);
            p->size = randomRangeF(2.0f, 4.0f);
            p->color = (SDL_Color){fmin(255, baseColor.r + randomRangeF(-20.0f, 20.0f)), fmin(255, baseColor.g + randomRangeF(-20.0f, 20.0f)), fmin(255, baseColor.b + randomRangeF(-20.0f, 20.0f)), 200};
            p->shape = randomRangeF(0.0f, 1.0f) > 0.3f ? 1 : 0;
        }
    }

    // Connection between hands
    for (int i = 0; i < 8; i++) {
        float t = (float)i / 8.0f;
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        p->position = (SDL_FPoint){hands[0].x + (hands[1].x - hands[0].x) * t + randomRangeF(-3.0f, 3.0f), hands[0].y + (hands[1].y - hands[0].y) * t + randomRangeF(-3.0f, 3.0f)};
        p->velocity = (SDL_FPoint){randomRangeF(-3.0f, 3.0f), randomRangeF(-10.0f, -5.0f)};
        p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.3f);
        p->size = randomRangeF(1.5f, 3.5f);
        p->color = (SDL_Color){fmin(255, baseColor.r + 40), fmin(255, baseColor.g + 40), fmin(255, baseColor.b + 40), 220};
        p->shape = 0;
    }
}

void emitHealParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor) {
    if (!emitter) return;

    for (int i = 0; i < 12; i++) {
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        float angle = randomRangeF(0, 2.0f * M_PI);
        float distance = randomRangeF(0.0f, radius * 0.8f);

        p->position = (SDL_FPoint){position.x + cosf(angle) * distance, position.y + sinf(angle) * distance};
        p->velocity = (SDL_FPoint){randomRangeF(-10.0f, 10.0f), randomRangeF(-40.0f, -15.0f)};
        p->lifetime = p->maxLifetime = randomRangeF(0.5f, 1.2f);
        p->size = randomRangeF(2.0f, 5.0f);

        float brightness = randomRangeF(0.8f, 1.2f);
        p->color = (SDL_Color){fmin(255, baseColor.r * brightness), fmin(255, baseColor.g * brightness), fmin(255, baseColor.b * brightness), randomRangeF(150.0f, 230.0f)};
        p->shape = rand() % 3 > 0 ? 1 : 0;  // Mostly circles
    }

    // Healing symbol
    t_entityParticle* p = getNextFreeParticle(emitter);
    if (p) {
        p->position = (SDL_FPoint){position.x, position.y - radius * 0.5f};
        p->velocity = (SDL_FPoint){0, -15.0f};
        p->lifetime = p->maxLifetime = randomRangeF(0.8f, 1.2f);
        p->size = radius * 0.5f;
        p->color = (SDL_Color){20, 220, 120, 200};  // Bright green
        p->shape = 2;                               // Healing cross
    }

    // Glowing circles
    for (int i = 0; i < 2; i++) {
        p = getNextFreeParticle(emitter);
        if (!p) continue;

        p->position = position;
        p->velocity = (SDL_FPoint){0, 0};
        p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.6f);
        p->size = radius * (0.6f + i * 0.4f);
        p->color = (SDL_Color){baseColor.r, baseColor.g, baseColor.b, (Uint8)(baseColor.a * 0.4f)};
        p->shape = 1;  // Circle
    }
}

void emitHealConnectionParticles(t_particleEmitter* emitter, SDL_FPoint source, SDL_FPoint target, SDL_Color baseColor) {
    if (!emitter) return;

    float dx = target.x - source.x;
    float dy = target.y - source.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 1.0f) return;  // Avoid division by zero

    float dirX = dx / dist;
    float dirY = dy / dist;
    float perpendicularX = -dirY;
    float perpendicularY = dirX;

    // Moving particles along beam
    int particleCount = fminf(30, dist / 5);
    for (int i = 0; i < particleCount; i++) {
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        float t = randomRangeF(0.0f, 0.4f);
        float offset = randomRangeF(0.0f, 5.0f);
        float angle = randomRangeF(0.0f, 2.0f * M_PI);

        p->position = (SDL_FPoint){source.x + dx * t + cosf(angle) * offset, source.y + dy * t + sinf(angle) * offset};

        float speed = randomRangeF(80.0f, 120.0f) * (1.2f + t * 0.3f);
        p->velocity = (SDL_FPoint){dirX * speed, dirY * speed};
        p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.4f);
        p->size = randomRangeF(3.0f, 5.0f);
        p->color = (SDL_Color){
            20,
            fmin(255, 200 + randomRangeF(-30.0f, 55.0f)),
            fmin(255, 180 + randomRangeF(-50.0f, 75.0f)),
            (Uint8)(200 + randomRangeF(-30.0f, 55.0f))};
        p->shape = rand() % 3 > 0 ? 1 : 0;  // Mostly circles
    }

    // Energy link
    int numSegments = fminf(20, dist / 10);
    for (int i = 0; i < numSegments; i++) {
        t_entityParticle* p = getNextFreeParticle(emitter);
        if (!p) continue;

        float t = (float)i / numSegments;
        float wave = sinf(t * 10.0f + SDL_GetTicks() * 0.01f) * 3.0f;

        p->position = (SDL_FPoint){
            source.x + dx * t + perpendicularX * wave,
            source.y + dy * t + perpendicularY * wave};

        p->velocity = (SDL_FPoint){dirX * randomRangeF(5.0f, 15.0f), dirY * randomRangeF(5.0f, 15.0f)};
        p->lifetime = p->maxLifetime = randomRangeF(0.1f, 0.2f);
        p->size = randomRangeF(2.0f, 4.0f);
        p->color = (SDL_Color){
            20,
            fmin(255, 150 + t * 100),
            fmin(255, 100 + t * 100),
            (Uint8)(150 + t * 100)};
        p->shape = rand() % 10 > 7 ? 0 : 1;  // Mostly circles
    }
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

        if (p->shape == 1) {
            // Dessiner un cercle
            for (int w = 0; w < size; w++) {
                for (int h = 0; h < size; h++) {
                    int dx = size / 2 - w;  // Distance du centre en x
                    int dy = size / 2 - h;  // Distance du centre en y
                    if ((dx * dx + dy * dy) <= (size / 2) * (size / 2)) {
                        SDL_RenderDrawPoint(renderer, x + w, y + h);
                    }
                }
            }
        } else {
            SDL_Rect rect = {x, y, size, size};
            SDL_RenderFillRect(renderer, &rect);

            SDL_SetRenderDrawColor(renderer, p->color.r * 0.7f, p->color.g * 0.7f, p->color.b * 0.7f, (Uint8)(p->color.a * alpha));
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}