#include "wizard.h"

#include <math.h>
#include <stdlib.h>

#include "../player.h"

#define HEAL_RADIUS 200.0f
#define HEAL_COOLDOWN 3.0f
#define HEAL_AMOUNT 10
#define DETECTION_RADIUS 150.0f
#define PATROL_DURATION 5.0f
#define PATROL_MOVE_SPEED 100.0f
#define SEARCH_RADIUS 300.0f

static float distanceBetweenEntities(t_entity* a, t_entity* b) {
    float dx = a->collisionCircle.x - b->collisionCircle.x;
    float dy = a->collisionCircle.y - b->collisionCircle.y;
    return sqrtf(dx * dx + dy * dy);
}

static void onWizardDeath(t_context* context, void* entity) {
    t_enemy* enemy = (t_enemy*)entity;
    t_wizard* wizard = (t_wizard*)enemy;

    enemy->entity.useCircleCollision = SDL_FALSE;
    resetParticleEmitter(wizard->particles);

    SDL_FPoint position = {wizard->base.entity.collisionCircle.x, wizard->base.entity.collisionCircle.y};
    float radius = wizard->base.entity.collisionCircle.radius;

    if (enemy->lastDamagedBy) {
        addPlayerXP(enemy->lastDamagedBy, enemy->xpReward);
    }
}

static void healNearbyEnemies(t_wizard* wizard, t_objectManager* entities) {
    SDL_FPoint wizardPos = {wizard->base.entity.collisionCircle.x, wizard->base.entity.collisionCircle.y};
    Debug_PushCircle(wizardPos.x, wizardPos.y, wizard->healRadius, SDL_COLOR_GREEN);

    t_enemy* bestTarget = NULL;
    float lowestHealthPercent = 1.0f;
    wizard->hasHealTarget = SDL_FALSE;

    for (int i = 0; i < entities->count; i++) {
        if (getObjectTypeId(entities, i) != getTypeIdByName(entities->registry, "ENEMY"))
            continue;

        t_enemy* ally = (t_enemy*)getObject(entities, i);
        if (!ally || ally == (t_enemy*)wizard || ally->health.isDead) continue;

        float dist = distanceBetweenEntities(&wizard->base.entity, &ally->entity);
        float healthPercent = (float)ally->health.currentHealth / (float)ally->health.maxHealth;

        if (dist <= SEARCH_RADIUS && healthPercent < 1.0f) {
            if (healthPercent < lowestHealthPercent) {
                lowestHealthPercent = healthPercent;
                bestTarget = ally;
            }
        }

        if (dist <= wizard->healRadius && ally->health.currentHealth < ally->health.maxHealth) {
            int healedAmount = applyHealing(&ally->health, wizard->healAmount, ally);
            if (healedAmount > 0) {
                SDL_FPoint allyPos = {ally->entity.collisionCircle.x, ally->entity.collisionCircle.y};
                Debug_PushLine(wizardPos.x, wizardPos.y, allyPos.x, allyPos.y, 2, SDL_COLOR_MAGENTA);
                emitHealConnectionParticles(wizard->particles, wizardPos, allyPos, wizard->particleColor);
                emitHealParticles(wizard->particles, allyPos, ally->entity.collisionCircle.radius, wizard->particleColor);
            }
        }
    }

    if (bestTarget) {
        wizard->targetHealPos.x = bestTarget->entity.collisionCircle.x;
        wizard->targetHealPos.y = bestTarget->entity.collisionCircle.y;
        wizard->hasHealTarget = SDL_TRUE;
    }
}

static void updateWizardInfo(t_wizard* wizard, t_entity* player, t_grid* grid) {
    SDL_FPoint obstructionPoint;
    SDL_FPoint start = {wizard->detectionRange.x, wizard->detectionRange.y};
    SDL_FPoint end = {player->collisionCircle.x, player->collisionCircle.y};

    wizard->playerInDetection = checkCircleCircleCollision(&wizard->detectionRange, &player->collisionCircle, NULL);
    if (wizard->playerInDetection) {
        wizard->playerInSight = gridRaycast(grid, start, end, 16, &obstructionPoint);
        Debug_PushLine(start.x, start.y, wizard->playerInSight ? end.x : obstructionPoint.x, wizard->playerInSight ? end.y : obstructionPoint.y, 3, wizard->playerInSight ? SDL_COLOR_GREEN : SDL_COLOR_RED);
    } else {
        wizard->playerInSight = SDL_FALSE;
    }
}

void wizardTakeDamage(t_enemy* enemy, int damage, t_joueur* player, t_context* context, SDL_FPoint hitDirection) {
    t_wizard* wizard = (t_wizard*)enemy;
    enemy->lastDamagedBy = player;

    SDL_FPoint position = {wizard->base.entity.collisionCircle.x, wizard->base.entity.collisionCircle.y};
    emitImpactParticles(wizard->particles, position, hitDirection, wizard->base.entity.collisionCircle.radius, (SDL_Color){107, 66, 33});

    wizard->hasHealTarget = SDL_FALSE;
    wizard->isPatrolling = SDL_FALSE;
    setAnimation(wizard->base.entity.animationController, "hurt");
    applyDamage(&enemy->health, damage, &enemy->entity, context);
}

