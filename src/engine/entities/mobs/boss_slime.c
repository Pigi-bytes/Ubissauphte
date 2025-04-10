#include "boss_slime.h"

#include <math.h>
#include <stdio.h>

#include "../player.h"

#define BOSS_JUMP_FORCE 250.0f
#define BOSS_TIME_UNTIL_JUMP 1.2f
#define BOSS_TIME_UNTIL_MOVE 3.0f
#define BOSS_DETECTION_RADIUS 250.0f
#define BOSS_PATROL_MOVES 8
#define BOSS_HEALTH 1000
#define BOSS_GROUND_POUND_FORCE 350.0f
#define BOSS_CHARGE_FORCE 400.0f
#define BOSS_SPECIAL_ATTACK_COOLDOWN 5.0f
#define LAST_KNOWN_REACH_THRESHOLD 15.0f
#define FULL_JUMP_POWER 1.0f
#define CHASE_JUMP_POWER 0.8f
#define PHASE2_HEALTH_THRESHOLD 0.6f
#define PHASE3_HEALTH_THRESHOLD 0.3f
#define PHASE_TRANSITION_DURATION 4.0f
#define HEALTH_ANIMATION_SPEED 3.5f
#define HEALTH_ANIMATION_EPSILON 0.001f

float randomWithPercentageVariationBoss(float x, float percentage) {
    float min = x * (1.0f - percentage), max = x * (1.0f + percentage);
    return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

void bossSlimeDealDamageToPlayer(t_boss_slime* bossSlime, t_joueur* player, t_context* context) {
    int damage = 20;
    if (bossSlime->currentPhase >= 2) damage += 5;
    if (bossSlime->currentPhase >= 3) damage += 5;

    SDL_FPoint position = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    emitImpactParticles(bossSlime->particles, position, (SDL_FPoint){0, 0}, player->entity.collisionCircle.radius, bossSlime->particleColor);
    playerTakeDamage(player, damage, context, position);
}

void renderBossSlime(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    t_boss_slime* bossSlime = (t_boss_slime*)enemy;

    renderEntityParticles(renderer, bossSlime->particles);

    if (enemy->health.isFlashing) {
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
    }

    renderEntity(renderer, &enemy->entity, camera);

    if (enemy->health.showHealthBar && enemy->health.healthBareRender) {
        enemy->health.healthBareRender(renderer, &enemy->health, enemy->entity.displayRect, camera);
    }
}

void onBossSlimeDeath(t_context* context, void* entity) {
    t_enemy* enemy = (t_enemy*)entity;
    t_boss_slime* bossSlime = (t_boss_slime*)enemy;
    float finalBlastRadius = bossSlime->detectionRange.radius;
    Debug_PushCircle(enemy->entity.collisionCircle.x, enemy->entity.collisionCircle.y, finalBlastRadius, SDL_COLOR_RED);
    Debug_PushCircle(enemy->entity.collisionCircle.x, enemy->entity.collisionCircle.y, finalBlastRadius * 0.7f, SDL_COLOR_YELLOW);
    enemy->entity.useCircleCollision = SDL_FALSE;
    if (enemy->lastDamagedBy != NULL) {
        t_joueur* player = (t_joueur*)enemy->lastDamagedBy;  // Cast pour accéder au joueur
        addPlayerXP(player, enemy->xpReward);
        setScene(context->sceneController, "win");
        //   Ajout de l'XP au joueur
    }
}

t_enemy* createBossSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene) {
    t_boss_slime* bossSlime = malloc(sizeof(t_boss_slime));
    initEnemyBase(&bossSlime->base, texture, rect, scene);
    initHealthSystem(&bossSlime->base.health, BOSS_HEALTH);

    bossSlime->base.update = updateBossSlime;
    bossSlime->base.render = renderBossSlime;

    bossSlime->base.health.maxHealth = BOSS_HEALTH;
    bossSlime->base.health.currentHealth = BOSS_HEALTH;
    bossSlime->base.health.invincibilityDuration = 0.7f;
    bossSlime->base.health.onDeathCallback = onBossSlimeDeath;
    bossSlime->base.health.healthBareRender = NULL;
    bossSlime->base.xpReward = 500;
    bossSlime->base.entity.physics = (t_physics){.velocity = {0, 0}, .mass = 200.0f, .friction = 0.01f, .restitution = 0.8f};

    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 2}, 4, 800, SDL_TRUE, "idle"));

    // Animation walk: cycle complet de rebond (normal → aplati → étiré → normal)
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 3, 1}, 4, 250, SDL_TRUE, "walk"));

    // Animation chargingJump: aplati de plus en plus pour se préparer à sauter
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 2, 2, 2}, 5, 100, SDL_FALSE, "chargingJump"));

    // Animation midAire: étiré vers le haut pendant le saut
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){3, 3, 3}, 3, 150, SDL_TRUE, "midAire"));

    // Animation landing: séquence d'étirement → aplatissement → retour normal
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){3, 2, 2, 2, 1}, 5, 100, SDL_FALSE, "landing"));

    // Animation attack: cycle rapide pour donner un effet d'agitation
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 3, 2, 1}, 5, 130, SDL_FALSE, "attack"));

    // Animation enraged: mouvement erratique entre toutes les formes, plus rapide
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){3, 1, 2, 3, 1, 2}, 6, 80, SDL_TRUE, "enraged"));

    // Initialisation des états
    bossSlime->state = BOSS_SLIME_IDLE;
    bossSlime->base.entity.debug = SDL_TRUE;

    // Initialisation des propriétés spécifiques au Boss Slime
    bossSlime->detectionRange = (t_circle){
        .x = rect.x + rect.w / 2,
        .y = rect.y + rect.h / 2,
        .radius = randomWithPercentageVariationBoss(BOSS_DETECTION_RADIUS, 0.1)};
    bossSlime->baseDetectionRange = bossSlime->detectionRange.radius;

    bossSlime->jumpCooldownDuration = randomWithPercentageVariationBoss(BOSS_TIME_UNTIL_JUMP, 0.2);
    bossSlime->idleDurationBeforeMove = randomWithPercentageVariationBoss(BOSS_TIME_UNTIL_MOVE, 0.3);
    bossSlime->jumpForce = randomWithPercentageVariationBoss(BOSS_JUMP_FORCE, 0.2);
    bossSlime->movesBaseValue = randomWithPercentageVariationBoss(BOSS_PATROL_MOVES, 0.3);

    bossSlime->groundPoundForce = BOSS_GROUND_POUND_FORCE;
    bossSlime->chargeForce = BOSS_CHARGE_FORCE;
    bossSlime->specialAttackCooldown = BOSS_SPECIAL_ATTACK_COOLDOWN;

    bossSlime->playerInDetection = SDL_FALSE;
    bossSlime->playerInSight = SDL_FALSE;
    bossSlime->lastKnownPlayerPos = (SDL_FPoint){0, 0};

    bossSlime->movesLeft = 0;
    bossSlime->currentPhase = 1;
    bossSlime->phase2HealthThreshold = PHASE2_HEALTH_THRESHOLD;
    bossSlime->phase3HealthThreshold = PHASE3_HEALTH_THRESHOLD;
    bossSlime->invulnerabilityPhaseDuration = PHASE_TRANSITION_DURATION;
    bossSlime->isPhaseTransition = SDL_FALSE;

    // Initialisation des timers
    bossSlime->idleTimer = initDeltaTimer();
    bossSlime->jumpCooldownTimer = initDeltaTimer();
    bossSlime->specialAttackTimer = initDeltaTimer();
    bossSlime->phaseTransitionTimer = initDeltaTimer();
    startDeltaTimer(bossSlime->idleTimer);
    startDeltaTimer(bossSlime->jumpCooldownTimer);
    startDeltaTimer(bossSlime->specialAttackTimer);

    // Initialisation des particules
    bossSlime->particleColor = (SDL_Color){67, 225, 179, 200};
    bossSlime->particles = createParticleEmitter();

    return (t_enemy*)bossSlime;
}

