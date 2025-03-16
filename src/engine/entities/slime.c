#include "slime.h"

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

t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_slime* slime = malloc(sizeof(t_slime));
    initEnemyBase(&slime->base, texture, rect);
    slime->base.update = updateSlime;

    slime->base.entity.physics = (t_physics){.velocity = {0, 0}, .acceleration = {0, 0}, .mass = 2.0f, .friction = 0.07f, .restitution = 1.0f};
    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 480, true, "idle"));
    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 180, true, "walk"));

    slime->state = SLIME_IDLE;

    slime->detectionRange = (t_circle){.x = rect.x + rect.w / 2, .y = rect.y + rect.h / 2, .radius = randomWithPercentageVariation(DETECTION_RADIUS, 0.2)};

    slime->jumpCooldownDuration = randomWithPercentageVariation(TIME_UNTIL_JUMP, 0.3);
    slime->idleDurationBeforePatrol = randomWithPercentageVariation(TIME_UNTIL_PATROL, 0.5);
    slime->jumpForce = randomWithPercentageVariation(JUMP_FORCE, 0.4);
    slime->patrolMovesBaseValue = randomWithPercentageVariation(PATROL_NUMER_MOVE, 0.5);

    slime->slimeJumpCooldownTimer = 0.0;

    return (t_enemy*)slime;
}

void initiateSlimeJump(t_slime* slime, SDL_FPoint direction, float powerJump) {
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }

    slime->base.entity.physics.acceleration.x += direction.x * (slime->jumpForce * powerJump);
    slime->base.entity.physics.acceleration.y += direction.y * (slime->jumpForce * powerJump);
    slime->slimeJumpCooldownTimer = slime->jumpCooldownDuration;
}

void handleIdleState(t_slime* slime, t_entity* player, float* deltaTime) {
    setAnimation(slime->base.entity.animationController, "idle");
    if (slime->playerInDetection && slime->playerInSight) {
        setAnimation(slime->base.entity.animationController, "walk");
        slime->state = SLIME_CHASE_PLAYER;
        slime->idleTimer = 0.0f;
    }

    slime->idleTimer += *deltaTime;

    if (slime->idleTimer >= slime->idleDurationBeforePatrol) {
        slime->state = SLIME_PATROL;
        slime->patrolMovesLeft = slime->patrolMovesBaseValue;
        slime->idleTimer = 0.0f;
        setAnimation(slime->base.entity.animationController, "walk");
    }
}

void handlePatrolState(t_slime* slime, t_entity* player, t_grid* grid) {
    if (slime->playerInDetection && slime->playerInSight) {
        slime->state = SLIME_CHASE_PLAYER;
        return;
    }

    if (slime->patrolMovesLeft <= 0) {
        slime->state = SLIME_IDLE;
        return;
    }

    if (slime->slimeJumpCooldownTimer <= 0) {
        float angle = (float)(rand() % 360) * (M_PI / 180.0f);
        SDL_FPoint currentPatrolDirection = {cosf(angle), sinf(angle)};
        SDL_FPoint pos = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};

        Debug_PushLine(pos.x, pos.y, pos.x + currentPatrolDirection.x * slime->detectionRange.radius, pos.y + currentPatrolDirection.y * slime->detectionRange.radius, 3, SDL_COLOR_BLUE);
        initiateSlimeJump(slime, currentPatrolDirection, PATROL_JUMP_POWER);
        slime->patrolMovesLeft--;
    }
}

void handleChasePlayerState(t_slime* slime, t_entity* player, t_grid* grid) {
    slime->lastKnownPlayerPos = (SDL_FPoint){player->collisionCircle.x, player->collisionCircle.y};
    if (!slime->playerInSight || !slime->playerInDetection) {
        slime->state = SLIME_CHASE_LAST_KNOWN;
        return;
    }

    if (slime->slimeJumpCooldownTimer <= 0) {
        SDL_FPoint direction = {player->collisionCircle.x - slime->detectionRange.x, player->collisionCircle.y - slime->detectionRange.y};
        initiateSlimeJump(slime, direction, FULL_JUMP_POWER);
    }
}

void handleChaseLastKnownState(t_slime* slime) {
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
        if (slime->slimeJumpCooldownTimer <= 0) {
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
    slime->base.entity.physics.acceleration.x = 0;
    slime->base.entity.physics.acceleration.y = 0;

    slime->slimeJumpCooldownTimer -= *deltaTime;

    updateSlimeInfo(slime, player, grid);

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

    Debug_PushCircle(slime->detectionRange.x, slime->detectionRange.y, slime->detectionRange.radius, COLORDEFAULT);
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}