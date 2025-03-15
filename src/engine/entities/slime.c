#include "slime.h"

t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_slime* slime = malloc(sizeof(t_slime));

    initEnemyBase(&slime->base, texture, rect);
    slime->base.update = updateSlime;

    slime->base.entity.physics = (t_physics){.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 1.0f, .friction = 0.07f, .restitution = 1.0f};

    slime->base.entity.debug = true;

    slime->detectionRange.x = rect.x + rect.w / 2;  // Centre X du Slime
    slime->detectionRange.y = rect.y + rect.h / 2;  // Centre Y du Slime
    slime->detectionRange.radius = 100.0f;          // Rayon de 50 pixels

    slime->slimeJumpPower = 300;
    slime->jumpCooldownValue = JUMP_COOLDOWN;

    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 480, true, "idle"));
    addAnimation(slime->base.entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 180, true, "walk"));
    setAnimation(slime->base.entity.animationController, "idle");

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
    slime->detectionRange.x = enemy->entity.collisionCircle.x;
    slime->detectionRange.y = enemy->entity.collisionCircle.y;
    updateJumpState(slime, *deltaTime);

    t_entity* player = getObject(entities, 0);

    t_collisionData collision;
    if (checkCircleCircleCollision(&slime->detectionRange, &player->collisionCircle, &collision)) {
        SDL_FPoint start = {slime->detectionRange.x, slime->detectionRange.y};
        SDL_FPoint end = {player->collisionCircle.x, player->collisionCircle.y};
        SDL_FPoint obstructionPoint = {0, 0};

        if (gridRaycast(grid, start, end, 16, &obstructionPoint)) {
            Debug_PushLine(start.x, start.y, end.x, end.y, 3, SDL_COLOR_GREEN);
            setAnimation(slime->base.entity.animationController, "walk");

            SDL_FPoint toPlayer = {
                player->collisionCircle.x - slime->base.entity.collisionCircle.x,
                player->collisionCircle.y - slime->base.entity.collisionCircle.y};

            if (!slime->isJumping) {
                printf("jump \n");
                initiateSlimeJump(slime, toPlayer);
            }

        } else {
            Debug_PushLine(start.x, start.y, obstructionPoint.x, obstructionPoint.y, 3, SDL_COLOR_RED);
            setAnimation(slime->base.entity.animationController, "idle");
        }
    } else {
        setAnimation(slime->base.entity.animationController, "idle");
    }

    Debug_PushCircle(slime->detectionRange.x, slime->detectionRange.y, slime->detectionRange.radius, COLORDEFAULT);
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}