void initiateBossJump(t_boss_slime* bossSlime, SDL_FPoint direction, float powerJump) {
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    } else {
        direction.x = 0;
        direction.y = 0;
    }

    SDL_FPoint position = {bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y};
    emitBossMovementParticles(bossSlime->particles, position, bossSlime->base.entity.collisionCircle.radius, SDL_COLOR_WHITE);

    bossSlime->base.entity.physics.velocity.x += (direction.x * (bossSlime->jumpForce * powerJump));
    bossSlime->base.entity.physics.velocity.y += (direction.y * (bossSlime->jumpForce * powerJump));
    resetDeltaTimer(bossSlime->jumpCooldownTimer);
}

void checkPhaseTransitions(t_boss_slime* bossSlime) {
    float healthPercentage = (float)bossSlime->base.health.currentHealth / (float)bossSlime->base.health.maxHealth;

    SDL_FPoint position = {bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y};

    if (healthPercentage <= bossSlime->phase2HealthThreshold && bossSlime->currentPhase == 1) {
        bossSlime->currentPhase = 2;
        bossSlime->isPhaseTransition = SDL_TRUE;

        // Particules de colère pour le changement de phase
        emitPhaseChangeParticles(bossSlime->particles, position, bossSlime->base.entity.collisionCircle.radius, (SDL_Color){255, 100, 0, 255});  // Orange vif

        resetDeltaTimer(bossSlime->phaseTransitionTimer);
        startDeltaTimer(bossSlime->phaseTransitionTimer);
        bossSlime->jumpForce *= 1.2f;
        bossSlime->jumpCooldownDuration *= 0.8f;
    } else if (healthPercentage <= bossSlime->phase3HealthThreshold && bossSlime->currentPhase == 2) {
        bossSlime->currentPhase = 3;
        bossSlime->isPhaseTransition = SDL_TRUE;

        // Particules de colère pour le changement de phase
        emitPhaseChangeParticles(bossSlime->particles, position, bossSlime->base.entity.collisionCircle.radius, (SDL_Color){255, 0, 0, 255});  // Rouge vif

        resetDeltaTimer(bossSlime->phaseTransitionTimer);
        startDeltaTimer(bossSlime->phaseTransitionTimer);
        bossSlime->jumpForce *= 1.3f;
        bossSlime->jumpCooldownDuration *= 0.7f;
    }

    if (bossSlime->isPhaseTransition && getDeltaTimer(bossSlime->phaseTransitionTimer) >= bossSlime->invulnerabilityPhaseDuration) {
        bossSlime->isPhaseTransition = SDL_FALSE;
    }
}

