#include "enemy.h"

// t_enemy* createEnemy(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
//     t_enemy* enemy = (t_enemy*)malloc(sizeof(t_enemy));

//     enemy->entity.texture = texture;
//     enemy->entity.displayRect = rect;
//     enemy->entity.flip = SDL_FLIP_NONE;
//     enemy->entity.animationController = initAnimationController();
//     enemy->entity.debug = SDL_TRUE;

//     enemy->entity.useCircleCollision = SDL_TRUE;
//     enemy->entity.collisionCircle.x = rect.x + rect.w / 2;
//     enemy->entity.collisionCircle.y = rect.y + rect.h / 2;
//     enemy->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;

//     enemy->wanderTimer = initTimer();
//     startTimer(enemy->wanderTimer);

//     // switch (type) {
//     //     case SLIME:
//     //         enemy->entity.physics = (t_physics){.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 1.0f, .friction = 0.07f, .restitution = 1.0f};
//     //         addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){1, 2}, 2, 240, true, "idle"));
//     //         addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){1, 2, 1, 3}, 4, 240, true, "walk"));
//     //         setAnimation(enemy->entity.animationController, "walk");
//     //         break;

//     //     case CRABE:
//     //         enemy->entity.physics = (t_physics){.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 10.0f, .friction = 0.1f, .restitution = 0.05f};
//     //         addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){1, 2, 3, 2, 3, 2}, 6, 240, true, "idle"));
//     //         addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){4, 1, 5}, 3, 150, true, "walk"));
//     //         setAnimation(enemy->entity.animationController, "walk");
//     //         break;

// void updateSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
//     t_slime* slime = (t_slime*)enemy;

//     const float MIN_DIRECTION_TIME = 5.0f;         // Temps minimum avant changement de direction
//     const float MAX_DIRECTION_TIME = 10.0f;        // Temps maximum avant changement de direction
//     const float MAX_ACCELERATION = 5.0f;           // Accélération maximale
//     const float ACCELERATION_CHANGE_SPEED = 1.0f;  // Vitesse de transition

//     if (getTicks(enemy->wanderTimer) / 1000.0f >= MIN_DIRECTION_TIME + (rand() % (int)(MAX_DIRECTION_TIME - MIN_DIRECTION_TIME))) {
//         // Génère une nouvelle accélération aléatoire
//         float targetAccelX = ((float)rand() / RAND_MAX) * 2 * MAX_ACCELERATION - MAX_ACCELERATION;
//         float targetAccelY = ((float)rand() / RAND_MAX) * 2 * MAX_ACCELERATION - MAX_ACCELERATION;

//         // Clamp les valeurs pour sécurité
//         targetAccelX = fmaxf(-MAX_ACCELERATION, fminf(MAX_ACCELERATION, targetAccelX));
//         targetAccelY = fmaxf(-MAX_ACCELERATION, fminf(MAX_ACCELERATION, targetAccelY));

//         // Interpolation progressive vers la nouvelle accélération
//         enemy->targetAcceleration.x = targetAccelX;
//         enemy->targetAcceleration.y = targetAccelY;

//         // Réinitialise le timer
//         resetTimer(enemy->wanderTimer);
//     }

//     // Interpolation progressive de l'accélération actuelle vers la cible
//     enemy->entity.physics.acceleration.x += (enemy->targetAcceleration.x - enemy->entity.physics.acceleration.x) * ACCELERATION_CHANGE_SPEED * *deltaTime;
//     enemy->entity.physics.acceleration.y += (enemy->targetAcceleration.y - enemy->entity.physics.acceleration.y) * ACCELERATION_CHANGE_SPEED * *deltaTime;

//     // Met à jour la physique de l'entité
//     updatePhysicEntity(&enemy->entity, deltaTime, grid, entities);
// }
//     //     case FANTOME:
//     //         enemy->entity.physics = (t_physics){.velocity = {0, 0}, .acceleration = {0.0f, 0.0f}, .mass = 0.01f, .friction = 0.01f, .restitution = 0.1f};
//     //         addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){2, 3}, 2, 240, true, "idle"));
//     //         addAnimation(enemy->entity.animationController, createAnimation(tileset, (int[]){1, 2, 3, 2}, 4, 150, true, "walk"));
//     //         setAnimation(enemy->entity.animationController, "walk");
//     //         break;

