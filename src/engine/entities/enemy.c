#include "enemy.h"

t_ennemi* createEnemy(SDL_Texture* texture, SDL_Rect rect) {
    t_ennemi* enemy = (t_ennemi*)malloc(sizeof(t_ennemi));

    // Configuration de base
    enemy->entity.texture = texture;
    enemy->entity.displayRect = rect;
    enemy->entity.flip = SDL_FLIP_NONE;
    enemy->entity.animationController = initAnimationController();
    enemy->entity.debug = SDL_FALSE;

    // Configuration des collisions
    enemy->entity.useCircleCollision = SDL_TRUE;
    enemy->entity.collisionCircle.x = rect.x + rect.w / 2;
    enemy->entity.collisionCircle.y = rect.y + rect.h / 2;
    enemy->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;

    // Physique statique
    t_physics enemyPhysics = {
        .velocity = {0, 0},
        .acceleration = {0.0f, 0.0f},
        .mass = 0.8f,  // Masse nulle = statique
        .friction = 0.03f,
        .restitution = 0.2f};
    enemy->entity.physics = enemyPhysics;

    enemy->entity.animationController->haveAnimation = SDL_FALSE;

    return enemy;
}

void renderEnemy(SDL_Renderer* renderer, t_ennemi* enemy, t_camera* camera) {
    renderEntity(renderer, &enemy->entity, camera);
}

void freeEnemy(void* object) {
    t_ennemi* enemy = (t_ennemi*)object;
    SDL_DestroyTexture(enemy->entity.texture);
    // freeAnimationController(enemy->entity.animationController);
    free(enemy);
}