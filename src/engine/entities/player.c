#include "player.h"

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.displayRect = rect;
    joueur->entity.flip = SDL_FLIP_NONE;
    joueur->entity.animationController = initAnimationController();
    joueur->entity.debug = SDL_FALSE;

    joueur->entity.useCircleCollision = SDL_TRUE;
    joueur->entity.collisionCircle.x = rect.x + rect.w / 2;
    joueur->entity.collisionCircle.y = rect.y + rect.h / 2;
    joueur->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;  // Set radius to half the smaller dimension of the rect

    t_physics playerPhysics = {
        .velocity = {0, 0},  // Vitesse initiale
        .mass = 10.0f,
        .friction = 0.02f,
        .restitution = 0.02};

    joueur->entity.physics = playerPhysics;
    joueur->isAttacking = SDL_FALSE;

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, SDL_TRUE, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 240, SDL_TRUE, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

    return joueur;
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    renderEntity(renderer, &player->entity, camera);

    player->arme.displayRect.x = (player->entity.collisionCircle.x - player->arme.displayRect.w / 2) + 8;
    player->arme.displayRect.y = (player->entity.collisionCircle.y - player->arme.displayRect.h / 2) + 2;

    Debug_PushRect(&player->arme.displayRect, 1, SDL_COLOR_RED);

    float angleEnDegres = -player->attackAngle * (180.0f / M_PI);
    SDL_Point pivot = {player->arme.displayRect.w / 2, player->arme.displayRect.h / 2 + player->arme.displayRect.h / 4};

    SDL_FPoint pivotWorldPos = {
        player->arme.displayRect.x + pivot.x,
        player->arme.displayRect.y + pivot.y};

    float crossSize = 3.0f;
    Debug_PushLine(
        pivotWorldPos.x - crossSize, pivotWorldPos.y,
        pivotWorldPos.x + crossSize, pivotWorldPos.y,
        1,
        SDL_COLOR_PINK);
    Debug_PushLine(
        pivotWorldPos.x, pivotWorldPos.y - crossSize,
        pivotWorldPos.x, pivotWorldPos.y + crossSize,
        1,
        SDL_COLOR_PINK);

    SDL_RenderCopyEx(renderer, player->arme.texture, NULL, &player->arme.displayRect, -angleEnDegres, &pivot, SDL_FLIP_NONE);

    if (player->entity.debug && player->isAttacking) {
        Debug_PushCircle(player->arme.attackHitbox.x, player->arme.attackHitbox.y, player->arme.attackHitbox.radius, SDL_COLOR_PINK);
        Debug_PushLine(player->entity.collisionCircle.x, player->entity.collisionCircle.y, player->entity.collisionCircle.x + cosf(player->attackAngle) * 40, player->entity.collisionCircle.y - sinf(player->attackAngle) * 40, 3, SDL_COLOR_TURQUOISE);
    }
}

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    if (player->isAttacking) {
        player->arme.attackTimer += *deltaTime;

        float weaponLength = 10.0f;
        player->arme.attackHitbox.x = player->entity.collisionCircle.x + cosf(player->attackAngle) * weaponLength;
        player->arme.attackHitbox.y = player->entity.collisionCircle.y - sinf(player->attackAngle) * weaponLength;

        if (player->arme.attackTimer >= player->arme.attackDuration) {
            player->isAttacking = SDL_FALSE;
        }
    }
    updatePhysicEntity(&player->entity, deltaTime, grid, entities);
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime) {
    float force = 400.0f;
    float force_dash = force * 3.5;

    float mouseWorldX = 0.0f, mouseWorldY = 0.0f;
    convertMouseToWorld(vp, input->mouseX, input->mouseY, &mouseWorldX, &mouseWorldY);
    float playerX = player->entity.collisionCircle.x;
    float playerY = player->entity.collisionCircle.y;
    float dirX = mouseWorldX - playerX;
    float dirY = mouseWorldY - playerY;

    player->attackAngle = atan2f(-dirY, dirX);

    if (input->key[player->control->left]) {
        player->entity.physics.velocity.x -= force * *deltaTime;
        player->entity.flip = SDL_FLIP_HORIZONTAL;
    }
    if (input->key[player->control->right]) {
        player->entity.physics.velocity.x += force * *deltaTime;
        player->entity.flip = SDL_FLIP_NONE;
    }
    if (input->key[player->control->up]) player->entity.physics.velocity.y -= force * *deltaTime;
    if (input->key[player->control->down]) player->entity.physics.velocity.y += force * *deltaTime;

    if (input->mouseButtons[SDL_BUTTON_RIGHT]) {
        float magnitude = sqrt(dirX * dirX + dirY * dirY);
        if (magnitude > 0.0f) {
            dirX /= magnitude;
            dirY /= magnitude;
        }

        player->entity.physics.velocity.x += dirX * force_dash * *deltaTime;
        player->entity.physics.velocity.y += dirY * force_dash * *deltaTime;
    }

    if (input->mouseButtons[SDL_BUTTON_LEFT]) {
        player->isAttacking = SDL_TRUE;
        player->arme.attackTimer = 0.0f;
    }

    if (player->entity.physics.velocity.x != 0.0f || player->entity.physics.velocity.y != 0.0f) {
        setAnimation(player->entity.animationController, "walk");
    } else {
        setAnimation(player->entity.animationController, "idle");
    }
}

void freePlayer(void* object) {
    t_joueur* player = (t_joueur*)object;
    SDL_DestroyTexture(player->entity.texture);
    free(player);
}