//     //     default:
//     //         break;
//     // }

//     setAnimation(enemy->entity.animationController, "walk");

//     return enemy;
// }

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect) {
    base->entity.texture = texture;
    base->entity.displayRect = rect;
    base->entity.flip = SDL_FLIP_NONE;
    base->entity.animationController = initAnimationController();

    base->entity.useCircleCollision = SDL_TRUE;
    base->entity.collisionCircle.x = rect.x + rect.w / 2;
    base->entity.collisionCircle.y = rect.y + rect.h / 2;
    base->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;

    base->showHealthBar = SDL_FALSE;          // La barre de vie est cachée par défaut
    base->healthBarTimer = initDeltaTimer();  // Timer pour la barre de vie
    startDeltaTimer(base->healthBarTimer);    // Timer pour la barre de vie
}

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    if (enemy->isInvincible) {
        // Alterner entre rouge et blanc toutes les 0.1 seconde
        float timer = getDeltaTimer(enemy->invincibilityTimer);
        if ((int)(timer * 10) % 2 == 0) {
            SDL_SetTextureColorMod(enemy->entity.texture, 255, 0, 0);  // Rouge
        } else {
            SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);  // Blanc
        }
    } else {
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);  // Couleur normale
    }

    renderEntity(renderer, &enemy->entity, camera);

    if (enemy->showHealthBar) {
        SDL_Rect enemyPos = enemy->entity.displayRect;  // Position de l'ennemi
        float healthRatio = (float)enemy->health / (float)enemy->maxHealth;

        float barWidth = enemy->entity.displayRect.w;  // Largeur de la barre de vie
        float barHeight = 5.0f;                        // Hauteur de la barre de vie
        float barX = enemyPos.x;                       // Centrer la barre au-dessus de l'ennemi
        float barY = enemyPos.y - 20.0f;               // Position Y de la barre (au-dessus de l'ennemi)

        // Dessiner le fond de la barre de vie (en rouge)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge
        SDL_Rect backgroundRect = {barX, barY, barWidth, barHeight};
        SDL_RenderFillRect(renderer, &backgroundRect);

        // Dessiner la partie remplie de la barre de vie (en vert)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Vert
        SDL_Rect healthRect = {barX, barY, barWidth * healthRatio, barHeight};
        SDL_RenderFillRect(renderer, &healthRect);
    }
}

void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    if (enemy->isInvincible) {
        updateDeltaTimer(enemy->invincibilityTimer, *deltaTime);

        if (getDeltaTimer(enemy->invincibilityTimer) >= enemy->invincibilityDuration) {
            enemy->isInvincible = SDL_FALSE;
        }
    }

    if (enemy->showHealthBar) {
        updateDeltaTimer(enemy->healthBarTimer, *deltaTime);

        if (getDeltaTimer(enemy->healthBarTimer) >= 3.0f) {
            enemy->showHealthBar = SDL_FALSE;
        }
    }

    if (enemy->health != 0 && enemy->update) {
        enemy->update(enemy, deltaTime, grid, entities);
    }
}

void freeEnemy(void* object) {
    t_enemy* enemy = (t_enemy*)object;
    SDL_DestroyTexture(enemy->entity.texture);
    // freeAnimationController(enemy->entity.animationController);
    free(enemy);
}

void takeDamage(t_enemy* enemy, int damage) {
    if (enemy->isInvincible) return;  // Si l'ennemi est invincible, on ne fait rien

    enemy->showHealthBar = SDL_TRUE;
    resetDeltaTimer(enemy->healthBarTimer);

    enemy->health -= damage;
    if (enemy->health < 0) {
        enemy->health = 0;
        enemy->showHealthBar = SDL_FALSE;
    }
    printf("%d \n", enemy->health);

    // Activer l'invincibilité
    enemy->isInvincible = SDL_TRUE;
    resetDeltaTimer(enemy->invincibilityTimer);
}