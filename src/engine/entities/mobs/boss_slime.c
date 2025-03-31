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
        addPlayerXP(player, enemy->xpReward);                // Ajout de l'XP au joueur
    }
}

t_enemy* createBossSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene) {
    t_boss_slime* bossSlime = malloc(sizeof(t_boss_slime));
    initEnemyBase(&bossSlime->base, texture, rect, scene);
    bossSlime->base.update = updateBossSlime;
    bossSlime->base.health.maxHealth = bossSlime->base.health.currentHealth = BOSS_HEALTH;
    bossSlime->base.health.invincibilityDuration = 0.7f;
    bossSlime->base.health.onDeathCallback = onBossSlimeDeath;
    bossSlime->base.health.healthBareRender = NULL;
    bossSlime->base.xpReward = 100;
    bossSlime->base.entity.physics = (t_physics){.velocity = {0, 0}, .mass = 200.0f, .friction = 0.01f, .restitution = 0.8f};

    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 600, SDL_TRUE, "idle"));
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 250, SDL_TRUE, "walk"));
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){1, 3, 3, 2}, 4, 150, SDL_TRUE, "attack"));
    addAnimation(bossSlime->base.entity.animationController, createAnimation(tileset, (int[]){3, 1, 3, 1}, 4, 100, SDL_TRUE, "enraged"));

    bossSlime->state = BOSS_SLIME_IDLE;
    bossSlime->base.entity.debug = SDL_TRUE;
    bossSlime->detectionRange = (t_circle){.x = rect.x + rect.w / 2, .y = rect.y + rect.h / 2, .radius = randomWithPercentageVariationBoss(BOSS_DETECTION_RADIUS, 0.1)};
    bossSlime->baseDetectionRange = bossSlime->detectionRange.radius;
    bossSlime->jumpCooldownDuration = randomWithPercentageVariationBoss(BOSS_TIME_UNTIL_JUMP, 0.2);
    bossSlime->idleDurationBeforeMove = randomWithPercentageVariationBoss(BOSS_TIME_UNTIL_MOVE, 0.3);
    bossSlime->jumpForce = randomWithPercentageVariationBoss(BOSS_JUMP_FORCE, 0.2);
    bossSlime->movesBaseValue = randomWithPercentageVariationBoss(BOSS_PATROL_MOVES, 0.3);
    bossSlime->groundPoundForce = BOSS_GROUND_POUND_FORCE;
    bossSlime->chargeForce = BOSS_CHARGE_FORCE;
    bossSlime->specialAttackCooldown = BOSS_SPECIAL_ATTACK_COOLDOWN;
    bossSlime->currentPhase = 1;
    bossSlime->phase2HealthThreshold = PHASE2_HEALTH_THRESHOLD;
    bossSlime->phase3HealthThreshold = PHASE3_HEALTH_THRESHOLD;
    bossSlime->invulnerabilityPhaseDuration = PHASE_TRANSITION_DURATION;
    bossSlime->isPhaseTransition = SDL_FALSE;

    bossSlime->jumpCooldownTimer = initDeltaTimer();
    bossSlime->idleTimer = initDeltaTimer();
    bossSlime->specialAttackTimer = initDeltaTimer();
    bossSlime->phaseTransitionTimer = initDeltaTimer();
    startDeltaTimer(bossSlime->jumpCooldownTimer);
    startDeltaTimer(bossSlime->idleTimer);
    startDeltaTimer(bossSlime->specialAttackTimer);
    return (t_enemy*)bossSlime;
}

void initiateBossJump(t_boss_slime* bossSlime, SDL_FPoint direction, float powerJump) {
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    bossSlime->base.entity.physics.velocity.x += (direction.x * (bossSlime->jumpForce * powerJump));
    bossSlime->base.entity.physics.velocity.y += (direction.y * (bossSlime->jumpForce * powerJump));
    resetDeltaTimer(bossSlime->jumpCooldownTimer);
}

