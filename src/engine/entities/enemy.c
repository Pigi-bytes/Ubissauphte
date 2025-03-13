#include "enemy.h"

t_ennemi* createEnemy(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_ennemi* enemy = (t_ennemi*)malloc(sizeof(t_ennemi));

    enemy->entity.texture = texture;
    enemy->entity.displayRect = rect;
    enemy->entity.flip = SDL_FLIP_NONE;
    enemy->entity.animationController = initAnimationController();
    enemy->entity.debug = SDL_TRUE;

    enemy->entity.useCircleCollision = SDL_TRUE;
    enemy->entity.collisionCircle.x = rect.x + rect.w / 2;
    enemy->entity.collisionCircle.y = rect.y + rect.h / 2;
    enemy->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;

    enemy->wanderTimer = NULL;
    startTimer(enemy->wanderTimer);

    t_physics enemyPhysics = {
        .velocity = {0, 0},
        .acceleration = {0.0f, 0.0f},
        .mass = 0.8f,  // Masse nulle = statique
        .friction = 0.03f,
        .restitution = 0.2f};

    enemy->entity.physics = enemyPhysics;

    addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){2, 3}, 2, 240, true, "idle"));
    addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){1, 2, 3, 2}, 4, 150, true, "walk"));
    setAnimation(enemy->entity.animationController, "walk");

    return enemy;
}

void renderEnemy(SDL_Renderer* renderer, t_ennemi* enemy, t_camera* camera) {
    renderEntity(renderer, &enemy->entity, camera);
}

void updateEnemy(t_ennemi* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    const float MIN_DIRECTION_TIME = 5.0f;         // Temps minimum avant changement de direction
    const float MAX_DIRECTION_TIME = 10.0f;        // Temps maximum avant changement de direction
    const float MAX_ACCELERATION = 5.0f;           // Accélération maximale
    const float ACCELERATION_CHANGE_SPEED = 1.0f;  // Vitesse de transition

    

    if (getTicks(enemy->wanderTimer) / 1000.0f >= MIN_DIRECTION_TIME + (rand() % (int)(MAX_DIRECTION_TIME - MIN_DIRECTION_TIME))) {
        // Génère une nouvelle accélération aléatoire
        float targetAccelX = ((float)rand() / RAND_MAX) * 2 * MAX_ACCELERATION - MAX_ACCELERATION;
        float targetAccelY = ((float)rand() / RAND_MAX) * 2 * MAX_ACCELERATION - MAX_ACCELERATION;

        // Clamp les valeurs pour sécurité
        targetAccelX = fmaxf(-MAX_ACCELERATION, fminf(MAX_ACCELERATION, targetAccelX));
        targetAccelY = fmaxf(-MAX_ACCELERATION, fminf(MAX_ACCELERATION, targetAccelY));

        // Interpolation progressive vers la nouvelle accélération
        enemy->targetAcceleration.x = targetAccelX;
        enemy->targetAcceleration.y = targetAccelY;

        // Réinitialise le timer
        resetTimer(enemy->wanderTimer);
    }

    // Interpolation progressive de l'accélération actuelle vers la cible
    enemy->entity.physics.acceleration.x += (enemy->targetAcceleration.x - enemy->entity.physics.acceleration.x) * ACCELERATION_CHANGE_SPEED * *deltaTime;
    enemy->entity.physics.acceleration.y += (enemy->targetAcceleration.y - enemy->entity.physics.acceleration.y) * ACCELERATION_CHANGE_SPEED * *deltaTime;

    // Met à jour la physique de l'entité
    updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
}

void freeEnemy(void* object) {
    t_ennemi* enemy = (t_ennemi*)object;
    SDL_DestroyTexture(enemy->entity.texture);
    // freeAnimationController(enemy->entity.animationController);
    free(enemy);
}