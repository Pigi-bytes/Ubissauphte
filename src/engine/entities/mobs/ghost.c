#include "ghost.h"

#include <math.h>

#include "../player.h"

#define GHOST_VERT_AMP_MIN 70.0f
#define GHOST_VERT_AMP_MAX 120.0f
#define GHOST_HORIZ_AMP_MIN 20.0f
#define GHOST_HORIZ_AMP_MAX 40.0f
#define GHOST_VERT_FREQ_MIN 0.8f
#define GHOST_VERT_FREQ_MAX 1.5f
#define GHOST_HORIZ_FREQ_MIN 0.4f
#define GHOST_HORIZ_FREQ_MAX 1.0f

#define GHOST_TARGET_CHANGE_MIN 1.5f
#define GHOST_TARGET_CHANGE_MAX 3.0f
#define GHOST_PLAYER_ORBIT_MIN 80.0f
#define GHOST_PLAYER_ORBIT_MAX 150.0f

#define GHOST_MOVE_SPEED 35.0f
#define GHOST_NORMAL_SPEED_FACTOR 0.6f
#define GHOST_BOOST_SPEED_FACTOR 1.5f
#define GHOST_BOOST_COOLDOWN 4.0f
#define GHOST_BOOST_DURATION 1.5f

static float randomFloat(float min, float max) {
    return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

static void generateNewTarget(t_ghost* ghost, t_grid* grid, t_entity* player) {
    if (!player) return;

    float angle = randomFloat(0, 2 * M_PI);
    float orbit = randomFloat(GHOST_PLAYER_ORBIT_MIN, GHOST_PLAYER_ORBIT_MAX);

    ghost->targetPos.x = player->collisionCircle.x + cosf(angle) * orbit;
    ghost->targetPos.y = player->collisionCircle.y + sinf(angle) * orbit;

    // Limites du niveau
    if (ghost->targetPos.x < 0) ghost->targetPos.x = 0;
    if (ghost->targetPos.y < 0) ghost->targetPos.y = 0;
    if (ghost->targetPos.x >= grid->width * 16) ghost->targetPos.x = grid->width * 16 - 1;
    if (ghost->targetPos.y >= grid->height * 16) ghost->targetPos.y = grid->height * 16 - 1;

    ghost->targetChangeTime = randomFloat(GHOST_TARGET_CHANGE_MIN, GHOST_TARGET_CHANGE_MAX);
}

void onGhostDeath(t_context* context, void* entity) {
    t_enemy* enemy = (t_enemy*)entity;
    if (enemy->lastDamagedBy) addPlayerXP(enemy->lastDamagedBy, enemy->xpReward);
}

void ghostDealDamageToPlayer(t_ghost* ghost, t_joueur* player, t_context* context) {
    if (!player || player->health.isInvincible || player->health.isDead) return;

    SDL_FPoint hitDirection = {
        player->entity.collisionCircle.x - ghost->base.entity.collisionCircle.x,
        player->entity.collisionCircle.y - ghost->base.entity.collisionCircle.y};

    float length = sqrtf(hitDirection.x * hitDirection.x + hitDirection.y * hitDirection.y);
    if (length > 0) {
        hitDirection.x /= length;
        hitDirection.y /= length;
    } else {
        hitDirection.x = 0;
        hitDirection.y = -1;
    }

    playerTakeDamage(player, 15, context, hitDirection);
}

void ghostTakeDamage(t_enemy* enemy, int damage, t_joueur* player, t_context* context, SDL_FPoint hitDirection) {
    ((t_ghost*)enemy)->transparency = 120;
    applyDamage(&enemy->health, damage, &enemy->entity, context);
}

void renderGhost(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    t_ghost* ghost = (t_ghost*)enemy;

    if (enemy->health.isDead) return;

    if (enemy->health.isFlashing) {
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
    }

    SDL_SetTextureAlphaMod(enemy->entity.texture, ghost->transparency);

    if (!(enemy->health.isInvincible && fmodf(getDeltaTimer(enemy->health.invincibilityTimer), 0.1) < 0.05)) {
        renderEntity(renderer, &enemy->entity, camera);
    }

    if (enemy->health.showHealthBar && enemy->health.healthBareRender) {
        enemy->health.healthBareRender(renderer, &enemy->health, enemy->entity.displayRect, camera);
    }
}

t_enemy* createGhost(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene) {
    t_ghost* ghost = malloc(sizeof(t_ghost));

    initEnemyBase(&ghost->base, texture, rect, scene);
    initHealthSystem(&ghost->base.health, 80);

    ghost->base.update = updateGhost;
    ghost->base.render = renderGhost;
    ghost->base.takeDamage = ghostTakeDamage;

    ghost->base.health.maxHealth = 80;
    ghost->base.health.currentHealth = 80;
    ghost->base.health.invincibilityDuration = 0.5f;
    ghost->base.health.onDeathCallback = onGhostDeath;
    ghost->base.xpReward = 15;

    ghost->base.entity.physics = (t_physics){.velocity = {0, 0}, .mass = 1.0f, .friction = 0.3f, .restitution = 1.0f};

    addAnimation(ghost->base.entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 480, SDL_TRUE, "idle"));
    setAnimation(ghost->base.entity.animationController, "idle");

    ghost->base.entity.debug = SDL_TRUE;

    ghost->moveSpeed = GHOST_MOVE_SPEED;
    ghost->verticalAmplitude = randomFloat(GHOST_VERT_AMP_MIN, GHOST_VERT_AMP_MAX);
    ghost->horizontalAmplitude = randomFloat(GHOST_HORIZ_AMP_MIN, GHOST_HORIZ_AMP_MAX);
    ghost->verticalFrequency = randomFloat(GHOST_VERT_FREQ_MIN, GHOST_VERT_FREQ_MAX);
    ghost->horizontalFrequency = randomFloat(GHOST_HORIZ_FREQ_MIN, GHOST_HORIZ_FREQ_MAX);
    ghost->verticalOffset = randomFloat(0, 2 * M_PI);
    ghost->horizontalOffset = randomFloat(0, 2 * M_PI);

    ghost->targetPos = (SDL_FPoint){rect.x + rect.w / 2, rect.y + rect.h / 2};
    ghost->targetChangeTime = 0.1f;
    ghost->timer = initDeltaTimer();
    startDeltaTimer(ghost->timer);
    ghost->elapsedTime = 0.0f;

    ghost->transparency = 180;
    ghost->isBoosting = SDL_FALSE;
    ghost->boostTimer = 0.0f;
    ghost->boostCooldown = 1.0f;

    return (t_enemy*)ghost;
}