void performGroundPound(t_boss_slime* bossSlime, t_entity* player, t_grid* grid) {
    // Constantes de timing pour l'attaque
    const float GROUND_POUND_PREP_TIME = 0.5f;       // Temps de préparation
    const float GROUND_POUND_AIR_TIME = 0.8f;        // Temps en l'air
    const float GROUND_POUND_IMPACT_TIME = 1.0f;     // Temps d'impact
    const float GROUND_POUND_SHOCKWAVE_TIME = 1.7f;  // Temps de l'onde de choc
    const float GROUND_POUND_RECOVERY_TIME = 2.2f;   // Temps de récupération

    float elapsedTime = getDeltaTimer(bossSlime->specialAttackTimer);

    SDL_FPoint position = {bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y};
    float radius = bossSlime->base.entity.collisionCircle.radius;

    // Phase de préparation du saut - s'aplatit pour accumuler l'énergie
    if (elapsedTime < GROUND_POUND_PREP_TIME) {
        // Animation de préparation: aplatissement (frame 2 répétée)
        setAnimation(bossSlime->base.entity.animationController, "chargingJump");

        float prepProgress = elapsedTime / GROUND_POUND_PREP_TIME;  // Progression de 0 à 1

        // Effet de "tremblement" vertical préparatoire
        if ((int)(elapsedTime * 20) % 2 == 0) {
            // Petites impulsions vers le haut de plus en plus fortes
            float upwardImpulse = -5.0f - 15.0f * prepProgress;
            bossSlime->base.entity.physics.velocity.y += upwardImpulse;
        }

        // Réduire les mouvements horizontaux pendant la préparation
        bossSlime->base.entity.physics.velocity.x *= 0.7f;
        bossSlime->base.entity.physics.velocity.y *= 0.85f;  // Moins d'amortissement vertical

        // Réduire la fréquence d'émission pour éviter la surcharge
        if ((int)(elapsedTime * 10) % 2 == 0) {
            emitGroundPoundParticles(bossSlime->particles, position, radius,
                                     (SDL_Color){255, 100, 0, 255}, elapsedTime);
        }

        // Cercles de debug pour référence visuelle
        float pulseSize = 0.5f + 0.5f * sinf(elapsedTime * 10.0f);
        float baseRadius = bossSlime->detectionRange.radius * 0.3f;
        Debug_PushCircle(position.x, position.y, baseRadius * pulseSize, SDL_COLOR_RED);
        Debug_PushCircle(position.x, position.y, baseRadius * 0.7f * pulseSize, SDL_COLOR_ORANGE);
        Debug_PushCircle(position.x, position.y, baseRadius * 0.4f * pulseSize, SDL_COLOR_YELLOW);
    }
    // Phase de saut en l'air - s'étire vers le haut pendant qu'il est en l'air
    else if (elapsedTime < GROUND_POUND_AIR_TIME) {
        // Animation en l'air: étirement vertical (frame 3)
        setAnimation(bossSlime->base.entity.animationController, "midAire");

        // Simule un saut avec une "poussée" vers le haut puis une chute rapide
        float airTime = elapsedTime - GROUND_POUND_PREP_TIME;  // Temps écoulé depuis le début de cette phase

        // Calculer la hauteur approximative du slime pour ajuster l'ombre
        float estimatedHeight = 0.0f;
        if (airTime < 0.15f) {
            // Phase d'ascension
            estimatedHeight = airTime * 300.0f;  // Montée rapide
        } else {
            // Phase de descente, calcul parabolique
            float peakHeight = 45.0f;                                                 // Hauteur maximale
            float totalAirTime = 0.3f;                                                // Temps total en l'air
            float normalizedTime = (airTime - 0.15f) / (totalAirTime - 0.15f);        // Entre 0 et 1
            estimatedHeight = peakHeight * (1.0f - normalizedTime * normalizedTime);  // Parabole descendante
        }

        // Ombre au sol uniquement pendant que le boss est en l'air
        // Taille qui diminue avec la hauteur
        float shadowSize = radius * (1.5f - estimatedHeight / 100.0f);
        if (shadowSize < radius * 0.5f) shadowSize = radius * 0.5f;

        // Opacité qui diminue avec la hauteur
        int shadowOpacity = 100 - (int)(estimatedHeight / 1.5f);
        if (shadowOpacity < 30) shadowOpacity = 30;

        // Créer l'ombre
        t_entityParticle* shadow = getNextFreeParticle(bossSlime->particles);
        if (shadow) {
            shadow->position = (SDL_FPoint){position.x, position.y + radius};  // Position fixe au sol
            shadow->velocity = (SDL_FPoint){0, 0};
            shadow->lifetime = shadow->maxLifetime = 0.05f;  // Durée très courte pour être constamment mise à jour
            shadow->size = shadowSize;
            shadow->color = (SDL_Color){20, 20, 20, (Uint8)shadowOpacity};
            shadow->shape = 1;  // Cercle pour l'ombre
        }

        if (airTime < 0.1f) {
            // Impulsion initiale forte vers le haut
            bossSlime->base.entity.physics.velocity.y = -150.0f;

            // Particules d'explosion au décollage
            for (int i = 0; i < 10; i++) {
                float angle = randomRangeF(0, 2.0f * M_PI);
                float distance = radius * randomRangeF(0.5f, 1.5f);

                t_entityParticle* p = getNextFreeParticle(bossSlime->particles);
                if (p) {
                    p->position = (SDL_FPoint){
                        position.x + cosf(angle) * distance * 0.2f,
                        position.y + sinf(angle) * distance * 0.2f + radius / 2};
                    p->velocity = (SDL_FPoint){
                        cosf(angle) * randomRangeF(20.0f, 50.0f),
                        sinf(angle) * randomRangeF(20.0f, 50.0f) + 40.0f  // Dirigées vers le bas
                    };
                    p->lifetime = p->maxLifetime = randomRangeF(0.3f, 0.7f);
                    p->size = randomRangeF(4.0f, 8.0f);
                    p->color = (SDL_Color){255, 150, 50, 220};
                    p->shape = 0;  // Carrés pour l'effet d'explosion
                }
            }
        } else if (airTime < 0.25f) {
            // Maintenir une vitesse négative (vers le haut) mais avec la gravité qui ralentit
            bossSlime->base.entity.physics.velocity.y += 8.0f;  // Gravité artificielle
            // Ne pas laisser remonter trop haut
            if (bossSlime->base.entity.physics.velocity.y < -80.0f) {
                bossSlime->base.entity.physics.velocity.y = -80.0f;
            }
        } else {
            // Phase de chute avec accélération rapide
            bossSlime->base.entity.physics.velocity.y += 15.0f;  // Gravité plus forte

            // Effet visuel de "rotation" en l'air
            if ((int)(elapsedTime * 15) % 2 == 0) {
                float angle = airTime * 10.0f;
                float spinRadius = radius * 0.5f;

                t_entityParticle* p = getNextFreeParticle(bossSlime->particles);
                if (p) {
                    p->position = (SDL_FPoint){
                        position.x + cosf(angle) * spinRadius,
                        position.y + sinf(angle) * spinRadius};
                    p->velocity = (SDL_FPoint){0, 10.0f};
                    p->lifetime = p->maxLifetime = 0.2f;
                    p->size = randomRangeF(3.0f, 6.0f);
                    p->color = (SDL_Color){255, 200, 100, 180};
                    p->shape = 1;  // Ronds pour l'effet de rotation
                }
            }
        }

        // Stabiliser horizontalement pendant le saut
        bossSlime->base.entity.physics.velocity.x *= 0.1f;

        // Émission moins fréquente pour éviter la surcharge
        if ((int)(elapsedTime * 15) % 2 == 0) {
            emitGroundPoundParticles(bossSlime->particles, position, radius,
                                     (SDL_Color){255, 80, 0, 255}, elapsedTime);
        }
    }
    // Phase d'atterrissage et impact - s'écrase au sol, s'aplatit avant l'onde de choc
    else if (elapsedTime < GROUND_POUND_IMPACT_TIME) {
        // Animation d'atterrissage: transition de étiré à aplati
        setAnimation(bossSlime->base.entity.animationController, "landing");

        // Ombre qui se fond avec le boss à l'impact
        float landingProgress = (elapsedTime - GROUND_POUND_AIR_TIME) / (GROUND_POUND_IMPACT_TIME - GROUND_POUND_AIR_TIME);

        // Impulsion vers le bas pour simuler l'impact
        if (elapsedTime < 0.85f) {
            // Chute très rapide avant l'impact
            bossSlime->base.entity.physics.velocity.y = 200.0f;

            // Traînée verticale pendant la chute
            if ((int)(elapsedTime * 20) % 2 == 0) {
                for (int i = 0; i < 3; i++) {
                    t_entityParticle* p = getNextFreeParticle(bossSlime->particles);
                    if (p) {
                        p->position = (SDL_FPoint){
                            position.x + randomRangeF(-radius * 0.3f, radius * 0.3f),
                            position.y - randomRangeF(5.0f, 20.0f)};
                        p->velocity = (SDL_FPoint){
                            randomRangeF(-3.0f, 3.0f),
                            randomRangeF(-5.0f, 10.0f)};
                        p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.4f);
                        p->size = randomRangeF(2.0f, 5.0f);
                        p->color = (SDL_Color){255, 100, 0, 150};
                        p->shape = rand() % 2;
                    }
                }
            }
        } else {
            // L'impact!
            if (elapsedTime >= 0.85f && elapsedTime < 0.86f) {
                // Effet de "secousse" au moment de l'impact
                bossSlime->base.entity.physics.velocity.y = 0;

                // Explosion de particules lors de l'impact
                for (int i = 0; i < 20; i++) {
                    float angle = (float)i / 20.0f * 2.0f * M_PI;
                    float speed = randomRangeF(40.0f, 120.0f);
                    float heightVar = randomRangeF(0.0f, 10.0f);  // Variation de hauteur

                    t_entityParticle* p = getNextFreeParticle(bossSlime->particles);
                    if (p) {
                        p->position = (SDL_FPoint){
                            position.x + cosf(angle) * radius * 0.2f,
                            position.y + sinf(angle) * radius * 0.2f + radius / 2 - heightVar};
                        p->velocity = (SDL_FPoint){
                            cosf(angle) * speed,
                            (sinf(angle) * speed * 0.5f) - 20.0f  // Dirigées légèrement vers le haut
                        };
                        p->lifetime = p->maxLifetime = randomRangeF(0.4f, 0.8f);
                        p->size = randomRangeF(4.0f, 9.0f);
                        p->color = (SDL_Color){255, randomRangeF(80.0f, 160.0f), 0, 220};
                        p->shape = i % 3 == 0 ? 1 : 0;  // Mélange de carrés et cercles
                    }
                }

                // Ombre d'impact
                t_entityParticle* shadowImpact = getNextFreeParticle(bossSlime->particles);
                if (shadowImpact) {
                    shadowImpact->position = (SDL_FPoint){position.x, position.y + radius * 0.2f};
                    shadowImpact->velocity = (SDL_FPoint){0, 0};
                    shadowImpact->lifetime = shadowImpact->maxLifetime = 0.3f;
                    shadowImpact->size = radius * 2.2f;
                    shadowImpact->color = (SDL_Color){20, 20, 20, 150};
                    shadowImpact->shape = 1;  // Cercle
                }

                // Onde de choc visuelle
                t_entityParticle* shockwave = getNextFreeParticle(bossSlime->particles);
                if (shockwave) {
                    shockwave->position = position;
                    shockwave->velocity = (SDL_FPoint){0, 0};
                    shockwave->lifetime = shockwave->maxLifetime = 0.3f;
                    shockwave->size = radius * 1.5f;
                    shockwave->color = (SDL_Color){255, 180, 50, 150};
                    shockwave->shape = 1;  // Cercle pour l'onde de choc
                }
            } else {
                // Stabiliser après l'impact
                bossSlime->base.entity.physics.velocity.x *= 0.1f;
                bossSlime->base.entity.physics.velocity.y *= 0.1f;
            }
        }

        // Émission moins fréquente pour éviter la surcharge
        if ((int)(elapsedTime * 15) % 2 == 0) {
            emitGroundPoundParticles(bossSlime->particles, position, radius,
                                     (SDL_Color){255, 80, 0, 255}, elapsedTime);
        }

        float growFactor = (elapsedTime - GROUND_POUND_AIR_TIME) / (GROUND_POUND_IMPACT_TIME - GROUND_POUND_AIR_TIME);
        float impactRadius = bossSlime->detectionRange.radius * 0.5f * growFactor;
        Debug_PushCircle(position.x, position.y, impactRadius, SDL_COLOR_RED);
    }
    // Phase de l'onde de choc - reste aplati pendant l'émission de l'onde
    else if (elapsedTime < GROUND_POUND_SHOCKWAVE_TIME) {
        // Maintien de l'animation d'atterrissage qui le montre aplati
        setAnimation(bossSlime->base.entity.animationController, "landing");

        float timeInPhase = elapsedTime - GROUND_POUND_IMPACT_TIME;
        float maxRadius = bossSlime->detectionRange.radius * 0.5f;
        float waveRadius = maxRadius * (timeInPhase / (GROUND_POUND_SHOCKWAVE_TIME - GROUND_POUND_IMPACT_TIME));

        // Effet de vibration lors de l'émission de l'onde de choc
        float vibrationIntensity = 2.0f * (1.0f - (timeInPhase / (GROUND_POUND_SHOCKWAVE_TIME - GROUND_POUND_IMPACT_TIME)));
        bossSlime->base.entity.physics.velocity.x += randomRangeF(-vibrationIntensity, vibrationIntensity);
        bossSlime->base.entity.physics.velocity.y += randomRangeF(-vibrationIntensity, vibrationIntensity);

        if ((int)(timeInPhase * 10) % 2 == 0) {
            emitGroundPoundParticles(bossSlime->particles, position, radius, (SDL_Color){255, 50, 0, 255}, elapsedTime);
        }

        Debug_PushCircle(position.x, position.y, waveRadius, SDL_COLOR_RED);
        Debug_PushCircle(position.x, position.y, waveRadius * 0.8f, SDL_COLOR_ORANGE);
        Debug_PushCircle(position.x, position.y, waveRadius * 0.6f, SDL_COLOR_YELLOW);

        t_circle shockwaveCircle = {.x = position.x, .y = position.y, .radius = waveRadius};

        if (checkCircleCircleCollision(&shockwaveCircle, &player->collisionCircle, NULL)) {
            // Le joueur est touché par l'onde de choc
            SDL_FPoint knockbackDir = {player->collisionCircle.x - position.x, player->collisionCircle.y - position.y};
            float knockbackLength = sqrtf(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);

            if (knockbackLength > 0) {
                knockbackDir.x /= knockbackLength;
                knockbackDir.y /= knockbackLength;
                player->physics.velocity.x += knockbackDir.x * bossSlime->groundPoundForce * 0.7f;
                player->physics.velocity.y += knockbackDir.y * bossSlime->groundPoundForce * 0.7f;
                bossSlimeDealDamageToPlayer(bossSlime, (t_joueur*)player, NULL);
            }
        }
    }
    // Phase de récupération - retourne progressivement à sa forme normale
    else if (elapsedTime < GROUND_POUND_RECOVERY_TIME) {
        // Animation de retour à l'état normal
        setAnimation(bossSlime->base.entity.animationController, "idle");

        // Effet de rebond léger après l'attaque
        float recoveryTime = elapsedTime - GROUND_POUND_SHOCKWAVE_TIME;
        float recoveryDuration = GROUND_POUND_RECOVERY_TIME - GROUND_POUND_SHOCKWAVE_TIME;
        float recoveryProgress = recoveryTime / recoveryDuration;

        if (recoveryTime < 0.2f) {
            // Petit rebond vers le haut pour donner l'impression de reprendre sa forme
            bossSlime->base.entity.physics.velocity.y = -20.0f * (1.0f - recoveryTime / 0.2f);
        } else {
            bossSlime->base.entity.physics.velocity.x *= 0.9f;
            bossSlime->base.entity.physics.velocity.y *= 0.9f;
        }

        // Émission de petites particules de récupération
        if ((int)(elapsedTime * 8) % 2 == 0) {
            SDL_Color recoverColor = {100, 150, 255, 180};
            emitParticles(bossSlime->particles,
                          position.x + randomRangeF(-radius * 0.5f, radius * 0.5f),
                          position.y + randomRangeF(-radius * 0.3f, radius * 0.3f),
                          recoverColor,
                          2,
                          2.0f, 5.0f,
                          5.0f, 15.0f,
                          0.2f, 0.5f);
        }

        if ((int)(elapsedTime * 5) % 2 == 0) {
            Debug_PushCircle(position.x, position.y - 10, 4, SDL_COLOR_BLUE);
        }
    } else {
        // Retour à l'état approprié selon la phase
        if (bossSlime->currentPhase == 2)
            bossSlime->state = BOSS_SLIME_ENRAGED;
        else if (bossSlime->currentPhase == 3)
            bossSlime->state = BOSS_SLIME_DESPERATE;
        else
            bossSlime->state = BOSS_SLIME_CHASE_PLAYER;
    }
}
void performChargeAttack(t_boss_slime* bossSlime, t_entity* player, t_grid* grid) {
    float elapsedTime = getDeltaTimer(bossSlime->specialAttackTimer);
    static SDL_FPoint fixedChargeDir = {0, 0};
    static int hasFixedDirection = 0;

    SDL_FPoint position = {bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y};
    float radius = bossSlime->base.entity.collisionCircle.radius;

    // Constantes de timing pour l'attaque
    const float CHARGE_PREP_TIME = 1.2f;      // Temps de préparation
    const float CHARGE_INITIATE_TIME = 1.5f;  // Temps avant le dash
    const float CHARGE_DASH_TIME = 2.0f;      // Temps de dash
    const float CHARGE_RECOVERY_TIME = 2.8f;  // Temps de récupération

    if (elapsedTime < 0.1f) {
        hasFixedDirection = 0;
    }

    // Calculer la direction de charge vers le joueur
    SDL_FPoint chargeDir = {player->collisionCircle.x - position.x,
                            player->collisionCircle.y - position.y};
    float length = sqrtf(chargeDir.x * chargeDir.x + chargeDir.y * chargeDir.y);
    if (length > 0) {
        chargeDir.x /= length;
        chargeDir.y /= length;
    } else {
        chargeDir.x = 1.0f;
        chargeDir.y = 0.0f;
    }

    // Phase de préparation - s'aplatit pour accumuler l'énergie dans la direction cible
    if (elapsedTime < CHARGE_PREP_TIME) {
        // Animation d'aplatissement (compression) avant le saut
        setAnimation(bossSlime->base.entity.animationController, "chargingJump");

        // Effet de "tremblement" en s'aplatissant - le boss se prépare à charger
        float shakeFactor = 0.3f * sinf(elapsedTime * 40.0f) * (elapsedTime / CHARGE_PREP_TIME);
        bossSlime->base.entity.physics.velocity.x += shakeFactor * chargeDir.x;
        bossSlime->base.entity.physics.velocity.y += shakeFactor * chargeDir.y;

        // 1. Émission plus intense de particules pour montrer la préparation
        for (int i = 0; i < 3; i++) {
            if ((int)(elapsedTime * 18) % 2 == 0) {
                float prepProgress = elapsedTime / CHARGE_PREP_TIME;

                // Particules qui convergent vers le boss depuis la direction cible
                float distanceAhead = radius * (2.0f + prepProgress * 5.0f);
                float spread = radius * 2.0f * (1.0f - prepProgress * 0.5f);

                SDL_FPoint particlePos = {
                    position.x + chargeDir.x * distanceAhead + randomRangeF(-spread, spread),
                    position.y + chargeDir.y * distanceAhead + randomRangeF(-spread, spread)};

                // Vitesses qui convergent vers le boss
                SDL_FPoint velocity = {
                    (position.x - particlePos.x) * randomRangeF(3.0f, 7.0f) * prepProgress,
                    (position.y - particlePos.y) * randomRangeF(3.0f, 7.0f) * prepProgress};

                // Couleur intense qui varie avec la progression
                SDL_Color particleColor = {
                    (Uint8)(173 + randomRangeF(-20, 20)),
                    (Uint8)(255 + randomRangeF(-20, 20)),
                    (Uint8)(239 + randomRangeF(-20, 20)),
                    (Uint8)(100 + prepProgress * 155)};

                // Obtenir une particule libre
                t_entityParticle* p = getNextFreeParticle(bossSlime->particles);
                if (p) {
                    p->position = particlePos;
                    p->velocity = velocity;
                    p->lifetime = p->maxLifetime = randomRangeF(0.2f, 0.4f);
                    p->size = randomRangeF(3.0f, 7.0f);
                    p->color = particleColor;
                    p->shape = 0;  // Forme carrée pour effet pixelisé
                }
            }
        }

        // Autres effets vibrationnels et aura comme avant...
        // ...

        // Ralentissement général pendant la préparation
        bossSlime->base.entity.physics.velocity.x *= 0.5f;
        bossSlime->base.entity.physics.velocity.y *= 0.5f;
    }
    // Phase d'initiation du dash - s'étire en l'air avant de s'élancer
    else if (elapsedTime < CHARGE_INITIATE_TIME) {
        // Animation en l'air: étirement (frame 3) pour se préparer à la charge
        setAnimation(bossSlime->base.entity.animationController, "midAire");

        // Donner un effet de saut en l'air avant la charge
        if (elapsedTime < (CHARGE_PREP_TIME + 0.15f)) {
            // Petit bond pour s'étirer avant de charger
            bossSlime->base.entity.physics.velocity.y = -30.0f;
        } else {
            // Pause en l'air - comme suspendu avant l'impact
            bossSlime->base.entity.physics.velocity.x = 0;
            bossSlime->base.entity.physics.velocity.y = 0;
        }

        // Effets de particules d'initiation comme avant...
        float chargeProgress = (elapsedTime - CHARGE_PREP_TIME) / (CHARGE_INITIATE_TIME - CHARGE_PREP_TIME);

        // Anneaux d'énergie et autres effets de particules...
        // ...
    }
    // Phase de dash actif - forme allongée pendant la charge rapide
    else if (elapsedTime < CHARGE_DASH_TIME) {
        // Animation enragée, déformation rapide pendant le dash
        setAnimation(bossSlime->base.entity.animationController, "enraged");

        if (!hasFixedDirection) {
            fixedChargeDir = chargeDir;
            hasFixedDirection = 1;

            // Effet visuel pour le lancement du dash
            SDL_Color burstColor = {0, 238, 194, 220};
            for (int i = 0; i < 8; i++) {
                float angle = (float)i * M_PI / 4.0f;
                float burstDist = radius * 1.2f;
                Debug_PushCircle(
                    position.x + cosf(angle) * burstDist,
                    position.y + sinf(angle) * burstDist,
                    radius * 0.2f, burstColor);
            }
        }

        // Émission de particules de traînée comme avant...
        // ...

        // Vitesse maximale dans la direction fixée
        bossSlime->base.entity.physics.velocity.x = fixedChargeDir.x * bossSlime->chargeForce;
        bossSlime->base.entity.physics.velocity.y = fixedChargeDir.y * bossSlime->chargeForce;
    }
    // Phase de récupération - s'écrase puis retourne progressivement à sa forme normale
    else if (elapsedTime < CHARGE_RECOVERY_TIME) {
        // Animation landing - montre le boss qui s'aplatit à l'impact puis se reforme
        setAnimation(bossSlime->base.entity.animationController, "landing");

        // Ralentissement prononcé via la vélocité
        bossSlime->base.entity.physics.velocity.x *= 0.7f;
        bossSlime->base.entity.physics.velocity.y *= 0.7f;

        // Autres effets de récupération...
        // ...
    }
    // Fin de l'attaque
    else if (elapsedTime >= CHARGE_RECOVERY_TIME) {
        // Retour à l'état approprié selon la phase
        if (bossSlime->currentPhase == 2)
            bossSlime->state = BOSS_SLIME_ENRAGED;
        else if (bossSlime->currentPhase == 3)
            bossSlime->state = BOSS_SLIME_DESPERATE;
        else
            bossSlime->state = BOSS_SLIME_CHASE_PLAYER;
    }
}

