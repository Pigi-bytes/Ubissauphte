#include "slime.h"

t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_slime* slime = malloc(sizeof(t_slime));

    initEnemyBase(&slime->base, texture, rect);
    slime->base.update = updateSlime;

    slime->base.entity.physics = (t_physics){.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 2.0f, .friction = 0.07f, .restitution = 1.0f};

    slime->base.entity.debug = true;

    slime->detectionRange.x = rect.x + rect.w / 2;  // Centre X du Slime
    slime->detectionRange.y = rect.y + rect.h / 2;  // Centre Y du Slime
    slime->detectionRange.radius = 100.0f;          // Rayon de 50 pixels

    slime->slimeJumpPower = 300;
    slime->jumpCooldownValue = JUMP_COOLDOWN;

    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 480, true, "idle"));
    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 180, true, "walk"));
    setAnimation(slime->base.entity.animationController, "idle");

    slime->hasLastKnownPos = SDL_FALSE;

    return (t_enemy*)slime;
}

void initiateSlimeJump(t_slime* slime, SDL_FPoint direction) {
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }

    slime->base.entity.physics.acceleration.x = direction.x * slime->slimeJumpPower;
    slime->base.entity.physics.acceleration.y = direction.y * slime->slimeJumpPower;

    slime->isJumping = SDL_TRUE;
    slime->jumpCooldown = slime->jumpCooldownValue;
}

void updateJumpState(t_slime* slime, float deltaTime) {
    slime->base.entity.physics.acceleration = (SDL_FPoint){0, 0};
    if (slime->isJumping) {
        slime->jumpCooldown -= deltaTime;

        if (slime->jumpCooldown <= 0) {
            slime->isJumping = SDL_FALSE;
            setAnimation(slime->base.entity.animationController, "idle");
        }
    }
}

void updateSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_slime* slime = (t_slime*)enemy;
    t_entity* player = getObject(entities, 0);
    SDL_bool playerDetected = SDL_FALSE;

    slime->detectionRange.x = enemy->entity.collisionCircle.x;
    slime->detectionRange.y = enemy->entity.collisionCircle.y;
    updateJumpState(slime, *deltaTime);

    t_collisionData detectionCollision;
    if (checkCircleCircleCollision(&slime->detectionRange, &player->collisionCircle, &detectionCollision)) {
        SDL_FPoint start = {slime->detectionRange.x, slime->detectionRange.y};
        SDL_FPoint end = {player->collisionCircle.x, player->collisionCircle.y};
        SDL_FPoint obstructionPoint = {0, 0};

        if (gridRaycast(grid, start, end, 16, &obstructionPoint)) {
            slime->lastKnownPlayerPos = end;
            slime->hasLastKnownPos = SDL_TRUE;
            playerDetected = SDL_TRUE;

            Debug_PushLine(start.x, start.y, end.x, end.y, 3, SDL_COLOR_GREEN);
            setAnimation(slime->base.entity.animationController, "walk");

            // Calculer la direction vers le joueur
            SDL_FPoint toPlayer = {
                end.x - slime->base.entity.collisionCircle.x,
                end.y - slime->base.entity.collisionCircle.y};

            if (!slime->isJumping) {
                initiateSlimeJump(slime, toPlayer);
            }
        } else {
            Debug_PushLine(start.x, start.y, obstructionPoint.x, obstructionPoint.y, 3, SDL_COLOR_RED);
        }
    }

    if (!playerDetected && slime->hasLastKnownPos) {
        SDL_FPoint slimePos = {slime->base.entity.collisionCircle.x, slime->base.entity.collisionCircle.y};

        Debug_PushLine(slimePos.x, slimePos.y, slime->lastKnownPlayerPos.x, slime->lastKnownPlayerPos.y, 3, SDL_COLOR_ORANGE);

        SDL_FPoint toLastKnown = {
            slime->lastKnownPlayerPos.x - slimePos.x,
            slime->lastKnownPlayerPos.y - slimePos.y};

        float distance = sqrtf(toLastKnown.x * toLastKnown.x + toLastKnown.y * toLastKnown.y);
        if (distance < 10.0f) {
            slime->hasLastKnownPos = SDL_FALSE;
        } else if (!slime->isJumping) {
            initiateSlimeJump(slime, toLastKnown);
        }
    }

    Debug_PushCircle(slime->detectionRange.x, slime->detectionRange.y, slime->detectionRange.radius, COLORDEFAULT);
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}