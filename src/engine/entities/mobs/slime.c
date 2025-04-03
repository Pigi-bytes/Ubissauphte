#include "slime.h"

#include "../player.h"

#define JUMP_FORCE 150.0f
#define TIME_UNTIL_JUMP 0.75f
#define TIME_UNTIL_PATROL 5.0f
#define DETECTION_RADIUS 100.0f
#define PATROL_NUMER_MOVE 10

#define LAST_KNOWN_REACH_THRESHOLD 10.0f
#define FULL_JUMP_POWER 1.0f
#define PATROL_JUMP_POWER 0.6f

float randomWithPercentageVariation(float x, float percentage) {
    float min = x * (1.0f - percentage);
    float max = x * (1.0f + percentage);
    return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

void onSlimeDeath(t_context* context, void* entity) {
    t_enemy* enemy = (t_enemy*)entity;
    t_slime* slime = (t_slime*)enemy;

    enemy->entity.useCircleCollision = SDL_FALSE;
    resetParticleEmitter(slime->particles);

    SDL_FPoint position = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};
    float radius = slime->base.entity.collisionCircle.radius;

    emitDeathParticles(slime->particles, position, radius, slime->particleColor);

    printf("Slime détruit !\n");
    if (enemy->lastDamagedBy != NULL) {
        addPlayerXP(enemy->lastDamagedBy, enemy->xpReward);
    }
}

void slimeDealDamageToPlayer(t_slime* slime, t_joueur* player, t_context* context) {
    if (!player || player->health.isInvincible || player->health.isDead) return;

    int damage = 10; 
    applyDamage(&player->health, damage, &player->entity, context);

    SDL_FPoint position = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    emitImpactParticles(slime->particles, position, (SDL_FPoint){0, 0}, player->entity.collisionCircle.radius, slime->particleColor);
    printf("Le slime inflige %d dégâts au joueur !\n", damage);
}

void slimeTakeDamage(t_enemy* enemy, int damage, t_joueur* player, t_context* context, SDL_FPoint hitDirection) {
    t_slime* slime = (t_slime*)enemy;

    SDL_FPoint position = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};
    float radius = slime->base.entity.collisionCircle.radius;
    emitImpactParticles(slime->particles, position, hitDirection, radius, slime->particleColor);

    applyDamage(&enemy->health, damage, &enemy->entity, context);
}

void renderSlime(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    renderEntityParticles(renderer, ((t_slime*)enemy)->particles);
    if (enemy->health.isDead) return;

    if (enemy->health.isFlashing) {
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
    }

    if (!(enemy->health.isInvincible && fmodf(getDeltaTimer(enemy->health.invincibilityTimer), 0.1) < 0.05)) {
        renderEntity(renderer, &enemy->entity, camera);
    }

    if (enemy->health.showHealthBar && enemy->health.healthBareRender) {
        enemy->health.healthBareRender(renderer, &enemy->health, enemy->entity.displayRect, camera);
    }
}

t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene) {
    t_slime* slime = malloc(sizeof(t_slime));
    initEnemyBase(&slime->base, texture, rect, scene);
    slime->base.update = updateSlime;
    slime->base.render = renderSlime;
    slime->base.takeDamage = slimeTakeDamage;

    slime->base.health.maxHealth = 100;
    slime->base.health.currentHealth = 100;
    slime->base.health.invincibilityDuration = 0.5f;
    slime->base.health.onDeathCallback = onSlimeDeath;

    slime->base.entity.physics = (t_physics){.velocity = {0, 0}, .mass = 2.0f, .friction = 0.02f, .restitution = 1.0f};
    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 480, SDL_TRUE, "idle"));
    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 180, SDL_TRUE, "walk"));

    slime->state = SLIME_IDLE;
    slime->base.entity.debug = SDL_TRUE;

    slime->detectionRange = (t_circle){.x = rect.x + rect.w / 2, .y = rect.y + rect.h / 2, .radius = randomWithPercentageVariation(DETECTION_RADIUS, 0.2)};
    slime->baseDetectionRange = slime->detectionRange.radius;

    slime->jumpCooldownDuration = randomWithPercentageVariation(TIME_UNTIL_JUMP, 0.3);
    slime->idleDurationBeforePatrol = randomWithPercentageVariation(TIME_UNTIL_PATROL, 0.5);
    slime->jumpForce = randomWithPercentageVariation(JUMP_FORCE, 0.4);
    slime->patrolMovesBaseValue = randomWithPercentageVariation(PATROL_NUMER_MOVE, 0.5);

    slime->jumpCooldownTimer = initDeltaTimer();
    slime->idleTimer = initDeltaTimer();

    startDeltaTimer(slime->jumpCooldownTimer);
    startDeltaTimer(slime->idleTimer);

    slime->particles = createParticleEmitter();
    slime->particleColor = (SDL_Color){67, 225, 179, 200};
    return (t_enemy*)slime;
}