void handleBossIdleState(t_boss_slime* bossSlime, t_entity* player, float* deltaTime) {
    bossSlime->detectionRange.radius = bossSlime->baseDetectionRange;
    setAnimation(bossSlime->base.entity.animationController, "idle");
    if (bossSlime->playerInDetection && bossSlime->playerInSight) {
        setAnimation(bossSlime->base.entity.animationController, "walk");
        bossSlime->state = BOSS_SLIME_CHASE_PLAYER;
        resetDeltaTimer(bossSlime->idleTimer);
        return;
    }
    updateDeltaTimer(bossSlime->idleTimer, *deltaTime);
    if (getDeltaTimer(bossSlime->idleTimer) >= bossSlime->idleDurationBeforeMove) {
        bossSlime->state = BOSS_SLIME_CHASE_LAST_KNOWN;
        bossSlime->movesLeft = bossSlime->movesBaseValue;
        resetDeltaTimer(bossSlime->idleTimer);
        setAnimation(bossSlime->base.entity.animationController, "walk");
    }
}

void handleBossChasePlayerState(t_boss_slime* bossSlime, t_entity* player, t_grid* grid) {
    bossSlime->detectionRange.radius = bossSlime->baseDetectionRange * 1.2f;
    setAnimation(bossSlime->base.entity.animationController, "walk");
    bossSlime->lastKnownPlayerPos = (SDL_FPoint){player->collisionCircle.x, player->collisionCircle.y};
    if (!bossSlime->playerInSight || !bossSlime->playerInDetection) {
        bossSlime->state = BOSS_SLIME_CHASE_LAST_KNOWN;
        return;
    }
    if (getDeltaTimer(bossSlime->specialAttackTimer) >= bossSlime->specialAttackCooldown) {
        int attackChoice = rand() % 10;
        if (bossSlime->currentPhase >= 2 && attackChoice < 4) {
            bossSlime->state = BOSS_SLIME_GROUND_POUND;
            resetDeltaTimer(bossSlime->specialAttackTimer);
            startDeltaTimer(bossSlime->specialAttackTimer);
            return;
        } else if (bossSlime->currentPhase >= 3 && attackChoice < 7) {
            bossSlime->state = BOSS_SLIME_CHARGE_ATTACK;
            resetDeltaTimer(bossSlime->specialAttackTimer);
            startDeltaTimer(bossSlime->specialAttackTimer);
            return;
        }
    }
    if (getDeltaTimer(bossSlime->jumpCooldownTimer) >= bossSlime->jumpCooldownDuration) {
        SDL_FPoint direction = {player->collisionCircle.x - bossSlime->detectionRange.x,
                                player->collisionCircle.y - bossSlime->detectionRange.y};
        initiateBossJump(bossSlime, direction, FULL_JUMP_POWER);
    }
}