void updateGhost(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_ghost* ghost = (t_ghost*)enemy;
    t_entity* player = getObject(entities, 0);

    // Gestion de la mort
    if (ghost->base.health.isDead) {
        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == ghost) {
                deleteObject(entities, i);
                return;
            }
        }
        return;
    }

    updateHealthSystem(&enemy->health, *deltaTime);
    updateDeltaTimer(ghost->timer, *deltaTime);
    ghost->elapsedTime += *deltaTime;

    if (getDeltaTimer(ghost->timer) >= ghost->targetChangeTime) {
        generateNewTarget(ghost, grid, player);
        resetDeltaTimer(ghost->timer);
    }

    SDL_FPoint direction = {ghost->targetPos.x - enemy->entity.collisionCircle.x, ghost->targetPos.y - enemy->entity.collisionCircle.y};

    float distanceToTarget = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (distanceToTarget < 10.0f) {
        generateNewTarget(ghost, grid, player);
        resetDeltaTimer(ghost->timer);
    }
    if (distanceToTarget > 0) {
        direction.x /= distanceToTarget;
        direction.y /= distanceToTarget;
    }

    float time = ghost->elapsedTime;
    float vertOsc = sinf(time * ghost->verticalFrequency + ghost->verticalOffset) * ghost->verticalAmplitude;
    float perpX = -direction.y;
    float perpY = direction.x;
    float horizOsc = sinf(time * ghost->horizontalFrequency + ghost->horizontalOffset) * ghost->horizontalAmplitude;

    if (ghost->boostCooldown > 0) ghost->boostCooldown -= *deltaTime;
    if (ghost->isBoosting) {
        ghost->boostTimer -= *deltaTime;
        ghost->transparency = 120 + (int)(60 * sinf(time * 4.0f));

        if (ghost->boostTimer <= 0) {
            ghost->isBoosting = SDL_FALSE;
            ghost->boostCooldown = GHOST_BOOST_COOLDOWN;
            ghost->transparency = 180;
        }
    } else if (ghost->boostCooldown <= 0) {
        float distToPlayer = sqrtf(powf(player->collisionCircle.x - enemy->entity.collisionCircle.x, 2) + powf(player->collisionCircle.y - enemy->entity.collisionCircle.y, 2));

        if (distToPlayer >= GHOST_PLAYER_ORBIT_MIN && distToPlayer <= GHOST_PLAYER_ORBIT_MAX && rand() % 100 < 25) {
            ghost->isBoosting = SDL_TRUE;
            ghost->boostTimer = GHOST_BOOST_DURATION;
        }
    }

    float speedFactor = ghost->isBoosting ? GHOST_BOOST_SPEED_FACTOR : GHOST_NORMAL_SPEED_FACTOR;
    float oscFactor = ghost->isBoosting ? 0.5f : 1.0f;

    enemy->entity.physics.velocity.x += direction.x * ghost->moveSpeed * speedFactor * *deltaTime;
    enemy->entity.physics.velocity.y += direction.y * ghost->moveSpeed * speedFactor * *deltaTime;
    enemy->entity.physics.velocity.x += perpX * horizOsc * oscFactor * *deltaTime;
    enemy->entity.physics.velocity.y += perpY * horizOsc * oscFactor * *deltaTime;
    enemy->entity.physics.velocity.y += vertOsc * oscFactor * *deltaTime;

    t_circle extendedCollisionCircle = enemy->entity.collisionCircle;
    extendedCollisionCircle.radius += 1.0f;
    Debug_PushCircle(extendedCollisionCircle.x, extendedCollisionCircle.y, extendedCollisionCircle.radius, SDL_COLOR_PINK);

    if (checkCircleCircleCollision(&extendedCollisionCircle, &player->collisionCircle, NULL)) {
        ghostDealDamageToPlayer(ghost, (t_joueur*)player, NULL);
    }

    if (ghost->base.entity.debug) {
        float x = enemy->entity.collisionCircle.x;
        float y = enemy->entity.collisionCircle.y;

        Debug_PushLine(x, y, x, y + vertOsc, 2, SDL_COLOR_GREEN);
        Debug_PushLine(x, y, x + horizOsc, y, 2, SDL_COLOR_RED);
        Debug_PushLine(x, y, ghost->targetPos.x, ghost->targetPos.y, 1, SDL_COLOR_GRAY);
        Debug_PushCircle(ghost->targetPos.x, ghost->targetPos.y, 5.0f, SDL_COLOR_GRAY);

        Debug_PushCircle(player->collisionCircle.x, player->collisionCircle.y, GHOST_PLAYER_ORBIT_MIN, SDL_COLOR_GRAY);
        Debug_PushCircle(player->collisionCircle.x, player->collisionCircle.y, GHOST_PLAYER_ORBIT_MAX, SDL_COLOR_GRAY);

        if (ghost->isBoosting) {
            Debug_PushCircle(x, y, 15.0f, SDL_COLOR_ORANGE);
            float tailX = x - enemy->entity.physics.velocity.x * 0.2f;
            float tailY = y - enemy->entity.physics.velocity.y * 0.2f;
            Debug_PushLine(x, y, tailX, tailY, 3, SDL_COLOR_ORANGE);
        }
    }

    SDL_Point lastPos = {enemy->entity.collisionCircle.x, enemy->entity.collisionCircle.y};

    enemy->entity.physics.velocity.x *= powf(enemy->entity.physics.friction, *deltaTime);
    enemy->entity.physics.velocity.y *= powf(enemy->entity.physics.friction, *deltaTime);
    enemy->entity.collisionCircle.x += enemy->entity.physics.velocity.x * *deltaTime;
    enemy->entity.collisionCircle.y += enemy->entity.physics.velocity.y * *deltaTime;

    float levelMinX = 0, levelMinY = 0;
    float levelMaxX = grid->width * 16, levelMaxY = grid->height * 16;
    float radius = enemy->entity.collisionCircle.radius;

    if (enemy->entity.collisionCircle.x < levelMinX + radius)
        enemy->entity.collisionCircle.x = levelMinX + radius;
    if (enemy->entity.collisionCircle.y < levelMinY + radius)
        enemy->entity.collisionCircle.y = levelMinY + radius;
    if (enemy->entity.collisionCircle.x > levelMaxX - radius)
        enemy->entity.collisionCircle.x = levelMaxX - radius;
    if (enemy->entity.collisionCircle.y > levelMaxY - radius)
        enemy->entity.collisionCircle.y = levelMaxY - radius;

    enemy->entity.displayRect.x += enemy->entity.collisionCircle.x - lastPos.x;
    enemy->entity.displayRect.y += enemy->entity.collisionCircle.y - lastPos.y;
}