void initiateSlimeJump(t_slime* slime, SDL_FPoint direction, float powerJump) {
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }

    SDL_FPoint position = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};
    emitLandingParticles(slime->particles, position, slime->base.entity.collisionCircle.radius, slime->particleColor);

    slime->base.entity.physics.velocity.x += (direction.x * (slime->jumpForce * powerJump));
    slime->base.entity.physics.velocity.y += (direction.y * (slime->jumpForce * powerJump));
    resetDeltaTimer(slime->jumpCooldownTimer);
}

void handleIdleState(t_slime* slime, t_entity* player, float* deltaTime) {
    slime->detectionRange.radius = slime->baseDetectionRange;
    setAnimation(slime->base.entity.animationController, "idle");
    if (slime->playerInDetection && slime->playerInSight) {
        setAnimation(slime->base.entity.animationController, "walk");
        slime->state = SLIME_CHASE_PLAYER;
        resetDeltaTimer(slime->idleTimer);
    }

    updateDeltaTimer(slime->idleTimer, *deltaTime);

    if (getDeltaTimer(slime->idleTimer) >= slime->idleDurationBeforePatrol) {
        slime->state = SLIME_PATROL;
        slime->patrolMovesLeft = slime->patrolMovesBaseValue;
        resetDeltaTimer(slime->idleTimer);
        setAnimation(slime->base.entity.animationController, "walk");
    }
}

void handlePatrolState(t_slime* slime, t_entity* player, t_grid* grid) {
    slime->detectionRange.radius = slime->baseDetectionRange;
    if (slime->playerInDetection && slime->playerInSight) {
        slime->state = SLIME_CHASE_PLAYER;
        return;
    }

    if (slime->patrolMovesLeft <= 0) {
        slime->state = SLIME_IDLE;
        return;
    }

    if (getDeltaTimer(slime->jumpCooldownTimer) >= slime->jumpCooldownDuration) {
        float angle = (float)(rand() % 360) * (M_PI / 180.0f);
        SDL_FPoint currentPatrolDirection = {cosf(angle), sinf(angle)};
        SDL_FPoint pos = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};

        Debug_PushLine(pos.x, pos.y, pos.x + currentPatrolDirection.x * slime->detectionRange.radius, pos.y + currentPatrolDirection.y * slime->detectionRange.radius, 3, SDL_COLOR_BLUE);
        initiateSlimeJump(slime, currentPatrolDirection, PATROL_JUMP_POWER);
        slime->patrolMovesLeft--;
    }
}

void handleChasePlayerState(t_slime* slime, t_entity* player, t_grid* grid) {
    slime->detectionRange.radius = slime->baseDetectionRange * 1.1f;

    slime->lastKnownPlayerPos = (SDL_FPoint){player->collisionCircle.x, player->collisionCircle.y};
    if (!slime->playerInSight || !slime->playerInDetection) {
        slime->state = SLIME_CHASE_LAST_KNOWN;
        return;
    }

    if (getDeltaTimer(slime->jumpCooldownTimer) >= slime->jumpCooldownDuration) {
        SDL_FPoint direction = {player->collisionCircle.x - slime->detectionRange.x, player->collisionCircle.y - slime->detectionRange.y};
        initiateSlimeJump(slime, direction, FULL_JUMP_POWER);
    }
}