void handleBossChaseLastKnownState(t_boss_slime* bossSlime) {
    bossSlime->detectionRange.radius = bossSlime->baseDetectionRange * 1.1f;
    setAnimation(bossSlime->base.entity.animationController, "walk");
    SDL_FPoint currentPos = {bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y};
    SDL_FPoint toLastKnown = {bossSlime->lastKnownPlayerPos.x - currentPos.x, bossSlime->lastKnownPlayerPos.y - currentPos.y};
    Debug_PushLine(currentPos.x, currentPos.y, bossSlime->lastKnownPlayerPos.x, bossSlime->lastKnownPlayerPos.y, 3, SDL_COLOR_ORANGE);
    if (bossSlime->playerInDetection && bossSlime->playerInSight) {
        if (bossSlime->currentPhase == 2)
            bossSlime->state = BOSS_SLIME_ENRAGED;
        else if (bossSlime->currentPhase == 3)
            bossSlime->state = BOSS_SLIME_DESPERATE;
        else
            bossSlime->state = BOSS_SLIME_CHASE_PLAYER;
        return;
    }
    float distance = sqrtf(toLastKnown.x * toLastKnown.x + toLastKnown.y * toLastKnown.y);
    if (distance < LAST_KNOWN_REACH_THRESHOLD) {
        bossSlime->state = BOSS_SLIME_IDLE;
        return;
    } else {
        if (getDeltaTimer(bossSlime->jumpCooldownTimer) >= bossSlime->jumpCooldownDuration) {
            initiateBossJump(bossSlime, toLastKnown, CHASE_JUMP_POWER);
        }
    }
}

