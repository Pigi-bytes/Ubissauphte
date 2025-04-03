#include "boss_slime.h"

#include <math.h>
#include <stdio.h>

#include "../player.h"

#define BOSS_JUMP_FORCE 250.0f
#define BOSS_TIME_UNTIL_JUMP 1.2f
#define BOSS_TIME_UNTIL_MOVE 3.0f
#define BOSS_DETECTION_RADIUS 250.0f
#define BOSS_PATROL_MOVES 8
#define BOSS_HEALTH 100000
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
    printf("BOSS SLIME: LE ROI EST MORT!\n");
    t_boss_slime* bossSlime = (t_boss_slime*)enemy;
    float finalBlastRadius = bossSlime->detectionRange.radius;
    Debug_PushCircle(enemy->entity.collisionCircle.x, enemy->entity.collisionCircle.y, finalBlastRadius, SDL_COLOR_RED);
    Debug_PushCircle(enemy->entity.collisionCircle.x, enemy->entity.collisionCircle.y, finalBlastRadius * 0.7f, SDL_COLOR_YELLOW);
    enemy->entity.useCircleCollision = SDL_FALSE;
    if (enemy->lastDamagedBy != NULL) {
        t_joueur* player = (t_joueur*)enemy->lastDamagedBy;  // Cast pour accéder au joueur
        addPlayerXP(player, enemy->xpReward);
        setScene(context->sceneController, "attente");
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
    bossSlime->base.xpReward = 100;
    bossSlime->base.entity.physics = (t_physics){.velocity = {0, 0}, .mass = 200.0f, .friction = 0.01f, .restitution = 0.8f};

    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 600, SDL_TRUE, "idle"));
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 250, SDL_TRUE, "walk"));
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 3, 3, 2}, 4, 150, SDL_TRUE, "attack"));
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){3, 1, 3, 1}, 4, 100, SDL_TRUE, "enraged"));

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
        printf("BOSS SLIME: ENTERING ENRAGED PHASE! WATCH OUT!\n");
        bossSlime->currentPhase = 2;
        bossSlime->isPhaseTransition = SDL_TRUE;

        // Particules de colère pour le changement de phase
        emitPhaseChangeParticles(bossSlime->particles, position, bossSlime->base.entity.collisionCircle.radius, (SDL_Color){255, 100, 0, 255});  // Orange vif

        resetDeltaTimer(bossSlime->phaseTransitionTimer);
        startDeltaTimer(bossSlime->phaseTransitionTimer);
        bossSlime->jumpForce *= 1.2f;
        bossSlime->jumpCooldownDuration *= 0.8f;
    } else if (healthPercentage <= bossSlime->phase3HealthThreshold && bossSlime->currentPhase == 2) {
        printf("BOSS SLIME: ENTERING DESPERATE PHASE! NOWHERE TO HIDE!\n");
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
    float elapsedTime = getDeltaTimer(bossSlime->specialAttackTimer);

    if (elapsedTime < 0.1f) {
        printf("BOSS SLIME: GROUND POUND INCOMING!\n");
    }

    SDL_FPoint position = {bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y};
    float radius = bossSlime->base.entity.collisionCircle.radius;

    if (elapsedTime < 0.8f) {
        setAnimation(bossSlime->base.entity.animationController, "idle");
        bossSlime->base.entity.physics.velocity.x *= 0.7f;
        bossSlime->base.entity.physics.velocity.y *= 0.7f;

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
    } else if (elapsedTime < 1.0f) {
        setAnimation(bossSlime->base.entity.animationController, "attack");
        bossSlime->base.entity.physics.velocity.x *= 0.1f;
        bossSlime->base.entity.physics.velocity.y *= 0.1f;

        // Émission moins fréquente pour éviter la surcharge
        if ((int)(elapsedTime * 15) % 2 == 0) {
            emitGroundPoundParticles(bossSlime->particles, position, radius,
                                     (SDL_Color){255, 80, 0, 255}, elapsedTime);
        }

        float growFactor = (elapsedTime - 0.8f) / 0.2f;
        float impactRadius = bossSlime->detectionRange.radius * 0.5f * growFactor;
        Debug_PushCircle(position.x, position.y, impactRadius, SDL_COLOR_RED);
        printf("BOSS SLIME: PREPARING GROUND SLAM!\n");
    } else if (elapsedTime < 1.7f) {
        float timeInPhase = elapsedTime - 1.0f;
        float maxRadius = bossSlime->detectionRange.radius * 0.5f;
        float waveRadius = maxRadius * (timeInPhase / 0.7f);

        // Réduire encore la fréquence d'émission pour la phase d'onde de choc
        if ((int)(timeInPhase * 10) % 2 == 0) {
            emitGroundPoundParticles(bossSlime->particles, position, radius,
                                     (SDL_Color){255, 50, 0, 255}, elapsedTime);
        }

        Debug_PushCircle(position.x, position.y, waveRadius, SDL_COLOR_RED);
        Debug_PushCircle(position.x, position.y, waveRadius * 0.8f, SDL_COLOR_ORANGE);
        Debug_PushCircle(position.x, position.y, waveRadius * 0.6f, SDL_COLOR_YELLOW);

        // Le reste inchangé...
        if (timeInPhase < 0.15f) {
            float distToPlayer = sqrtf(powf(player->collisionCircle.x - position.x, 2) +
                                       powf(player->collisionCircle.y - position.y, 2));
            if (distToPlayer < waveRadius) {
                SDL_FPoint knockbackDir = {player->collisionCircle.x - position.x,
                                           player->collisionCircle.y - position.y};
                float knockbackLength = sqrtf(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);
                if (knockbackLength > 0) {
                    knockbackDir.x /= knockbackLength;
                    knockbackDir.y /= knockbackLength;
                    player->physics.velocity.x += knockbackDir.x * bossSlime->groundPoundForce * 0.7f;
                    player->physics.velocity.y += knockbackDir.y * bossSlime->groundPoundForce * 0.7f;
                    printf("BOSS SLIME: PLAYER CAUGHT IN GROUND POUND!\n");
                }
            } else {
                printf("BOSS SLIME: PLAYER DODGED GROUND POUND!\n");
            }
        }
    } else if (elapsedTime < 2.2f) {
        setAnimation(bossSlime->base.entity.animationController, "idle");
        printf("BOSS SLIME: RECOVERING FROM GROUND POUND...\n");
        bossSlime->base.entity.physics.velocity.x *= 0.9f;
        bossSlime->base.entity.physics.velocity.y *= 0.9f;

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
    const float CHARGE_PREP_TIME = 1.2f;      // Temps de préparation augmenté
    const float CHARGE_INITIATE_TIME = 1.5f;  // Temps avant le dash augmenté
    const float CHARGE_DASH_TIME = 2.0f;      // Temps de dash augmenté
    const float CHARGE_RECOVERY_TIME = 2.8f;  // Temps de récupération augmenté

    if (elapsedTime < 0.1f) {
        printf("BOSS SLIME: PREPARING CHARGE ATTACK!\n");
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

    // Phase de préparation
    if (elapsedTime < CHARGE_PREP_TIME) {
        setAnimation(bossSlime->base.entity.animationController, "idle");

        // Animation de vibration via des impulsions de vélocité
        if ((int)(elapsedTime * 20) % 2 == 0) {
            float vibrationIntensity = 2.0f * elapsedTime / CHARGE_PREP_TIME;
            // Ajouter de petites impulsions de vélocité aléatoires
            bossSlime->base.entity.physics.velocity.x += randomRangeF(-vibrationIntensity, vibrationIntensity);
            bossSlime->base.entity.physics.velocity.y += randomRangeF(-vibrationIntensity, vibrationIntensity);
        }

        // Ralentissement général pendant la préparation
        bossSlime->base.entity.physics.velocity.x *= 0.5f;
        bossSlime->base.entity.physics.velocity.y *= 0.5f;

        // Émission de particules pour la préparation
        if ((int)(elapsedTime * 15) % 2 == 0) {
            emitChargeAttackParticles(bossSlime->particles, position, radius,
                                      (SDL_Color){255, 100, 0, 255},
                                      elapsedTime / CHARGE_PREP_TIME * 0.8f, chargeDir);
        }

        // Indication visuelle de la direction de charge
        float intensityFactor = elapsedTime / CHARGE_PREP_TIME;
        Debug_PushLine(position.x, position.y,
                       position.x + chargeDir.x * 200 * intensityFactor,
                       position.y + chargeDir.y * 200 * intensityFactor, 5, SDL_COLOR_ORANGE);

        // Pulse visuel autour du boss
        float pulseSize = 0.8f + 0.4f * sinf(elapsedTime * 15.0f);
        Debug_PushCircle(position.x, position.y,
                         radius * pulseSize, SDL_COLOR_ORANGE);

    }
    // Phase d'initiation du dash
    else if (elapsedTime < CHARGE_INITIATE_TIME) {
        // Animation "prêt à bondir"
        setAnimation(bossSlime->base.entity.animationController, "attack");
        printf("BOSS SLIME: CHAAARGE!\n");

        // Blocage complet du mouvement pendant l'initiation
        bossSlime->base.entity.physics.velocity.x = 0;
        bossSlime->base.entity.physics.velocity.y = 0;

        // Émission intense de particules pour l'initiation
        emitChargeAttackParticles(bossSlime->particles, position, radius,
                                  (SDL_Color){255, 100, 0, 255},
                                  0.8f + (elapsedTime - CHARGE_PREP_TIME) / (CHARGE_INITIATE_TIME - CHARGE_PREP_TIME) * 0.2f,
                                  chargeDir);

        // Flash de lumière qui s'intensifie
        float flashIntensity = (elapsedTime - CHARGE_PREP_TIME) / (CHARGE_INITIATE_TIME - CHARGE_PREP_TIME);
        SDL_Color flashColor = {
            255,
            (Uint8)(100 + 155 * flashIntensity),
            0,
            (Uint8)(100 + 155 * flashIntensity)};
        Debug_PushCircle(position.x, position.y, radius * (1.0f + flashIntensity * 0.5f), flashColor);

        // Ligne directionnelle plus prononcée
        Debug_PushLine(position.x, position.y,
                       position.x + chargeDir.x * 250,
                       position.y + chargeDir.y * 250, 8, SDL_COLOR_RED);

    }
    // Phase de dash actif
    else if (elapsedTime < CHARGE_DASH_TIME) {
        // Animation dash
        setAnimation(bossSlime->base.entity.animationController, "enraged");

        if (!hasFixedDirection) {
            fixedChargeDir = chargeDir;
            hasFixedDirection = 1;

            // Effet additionnel pour indiquer le début du dash
            SDL_Color burstColor = {255, 200, 50, 220};
            for (int i = 0; i < 12; i++) {
                float angle = (float)i * M_PI / 6.0f;
                float burstDist = radius * 1.5f;
                Debug_PushCircle(
                    position.x + cosf(angle) * burstDist,
                    position.y + sinf(angle) * burstDist,
                    radius * 0.2f, burstColor);
            }
        }

        // Émission continue de particules pendant le dash
        if ((int)(elapsedTime * 20) % 2 == 0) {
            emitChargeAttackParticles(bossSlime->particles, position, radius,
                                      (SDL_Color){255, 100, 0, 255},
                                      1.0f + (elapsedTime - CHARGE_INITIATE_TIME) / (CHARGE_DASH_TIME - CHARGE_INITIATE_TIME) * 0.5f,
                                      fixedChargeDir);
        }

        // Vitesse maximale dans la direction fixée
        bossSlime->base.entity.physics.velocity.x = fixedChargeDir.x * bossSlime->chargeForce;
        bossSlime->base.entity.physics.velocity.y = fixedChargeDir.y * bossSlime->chargeForce;

        // Traînée visuelle derrière le boss
        for (int i = 1; i <= 5; i++) {
            float distance = i * 20.0f;
            float alphaFactor = 1.0f - (float)i / 6.0f;
            SDL_Color trailColor = {255, 165, 0, (Uint8)(200 * alphaFactor)};
            Debug_PushCircle(position.x - fixedChargeDir.x * distance,
                             position.y - fixedChargeDir.y * distance,
                             radius * (1.0f - i * 0.15f), trailColor);
        }

        // Détection de collision avec le joueur
        float distToPlayer = sqrtf(powf(player->collisionCircle.x - position.x, 2) +
                                   powf(player->collisionCircle.y - position.y, 2));
        if (distToPlayer < radius + player->collisionCircle.radius) {
            SDL_FPoint knockbackDir = {player->collisionCircle.x - position.x,
                                       player->collisionCircle.y - position.y};
            float knockbackLength = sqrtf(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);
            if (knockbackLength > 0) {
                knockbackDir.x /= knockbackLength;
                knockbackDir.y /= knockbackLength;
                player->physics.velocity.x += knockbackDir.x * bossSlime->chargeForce * 0.6f;
                player->physics.velocity.y += knockbackDir.y * bossSlime->chargeForce * 0.6f;
                printf("BOSS SLIME: CHARGE HIT PLAYER!\n");

                // Effet visuel d'impact - si la fonction existe
                // Si emitImpactParticles n'existe pas, vous pouvez remplacer par une autre fonction de particules
                if ((int)(elapsedTime * 30) % 2 == 0) {
                    SDL_Color impactColor = {255, 50, 0, 255};
                    emitParticles(
                        bossSlime->particles,
                        player->collisionCircle.x,
                        player->collisionCircle.y,
                        impactColor,
                        8,             // count
                        5.0f, 10.0f,   // size
                        10.0f, 30.0f,  // speed
                        0.2f, 0.4f     // lifetime
                    );
                }
            }
        }
    }
    // Phase de récupération
    else if (elapsedTime < CHARGE_RECOVERY_TIME) {
        printf("BOSS SLIME: TIRED AFTER CHARGE...\n");

        // Animation d'épuisement
        setAnimation(bossSlime->base.entity.animationController, "idle");

        // Ralentissement très prononcé via la vélocité
        bossSlime->base.entity.physics.velocity.x *= 0.7f;
        bossSlime->base.entity.physics.velocity.y *= 0.7f;

        // Émission de particules d'épuisement
        float recoveryProgress = (elapsedTime - CHARGE_DASH_TIME) / (CHARGE_RECOVERY_TIME - CHARGE_DASH_TIME);
        if ((int)(elapsedTime * 8) % 2 == 0) {
            emitChargeAttackParticles(bossSlime->particles, position, radius,
                                      (SDL_Color){255, 100, 0, 255},
                                      1.5f + recoveryProgress * 0.5f,
                                      fixedChargeDir);
        }

        // Effet de "vacillement" pendant la récupération
        if ((int)(elapsedTime * 12) % 3 == 0) {
            // Ajouter de petites impulsions de vélocité qui s'opposent
            float swayMagnitude = 3.0f * (1.0f - recoveryProgress);
            bossSlime->base.entity.physics.velocity.x += randomRangeF(-swayMagnitude, swayMagnitude);
            bossSlime->base.entity.physics.velocity.y += randomRangeF(-swayMagnitude, swayMagnitude);
        }


    }
    // Fin de l'attaque
    else if (elapsedTime >= CHARGE_RECOVERY_TIME) {
        printf("BOSS SLIME: RECOVERED FROM CHARGE\n");

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
            printf("BOSS SLIME: PREPARING GROUND POUND ATTACK!\n");
            bossSlime->state = BOSS_SLIME_GROUND_POUND;
            resetDeltaTimer(bossSlime->specialAttackTimer);
            startDeltaTimer(bossSlime->specialAttackTimer);
            return;
        } else if (bossSlime->currentPhase >= 3 && attackChoice < 7) {
            printf("BOSS SLIME: PREPARING CHARGE ATTACK!\n");
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