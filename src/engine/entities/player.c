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

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, true, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){3, 4}, 2, 240, true, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

    return joueur;
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    renderEntity(renderer, &player->entity, camera);
}

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    updatePhysicEntity(&player->entity, deltaTime, grid, entities);
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, float* deltaTime) {
    float force = 10.0f;
    float force_dash = force * 3.5;

    // Reset de l'accelaration a chaque frame
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

    if (player->entity.physics.acceleration.x != 0.0f || player->entity.physics.acceleration.y != 0.0f) {
        setAnimation(player->entity.animationController, "walk");
    } else {
        setAnimation(player->entity.animationController, "idle");
    }

    // INPUT_LOG("Accélération finale: (%.1f,%.1f)", player->entity.physics.acceleration.x, player->entity.physics.acceleration.y);
}

void freePlayer(void* object) {
    t_joueur* player = (t_joueur*)object;
    SDL_DestroyTexture(player->entity.texture);
    free(player);
}