void handleEnragedState(t_boss_slime* bossSlime, t_entity* player, t_grid* grid) {
    bossSlime->detectionRange.radius = bossSlime->baseDetectionRange * 1.3f;
    setAnimation(bossSlime->base.entity.animationController, "enraged");
    if (bossSlime->playerInSight) {
        if (getDeltaTimer(bossSlime->jumpCooldownTimer) >= bossSlime->jumpCooldownDuration * 0.7f) {
            SDL_FPoint direction = {player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x,
                                    player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y};
            initiateBossJump(bossSlime, direction, FULL_JUMP_POWER * 1.2f);
        }
        if (getDeltaTimer(bossSlime->specialAttackTimer) >= bossSlime->specialAttackCooldown * 0.8f) {
            int attackChoice = rand() % 2;
            if (attackChoice == 0) {
                bossSlime->state = BOSS_SLIME_GROUND_POUND;
                resetDeltaTimer(bossSlime->specialAttackTimer);
                startDeltaTimer(bossSlime->specialAttackTimer);
            } else {
                bossSlime->state = BOSS_SLIME_CHARGE_ATTACK;
                resetDeltaTimer(bossSlime->specialAttackTimer);
                startDeltaTimer(bossSlime->specialAttackTimer);
            }
        }
    } else {
        bossSlime->state = BOSS_SLIME_CHASE_LAST_KNOWN;
    }
}

