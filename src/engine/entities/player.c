#include "player.h"

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.displayRect = rect;
    joueur->entity.flip = SDL_FLIP_NONE;
    joueur->entity.animationController = initAnimationController();
    joueur->entity.debug = SDL_TRUE;

    joueur->entity.useCircleCollision = SDL_TRUE;
    joueur->entity.collisionCircle.x = rect.x + rect.w / 2;
    joueur->entity.collisionCircle.y = rect.y + rect.h / 2;
    joueur->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;  // Set radius to half the smaller dimension of the rect

    t_physics playerPhysics = {
        .velocity = {0, 0},  // Vitesse initiale
        .acceleration = {0.0f, 0.0f},
        .mass = 10.0f,
        .friction = 0.05f,
        .restitution = 0.02};

    joueur->entity.physics = playerPhysics;

    joueur->isAttacking = SDL_FALSE;
    joueur->attackDuration = 0.2f;  // 200ms
    joueur->attackTimer = 0.0f;
    joueur->attackHitbox.radius = 8.0f;  // Taille de la hitbox

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, true, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 240, true, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

    return joueur;
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    renderEntity(renderer, &player->entity, camera);

    if (player->entity.debug && player->isAttacking) {
        Debug_PushCircle(player->attackHitbox.x, player->attackHitbox.y, player->attackHitbox.radius, SDL_COLOR_PINK);
        Debug_PushLine(player->entity.collisionCircle.x, player->entity.collisionCircle.y, player->entity.collisionCircle.x + cosf(player->attackAngle) * 40, player->entity.collisionCircle.y - sinf(player->attackAngle) * 40, 3, SDL_COLOR_TURQUOISE);
    }
}

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    if (player->isAttacking) {
        player->attackTimer += *deltaTime;

        float weaponLength = 10.0f;
        player->attackHitbox.x = player->entity.collisionCircle.x + cosf(player->attackAngle) * weaponLength;
        player->attackHitbox.y = player->entity.collisionCircle.y - sinf(player->attackAngle) * weaponLength;

        if (player->attackTimer >= player->attackDuration) {
            player->isAttacking = SDL_FALSE;
        }
    }
    updatePhysicEntity(&player->entity, deltaTime, grid, entities);
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp) {
    float force = 10.0f;
    float force_dash = force * 3.5;

    player->entity.physics.acceleration.x = 0.0f;
    player->entity.physics.acceleration.y = 0.0f;

    if (input->key[player->control->left]) {
        player->entity.physics.acceleration.x -= force;
        player->entity.flip = SDL_FLIP_HORIZONTAL;
    }
    if (input->key[player->control->right]) {
        player->entity.physics.acceleration.x += force;
        player->entity.flip = SDL_FLIP_NONE;
    }
    if (input->key[player->control->up]) player->entity.physics.acceleration.y -= force;
    if (input->key[player->control->down]) player->entity.physics.acceleration.y += force;

    // Dash
    if (input->key[SDL_SCANCODE_SPACE]) {
        if (input->key[player->control->left]) player->entity.physics.acceleration.x -= force_dash;
        if (input->key[player->control->right]) player->entity.physics.acceleration.x += force_dash;
        if (input->key[player->control->up]) player->entity.physics.acceleration.y -= force_dash;
        if (input->key[player->control->down]) player->entity.physics.acceleration.y += force_dash;
    }

    if (input->mouseButtons[SDL_BUTTON_LEFT]) {
        printf("Attack \n");
        float mouseWorldX, mouseWorldY;
        convertMouseToWorld(vp, input->mouseX, input->mouseY, &mouseWorldX, &mouseWorldY);

        // Position du joueur dans le monde (pas écran !)
        float playerX = player->entity.collisionCircle.x;
        float playerY = player->entity.collisionCircle.y;

        // Vecteur relatif
        float relX = mouseWorldX - playerX;
        float relY = mouseWorldY - playerY;

        // Angle (inversion Y si nécessaire)
        player->attackAngle = atan2f(-relY, relX);
        player->isAttacking = SDL_TRUE;
        player->attackTimer = 0.0f;
    }

    if (player->entity.physics.acceleration.x != 0.0f || player->entity.physics.acceleration.y != 0.0f) {
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