static void generateRandomPatrolPoint(t_wizard* wizard, t_grid* grid) {
    float angle = randomRangeF(0, 2.0f * M_PI);
    float distance = randomRangeF(100.0f, 200.0f);
    SDL_FPoint currentPos = {wizard->base.entity.collisionCircle.x, wizard->base.entity.collisionCircle.y};

    wizard->patrolTarget.x = currentPos.x + cosf(angle) * distance;
    wizard->patrolTarget.y = currentPos.y + sinf(angle) * distance;

    int tileSize = 16;
    int gridWidth = grid->width * tileSize;
    int gridHeight = grid->height * tileSize;

    wizard->patrolTarget.x = fmaxf(tileSize, fminf(wizard->patrolTarget.x, gridWidth - tileSize));
    wizard->patrolTarget.y = fmaxf(tileSize, fminf(wizard->patrolTarget.y, gridHeight - tileSize));

    SDL_FPoint obstructionPoint;
    if (!gridRaycast(grid, currentPos, wizard->patrolTarget, tileSize, &obstructionPoint)) {
        wizard->patrolTarget = obstructionPoint;
        float dx = obstructionPoint.x - currentPos.x;
        float dy = obstructionPoint.y - currentPos.y;
        float len = sqrtf(dx * dx + dy * dy);
        if (len > 0) {
            wizard->patrolTarget.x -= (dx / len) * 20.0f;
            wizard->patrolTarget.y -= (dy / len) * 20.0f;
        }
    }

    resetDeltaTimer(wizard->patrolTimer);
}

static void updateWizard(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_wizard* wizard = (t_wizard*)enemy;
    t_entity* player = getObject(entities, 0);

    if (wizard->base.health.isDead) {
        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == enemy) {
                deleteObject(entities, i);
                return;
            }
        }
        return;
    }

    wizard->detectionRange.x = enemy->entity.collisionCircle.x;
    wizard->detectionRange.y = enemy->entity.collisionCircle.y;

    updateHealthSystem(&wizard->base.health, *deltaTime);
    updateDeltaTimer(wizard->healTimer, *deltaTime);
    updateDeltaTimer(wizard->patrolTimer, *deltaTime);
    updateWizardInfo(wizard, player, grid);

    Debug_PushCircle(wizard->detectionRange.x, wizard->detectionRange.y, wizard->detectionRange.radius, SDL_COLOR_ORANGE);
    Debug_PushCircle(wizard->detectionRange.x, wizard->detectionRange.y, wizard->fleeRadius, SDL_COLOR_RED);
    Debug_PushCircle(wizard->detectionRange.x, wizard->detectionRange.y, wizard->healRadius, SDL_COLOR_GREEN);

    SDL_FPoint currentPos = {enemy->entity.collisionCircle.x, enemy->entity.collisionCircle.y};
    SDL_FPoint direction = {0, 0};
    float moveSpeed = wizard->moveSpeed * (*deltaTime);

    float timeToNextHeal = wizard->healCooldown - getDeltaTimer(wizard->healTimer);
    if (timeToNextHeal < 0.5f) {
        if (getDeltaTimer(wizard->healTimer) >= wizard->healCooldown) {
            setAnimation(wizard->base.entity.animationController, "cast");
            SDL_FPoint position = {wizard->base.entity.collisionCircle.x, wizard->base.entity.collisionCircle.y};
            emitSpellCastParticles(wizard->particles, position, wizard->base.entity.collisionCircle.radius, wizard->particleColor);
            healNearbyEnemies(wizard, entities);
            resetDeltaTimer(wizard->healTimer);
        } else {
            healNearbyEnemies(wizard, entities);
        }
    }

    if (wizard->playerInDetection && wizard->playerInSight) {
        float distToPlayer = distanceBetweenEntities(&enemy->entity, player);
        wizard->isPatrolling = SDL_FALSE;

        if (distToPlayer < wizard->fleeRadius) {
            direction.x = currentPos.x - player->collisionCircle.x;
            direction.y = currentPos.y - player->collisionCircle.y;
            float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
                float fleeFactor = 1.5f * (1.0f - distToPlayer / wizard->fleeRadius);
                moveSpeed *= (1.0f + fleeFactor);
                Debug_PushLine(currentPos.x, currentPos.y, currentPos.x + direction.x * 30.0f, currentPos.y + direction.y * 30.0f, 2, SDL_COLOR_RED);
                setAnimation(wizard->base.entity.animationController, "cast");
            }
        }
    } else if (wizard->hasHealTarget) {
        wizard->isPatrolling = SDL_FALSE;
        direction.x = wizard->targetHealPos.x - currentPos.x;
        direction.y = wizard->targetHealPos.y - currentPos.y;
        float distToTarget = sqrtf(direction.x * direction.x + direction.y * direction.y);

        if (distToTarget > 5.0f) {
            direction.x /= distToTarget;
            direction.y /= distToTarget;
            Debug_PushLine(currentPos.x, currentPos.y, wizard->targetHealPos.x, wizard->targetHealPos.y, 2, SDL_COLOR_GREEN);
            setAnimation(wizard->base.entity.animationController, "walk");
        } else {
            setAnimation(wizard->base.entity.animationController, "idle");
        }
    } else {
        if (!wizard->isPatrolling || getDeltaTimer(wizard->patrolTimer) >= PATROL_DURATION) {
            generateRandomPatrolPoint(wizard, grid);
            wizard->isPatrolling = SDL_TRUE;
            Debug_PushCircle(wizard->patrolTarget.x, wizard->patrolTarget.y, 5.0f, SDL_COLOR_YELLOW);
        }

        direction.x = wizard->patrolTarget.x - currentPos.x;
        direction.y = wizard->patrolTarget.y - currentPos.y;
        float distToPatrolPoint = sqrtf(direction.x * direction.x + direction.y * direction.y);

        if (distToPatrolPoint < 10.0f) {
            generateRandomPatrolPoint(wizard, grid);
        } else {
            direction.x /= distToPatrolPoint;
            direction.y /= distToPatrolPoint;
            moveSpeed = PATROL_MOVE_SPEED * (*deltaTime);
            Debug_PushLine(currentPos.x, currentPos.y, wizard->patrolTarget.x, wizard->patrolTarget.y, 2, SDL_COLOR_YELLOW);
            setAnimation(wizard->base.entity.animationController, "walk");
        }
    }

    if (direction.x != 0 || direction.y != 0) {
        enemy->entity.physics.velocity.x += direction.x * moveSpeed;
        enemy->entity.physics.velocity.y += direction.y * moveSpeed;

        if (direction.x < 0) {
            enemy->entity.flip = SDL_FLIP_NONE;  // Face left
        } else if (direction.x > 0) {
            enemy->entity.flip = SDL_FLIP_HORIZONTAL;  // Face right
        }
    }

    updateEntityParticles(wizard->particles, *deltaTime);
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}