void handleDesperateState(t_boss_slime* bossSlime, t_entity* player, t_grid* grid) {
    bossSlime->detectionRange.radius = bossSlime->baseDetectionRange * 1.5f;
    setAnimation(bossSlime->base.entity.animationController, "enraged");
    if (bossSlime->playerInSight) {
        if (getDeltaTimer(bossSlime->jumpCooldownTimer) >= bossSlime->jumpCooldownDuration * 0.5f) {
            if (rand() % 2 == 0) {
                SDL_FPoint direction = {player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x,
                                        player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y};
                initiateBossJump(bossSlime, direction, FULL_JUMP_POWER * 1.4f);
            } else {
                int attackChoice = rand() % 2;
                if (attackChoice == 0) {
                    bossSlime->state = BOSS_SLIME_GROUND_POUND;
                    resetDeltaTimer(bossSlime->specialAttackTimer);
                    startDeltaTimer(bossSlime->specialAttackTimer);
                } else {
                    bossSlime->state = BOSS_SLIME_CHARGE_ATTACK;
                    resetDeltaTimer(bossSlime->specialAttackTimer);
                    startDeltaTimer(bossSlime->specialAttackTimer);
                }
            }
        }
    } else {
        SDL_FPoint randomDir = {bossSlime->lastKnownPlayerPos.x - bossSlime->base.entity.collisionCircle.x + (rand() % 100 - 50),
                                bossSlime->lastKnownPlayerPos.y - bossSlime->base.entity.collisionCircle.y + (rand() % 100 - 50)};
        float length = sqrtf(randomDir.x * randomDir.x + randomDir.y * randomDir.y);
        if (length > 0) {
            randomDir.x /= length;
            randomDir.y /= length;
        }
        initiateBossJump(bossSlime, randomDir, FULL_JUMP_POWER);
        bossSlime->state = BOSS_SLIME_CHASE_LAST_KNOWN;
    }
}

void updateBossSlimeInfo(t_boss_slime* bossSlime, t_entity* player, t_grid* grid) {
    SDL_FPoint obstructionPoint;
    SDL_FPoint start = {bossSlime->detectionRange.x, bossSlime->detectionRange.y};
    SDL_FPoint end = {player->collisionCircle.x, player->collisionCircle.y};
    bossSlime->playerInDetection = checkCircleCircleCollision(&bossSlime->detectionRange, &player->collisionCircle, NULL);
    if (bossSlime->playerInDetection) {
        bossSlime->playerInSight = gridRaycast(grid, start, end, 16, &obstructionPoint);
        if (bossSlime->playerInSight)
            Debug_PushLine(start.x, start.y, end.x, end.y, 3, SDL_COLOR_GREEN);
        else
            Debug_PushLine(start.x, start.y, obstructionPoint.x, obstructionPoint.y, 3, SDL_COLOR_RED);
    } else {
        bossSlime->playerInSight = SDL_FALSE;
    }
}

void updateBossSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_boss_slime* bossSlime = (t_boss_slime*)enemy;
    t_entity* player = getObject(entities, 0);
    bossSlime->detectionRange.x = enemy->entity.collisionCircle.x;
    bossSlime->detectionRange.y = enemy->entity.collisionCircle.y;
    updateDeltaTimer(bossSlime->jumpCooldownTimer, *deltaTime);
    updateDeltaTimer(bossSlime->idleTimer, *deltaTime);
    updateDeltaTimer(bossSlime->specialAttackTimer, *deltaTime);
    updateDeltaTimer(bossSlime->phaseTransitionTimer, *deltaTime);
    updateBossSlimeInfo(bossSlime, player, grid);
    checkPhaseTransitions(bossSlime);

    t_circle extendedCollisionCircle = bossSlime->base.entity.collisionCircle;
    extendedCollisionCircle.radius += 1.0f;

    // Visualiser le cercle de collision étendu (optionnel)
    Debug_PushCircle(extendedCollisionCircle.x, extendedCollisionCircle.y, extendedCollisionCircle.radius, SDL_COLOR_PINK);

    // Vérifier la collision avec le joueur pour appliquer des dégâts de contact
    if (checkCircleCircleCollision(&extendedCollisionCircle, &player->collisionCircle, NULL)) {
        bossSlimeDealDamageToPlayer(bossSlime, (t_joueur*)player, NULL);
    }

    if (bossSlime->isPhaseTransition) {
        if (bossSlime->currentPhase == 2)
            setAnimation(bossSlime->base.entity.animationController, "enraged");
        else
            setAnimation(bossSlime->base.entity.animationController, "attack");
        if (getDeltaTimer(bossSlime->jumpCooldownTimer) >= bossSlime->jumpCooldownDuration * 0.5f) {
            float angle = (float)(rand() % 360) * (M_PI / 180.0f);
            SDL_FPoint randomDir = {cosf(angle), sinf(angle)};
            initiateBossJump(bossSlime, randomDir, CHASE_JUMP_POWER);
        }
    } else {
        switch (bossSlime->state) {
            case BOSS_SLIME_IDLE:
                handleBossIdleState(bossSlime, player, deltaTime);
                break;
            case BOSS_SLIME_CHASE_PLAYER:
                handleBossChasePlayerState(bossSlime, player, grid);
                break;
            case BOSS_SLIME_CHASE_LAST_KNOWN:
                handleBossChaseLastKnownState(bossSlime);
                break;
            case BOSS_SLIME_ENRAGED:
                handleEnragedState(bossSlime, player, grid);
                break;
            case BOSS_SLIME_DESPERATE:
                handleDesperateState(bossSlime, player, grid);
                break;
            case BOSS_SLIME_GROUND_POUND:
                performGroundPound(bossSlime, player, grid);
                break;
            case BOSS_SLIME_CHARGE_ATTACK:
                performChargeAttack(bossSlime, player, grid);
                break;
        }
    }

    updateEntityParticles(bossSlime->particles, *deltaTime);
    SDL_Color detectionColor = bossSlime->isPhaseTransition ? SDL_COLOR_YELLOW : (bossSlime->currentPhase == 3 ? SDL_COLOR_RED : (bossSlime->currentPhase == 2 ? SDL_COLOR_ORANGE : COLORDEFAULT));
    Debug_PushCircle(bossSlime->detectionRange.x, bossSlime->detectionRange.y, bossSlime->detectionRange.radius, detectionColor);
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}

t_bossHealthBar* createBossHealthBar(TTF_Font* font, t_enemy* boss) {
    t_bossHealthBar* bar = malloc(sizeof(t_bossHealthBar));
    if (!bar) return NULL;

    bar->isActive = SDL_TRUE;
    bar->font = font;
    bar->boss = boss;
    bar->bossName = "ROI SLIME: PABLO";
    bar->displayedHealthRatio = bar->targetHealthRatio = 1.0f;
    bar->currentPhase = 1;
    bar->lastDamageTime = 0;

    bar->phaseColors[0] = (SDL_Color){0, 200, 255, 255};  // Bleu
    bar->phaseColors[1] = (SDL_Color){255, 150, 0, 255};  // Orange
    bar->phaseColors[2] = (SDL_Color){255, 20, 20, 255};  // Rouge

    bar->phaseTransitionTimer = initDeltaTimer();
    startDeltaTimer(bar->phaseTransitionTimer);

    // Ne pas créer les textures ici -> a faire au premier rendu
    bar->nameText = NULL;

    return bar;
}