void handleChaseLastKnownState(t_slime* slime) {
    slime->detectionRange.radius = slime->baseDetectionRange * 1.1f;

    SDL_FPoint currentPos = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};
    SDL_FPoint toLastKnown = {slime->lastKnownPlayerPos.x - currentPos.x, slime->lastKnownPlayerPos.y - currentPos.y};
    Debug_PushLine(currentPos.x, currentPos.y, slime->lastKnownPlayerPos.x, slime->lastKnownPlayerPos.y, 3, SDL_COLOR_ORANGE);

    if (slime->playerInDetection && slime->playerInSight) {
        slime->state = SLIME_CHASE_PLAYER;
    }

    float distance = sqrtf(toLastKnown.x * toLastKnown.x + toLastKnown.y * toLastKnown.y);
    if (distance < LAST_KNOWN_REACH_THRESHOLD) {
        slime->state = SLIME_IDLE;
        return;
    } else {
        if (getDeltaTimer(slime->jumpCooldownTimer) >= slime->jumpCooldownDuration) {
            initiateSlimeJump(slime, toLastKnown, FULL_JUMP_POWER);
        }
    }
}

void updateSlimeInfo(t_slime* slime, t_entity* player, t_grid* grid) {
    SDL_FPoint obstructionPoint;
    SDL_FPoint start = {slime->detectionRange.x, slime->detectionRange.y};
    SDL_FPoint end = {player->collisionCircle.x, player->collisionCircle.y};

    slime->playerInDetection = checkCircleCircleCollision(&slime->detectionRange, &player->collisionCircle, NULL);
    if (slime->playerInDetection) {
        slime->playerInSight = gridRaycast(grid, start, end, 16, &obstructionPoint);
        if (slime->playerInSight)
            Debug_PushLine(start.x, start.y, end.x, end.y, 3, SDL_COLOR_GREEN);
        else
            Debug_PushLine(start.x, start.y, obstructionPoint.x, obstructionPoint.y, 3, SDL_COLOR_RED);
    } else {
        slime->playerInSight = SDL_FALSE;
    }
}

void updateSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_slime* slime = (t_slime*)enemy;
    t_entity* player = getObject(entities, 0);

    slime->detectionRange.x = enemy->entity.collisionCircle.x;
    slime->detectionRange.y = enemy->entity.collisionCircle.y;

    updateDeltaTimer(slime->jumpCooldownTimer, *deltaTime);
    updateSlimeInfo(slime, player, grid);

    t_circle extendedCollisionCircle = slime->base.entity.collisionCircle;
    extendedCollisionCircle.radius += 1.0f;
    Debug_PushCircle(extendedCollisionCircle.x, extendedCollisionCircle.y, extendedCollisionCircle.radius, SDL_COLOR_PINK);

    if (checkCircleCircleCollision(&extendedCollisionCircle, &player->collisionCircle, NULL)) {
        slimeDealDamageToPlayer(slime, (t_joueur*)player, NULL);
    }

    switch (slime->state) {
        case SLIME_IDLE:
            handleIdleState(slime, player, deltaTime);
            break;

        case SLIME_PATROL:
            handlePatrolState(slime, player, grid);
            break;

        case SLIME_CHASE_PLAYER:
            handleChasePlayerState(slime, player, grid);
            break;

        case SLIME_CHASE_LAST_KNOWN:
            handleChaseLastKnownState(slime);
            break;
    }

    updateEntityParticles(slime->particles, *deltaTime);
    Debug_PushCircle(slime->detectionRange.x, slime->detectionRange.y, slime->detectionRange.radius, COLORDEFAULT);
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}