static void renderWizard(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    t_wizard* wizard = (t_wizard*)enemy;
    renderEntityParticles(renderer, wizard->particles);
    if (enemy->health.isDead) return;

    if (enemy->health.isFlashing) {
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
    }

    if (!(enemy->health.isInvincible && fmodf(getDeltaTimer(enemy->health.invincibilityTimer), 0.1f) < 0.05f)) {
        renderEntity(renderer, &enemy->entity, camera);
    }

    if (enemy->health.showHealthBar && enemy->health.healthBareRender) {
        enemy->health.healthBareRender(renderer, &enemy->health, enemy->entity.displayRect, camera);
    }
}

t_enemy* createWizard(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene) {
    t_wizard* wizard = malloc(sizeof(t_wizard));
    initEnemyBase(&wizard->base, texture, rect, scene);

    wizard->base.update = updateWizard;
    wizard->base.render = renderWizard;
    wizard->base.takeDamage = wizardTakeDamage;

    initHealthSystem(&wizard->base.health, 80);
    wizard->base.health.maxHealth = 80;
    wizard->base.health.currentHealth = 80;
    wizard->base.xpReward = 15;
    wizard->base.health.onDeathCallback = onWizardDeath;

    wizard->base.entity.physics = (t_physics){.velocity = {0, 0}, .mass = 5.0f, .friction = 0.05f, .restitution = 0.5f};

    wizard->healRadius = HEAL_RADIUS;
    wizard->healAmount = HEAL_AMOUNT;
    wizard->healCooldown = HEAL_COOLDOWN;
    wizard->healTimer = initDeltaTimer();
    startDeltaTimer(wizard->healTimer);

    wizard->moveSpeed = 60.0f;
    wizard->hasHealTarget = SDL_FALSE;
    wizard->targetHealPos = (SDL_FPoint){0, 0};

    wizard->patrolTimer = initDeltaTimer();
    wizard->patrolTarget = (SDL_FPoint){0, 0};
    wizard->isPatrolling = SDL_FALSE;
    startDeltaTimer(wizard->patrolTimer);

    addAnimation(wizard->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 480, SDL_TRUE, "idle"));
    addAnimation(wizard->base.entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 180, SDL_TRUE, "cast"));
    addAnimation(wizard->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 180, SDL_TRUE, "walk"));
    setAnimation(wizard->base.entity.animationController, "idle");

    wizard->particles = createParticleEmitter();
    wizard->particleColor = (SDL_Color){64, 255, 128, 200};

    wizard->fleeRadius = 150.0f;
    wizard->detectionRange = (t_circle){.x = rect.x + rect.w / 2, .y = rect.y + rect.h / 2, .radius = DETECTION_RADIUS};

    wizard->base.entity.debug = SDL_TRUE;
    return (t_enemy*)wizard;
}