void updateBossHealthBar(t_bossHealthBar* bar, float deltaTime) {
    t_boss_slime* bossSlime = (t_boss_slime*)bar->boss;

    bar->targetHealthRatio = (float)bar->boss->health.currentHealth / bar->boss->health.maxHealth;
    float diff = bar->targetHealthRatio - bar->displayedHealthRatio;
    bar->displayedHealthRatio += diff * HEALTH_ANIMATION_SPEED * deltaTime;
    if (fabs(diff) < HEALTH_ANIMATION_EPSILON)
        bar->displayedHealthRatio = bar->targetHealthRatio;

    if (bossSlime->currentPhase != bar->currentPhase) {
        bar->currentPhase = bossSlime->currentPhase;
        resetDeltaTimer(bar->phaseTransitionTimer);
        startDeltaTimer(bar->phaseTransitionTimer);
    }

    if (bar->targetHealthRatio < bar->displayedHealthRatio + 0.02f)
        bar->lastDamageTime = 0.5f;
    else
        bar->lastDamageTime = fmaxf(0, bar->lastDamageTime - deltaTime);

    updateDeltaTimer(bar->phaseTransitionTimer, deltaTime);
}

void renderBossHealthBar(SDL_Renderer* renderer, t_bossHealthBar* bar, int windowWidth, int windowHeight) {
    if (!bar || !bar->isActive || !bar->boss) return;

    if (bar->nameText == NULL && renderer != NULL) {
        bar->nameText = createTextOutline(renderer, bar->bossName, bar->font, (SDL_Color){220, 220, 220, 255}, (SDL_Color){20, 20, 20, 255}, 1);
    }

    int barHeight = 24;
    int margin = 150;
    int barWidth = windowWidth - (margin * 2);
    int barY = windowHeight - 60;
    int borderSize = 3;

    t_boss_slime* bossSlime = (t_boss_slime*)bar->boss;
    int phaseIndex = bossSlime->currentPhase - 1;
    if (phaseIndex < 0) phaseIndex = 0;
    if (phaseIndex > 2) phaseIndex = 2;

    SDL_Color baseColor = bar->phaseColors[phaseIndex];
    int fillWidth = (int)(barWidth * bar->displayedHealthRatio);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &(SDL_Rect){margin - borderSize + 3, barY - borderSize + 3, barWidth + borderSize * 2, barHeight + borderSize * 2});
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Bordure et fond
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &(SDL_Rect){margin - borderSize, barY - borderSize, barWidth + borderSize * 2, barHeight + borderSize * 2});

    // Effet relief
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderDrawRect(renderer, &(SDL_Rect){margin - borderSize + 1, barY - borderSize + 1, barWidth + borderSize * 2 - 2, barHeight + borderSize * 2 - 2});

    // Fond de la barre
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderFillRect(renderer, &(SDL_Rect){margin, barY, barWidth, barHeight});

    // Barre de vie
    SDL_SetRenderDrawColor(renderer, baseColor.r, baseColor.g, baseColor.b, 255);
    SDL_RenderFillRect(renderer, &(SDL_Rect){margin, barY, fillWidth, barHeight});

    // Effet de brillance sur le haut de la barre
    SDL_SetRenderDrawColor(renderer, (Uint8)fminf(255, baseColor.r * 1.3f), (Uint8)fminf(255, baseColor.g * 1.3f), (Uint8)fminf(255, baseColor.b * 1.3f), 200);
    SDL_RenderFillRect(renderer, &(SDL_Rect){margin, barY, fillWidth, 2});

    // Effet d'ombre sur le bas de la barre
    SDL_SetRenderDrawColor(renderer, (Uint8)(baseColor.r * 0.7f), (Uint8)(baseColor.g * 0.7f), (Uint8)(baseColor.b * 0.7f), 200);
    SDL_RenderFillRect(renderer, &(SDL_Rect){margin, barY + barHeight - 2, fillWidth, 2});

    // Segments (10)
    int segmentWidth = barWidth / 10;
    for (int i = 1; i < 10; i++) {
        int segX = margin + i * segmentWidth;
        SDL_SetRenderDrawColor(renderer, segX < margin + fillWidth ? 0 : 100, segX < margin + fillWidth ? 0 : 100, segX < margin + fillWidth ? 0 : 100, 150);
        SDL_RenderFillRect(renderer, &(SDL_Rect){segX - 2, barY - 2, 4, barHeight + 4});

        // Contour des segments pour plus de visibilité
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){segX - 2, barY - 2, 4, barHeight + 4});
    }

    // Marqueurs de phase
    int phase2X = margin + (int)(barWidth * bossSlime->phase2HealthThreshold);
    int phase3X = margin + (int)(barWidth * bossSlime->phase3HealthThreshold);

    // Phase 2 avec ombre
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &(SDL_Rect){phase2X - 4 + 2, barY - 5 + 2, 8, barHeight + 10});

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &(SDL_Rect){phase2X - 4, barY - 5, 8, barHeight + 10});

    SDL_SetRenderDrawColor(renderer, bar->phaseColors[1].r, bar->phaseColors[1].g, bar->phaseColors[1].b, 220);
    SDL_RenderFillRect(renderer, &(SDL_Rect){phase2X - 3, barY - 4, 6, barHeight + 8});

    // Phase 3 avec ombre
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &(SDL_Rect){phase3X - 4 + 2, barY - 5 + 2, 8, barHeight + 10});

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &(SDL_Rect){phase3X - 4, barY - 5, 8, barHeight + 10});

    SDL_SetRenderDrawColor(renderer, bar->phaseColors[2].r, bar->phaseColors[2].g, bar->phaseColors[2].b, 220);
    SDL_RenderFillRect(renderer, &(SDL_Rect){phase3X - 3, barY - 4, 6, barHeight + 8});

    // Nom du boss avec ombre
    if (bar->nameText) {
        bar->nameText->rect.x = windowWidth / 2 - bar->nameText->rect.w / 2;
        bar->nameText->rect.y = barY - 35;
        renderText(renderer, bar->nameText);
    }
}

void freeBossHealthBar(void* obj) {
    t_bossHealthBar* bar = (t_bossHealthBar*)obj;
    if (!bar) return;

    // Libérer les textes pré-rendus
    if (bar->nameText) {
        if (bar->nameText->text) free(bar->nameText->text);
        if (bar->nameText->texture) SDL_DestroyTexture(bar->nameText->texture);
        free(bar->nameText);
    }

    if (bar->phaseTransitionTimer) free(bar->phaseTransitionTimer);
    free(bar);
}

void updateBossHealthBarWrapper(t_fonctionParam* f) {
    updateBossHealthBar(GET_PTR(f, 0, t_bossHealthBar*), *GET_PTR(f, 1, float*));
}

void renderBossHealthBarWrapper(t_fonctionParam* f) {
    renderBossHealthBar(GET_PTR(f, 0, SDL_Renderer*), GET_PTR(f, 1, t_bossHealthBar*),
                        *GET_PTR(f, 2, int*), *GET_PTR(f, 3, int*));
}