void checkPhaseTransitions(t_boss_slime* bossSlime) {
    float healthPercentage = (float)bossSlime->base.health.currentHealth / (float)bossSlime->base.health.maxHealth;
    if (healthPercentage <= bossSlime->phase2HealthThreshold && bossSlime->currentPhase == 1) {
        printf("BOSS SLIME: ENTERING ENRAGED PHASE! WATCH OUT!\n");
        bossSlime->currentPhase = 2;
        bossSlime->isPhaseTransition = SDL_TRUE;
        resetDeltaTimer(bossSlime->phaseTransitionTimer);
        startDeltaTimer(bossSlime->phaseTransitionTimer);
        bossSlime->jumpForce *= 1.2f;
        bossSlime->jumpCooldownDuration *= 0.8f;
    } else if (healthPercentage <= bossSlime->phase3HealthThreshold && bossSlime->currentPhase == 2) {
        printf("BOSS SLIME: ENTERING DESPERATE PHASE! NOWHERE TO HIDE!\n");
        bossSlime->currentPhase = 3;
        bossSlime->isPhaseTransition = SDL_TRUE;
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
    if (elapsedTime < 0.8f) {
        setAnimation(bossSlime->base.entity.animationController, "idle");
        bossSlime->base.entity.physics.velocity.x *= 0.7f;
        bossSlime->base.entity.physics.velocity.y *= 0.7f;
        float pulseSize = 0.5f + 0.5f * sinf(elapsedTime * 10.0f);
        float baseRadius = bossSlime->detectionRange.radius * 0.3f;
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, baseRadius * pulseSize, SDL_COLOR_RED);
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, baseRadius * 0.7f * pulseSize, SDL_COLOR_ORANGE);
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, baseRadius * 0.4f * pulseSize, SDL_COLOR_YELLOW);
    } else if (elapsedTime < 1.0f) {
        setAnimation(bossSlime->base.entity.animationController, "attack");
        bossSlime->base.entity.physics.velocity.x *= 0.1f;
        bossSlime->base.entity.physics.velocity.y *= 0.1f;
        float growFactor = (elapsedTime - 0.8f) / 0.2f;
        float impactRadius = bossSlime->detectionRange.radius * 0.5f * growFactor;
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, impactRadius, SDL_COLOR_RED);
        printf("BOSS SLIME: PREPARING GROUND SLAM!\n");
    } else if (elapsedTime < 1.7f) {
        float timeInPhase = elapsedTime - 1.0f;
        float maxRadius = bossSlime->detectionRange.radius * 0.5f;
        float waveRadius = maxRadius * (timeInPhase / 0.7f);
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, waveRadius, SDL_COLOR_RED);
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, waveRadius * 0.8f, SDL_COLOR_ORANGE);
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y, waveRadius * 0.6f, SDL_COLOR_YELLOW);
        if (timeInPhase < 0.1f) printf("BOSS SLIME: BOOM!\n");
        if (timeInPhase < 0.15f) {
            float distToPlayer = sqrtf(powf(player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x, 2) +
                                       powf(player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y, 2));
            if (distToPlayer < waveRadius) {
                SDL_FPoint knockbackDir = {player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x,
                                           player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y};
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
        if ((int)(elapsedTime * 5) % 2 == 0) {
            Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y - 10, 4, SDL_COLOR_BLUE);
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

    if (elapsedTime < 0.1f) {
        printf("BOSS SLIME: PREPARING CHARGE ATTACK!\n");
        hasFixedDirection = 0;
    }
    if (elapsedTime < 0.8f) {
        setAnimation(bossSlime->base.entity.animationController, "idle");
        bossSlime->base.entity.physics.velocity.x *= 0.6f;
        bossSlime->base.entity.physics.velocity.y *= 0.6f;
        SDL_FPoint chargeDir = {player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x,
                                player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y};
        float length = sqrtf(chargeDir.x * chargeDir.x + chargeDir.y * chargeDir.y);
        if (length > 0) {
            chargeDir.x /= length;
            chargeDir.y /= length;
            float intensityFactor = elapsedTime / 0.8f;
            Debug_PushLine(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y,
                           bossSlime->base.entity.collisionCircle.x + chargeDir.x * 150 * intensityFactor,
                           bossSlime->base.entity.collisionCircle.y + chargeDir.y * 150 * intensityFactor, 5, SDL_COLOR_ORANGE);
        }
        float pulseSize = 0.8f + 0.2f * sinf(elapsedTime * 15.0f);
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y,
                         bossSlime->base.entity.collisionCircle.radius * pulseSize, SDL_COLOR_ORANGE);
    } else if (elapsedTime < 1.0f) {
        printf("BOSS SLIME: CHAAARGE!\n");
        SDL_FPoint chargeDir = {player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x,
                                player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y};
        float length = sqrtf(chargeDir.x * chargeDir.x + chargeDir.y * chargeDir.y);
        if (length > 0) {
            chargeDir.x /= length;
            chargeDir.y /= length;
            Debug_PushLine(bossSlime->base.entity.collisionCircle.x, bossSlime->base.entity.collisionCircle.y,
                           bossSlime->base.entity.collisionCircle.x + chargeDir.x * 200,
                           bossSlime->base.entity.collisionCircle.y + chargeDir.y * 200, 7, SDL_COLOR_RED);
        }
    } else if (elapsedTime < 1.5f) {
        setAnimation(bossSlime->base.entity.animationController, "enraged");
        if (!hasFixedDirection) {
            fixedChargeDir.x = player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x;
            fixedChargeDir.y = player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y;
            float length = sqrtf(fixedChargeDir.x * fixedChargeDir.x + fixedChargeDir.y * fixedChargeDir.y);
            if (length > 0) {
                fixedChargeDir.x /= length;
                fixedChargeDir.y /= length;
            }
            hasFixedDirection = 1;
        }
        bossSlime->base.entity.physics.velocity.x = fixedChargeDir.x * bossSlime->chargeForce;
        bossSlime->base.entity.physics.velocity.y = fixedChargeDir.y * bossSlime->chargeForce;
        for (int i = 1; i <= 5; i++) {
            float distance = i * 15.0f;
            Debug_PushCircle(bossSlime->base.entity.collisionCircle.x - fixedChargeDir.x * distance,
                             bossSlime->base.entity.collisionCircle.y - fixedChargeDir.y * distance,
                             bossSlime->base.entity.collisionCircle.radius * (1.0f - i * 0.15f), SDL_COLOR_YELLOW);
        }
        float distToPlayer = sqrtf(powf(player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x, 2) +
                                   powf(player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y, 2));
        if (distToPlayer < bossSlime->base.entity.collisionCircle.radius + player->collisionCircle.radius) {
            SDL_FPoint knockbackDir = {player->collisionCircle.x - bossSlime->base.entity.collisionCircle.x,
                                       player->collisionCircle.y - bossSlime->base.entity.collisionCircle.y};
            float knockbackLength = sqrtf(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);
            if (knockbackLength > 0) {
                knockbackDir.x /= knockbackLength;
                knockbackDir.y /= knockbackLength;
                player->physics.velocity.x += knockbackDir.x * bossSlime->chargeForce * 0.6f;
                player->physics.velocity.y += knockbackDir.y * bossSlime->chargeForce * 0.6f;
                printf("BOSS SLIME: CHARGE HIT PLAYER!\n");
            }
        }
    } else if (elapsedTime < 2.0f) {
        printf("BOSS SLIME: TIRED AFTER CHARGE...\n");
        bossSlime->base.entity.physics.velocity.x *= 0.85f;
        bossSlime->base.entity.physics.velocity.y *= 0.85f;
        setAnimation(bossSlime->base.entity.animationController, "idle");
        float angle = (float)(rand() % 360) * (M_PI / 180.0f);
        float distance = (float)(rand() % 10) + 5.0f;
        Debug_PushCircle(bossSlime->base.entity.collisionCircle.x + cosf(angle) * distance,
                         bossSlime->base.entity.collisionCircle.y + sinf(angle) * distance,
                         2 + (float)(rand() % 3), SDL_COLOR_BLUE);
    } else if (elapsedTime >= 2.0f) {
        printf("BOSS SLIME: RECOVERED FROM CHARGE\n");
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