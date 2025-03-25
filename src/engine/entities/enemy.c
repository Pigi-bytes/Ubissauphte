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

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene) {
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

    base->flashTimer = initDeltaTimer();  // Timer pour la barre de vie
    startDeltaTimer(base->flashTimer);    // Timer pour la barre de vie
    base->flashDuration = 0.17f;

    base->isDead = SDL_FALSE;

    // Associer la scène
    base->entity.currentScene = scene;
}

void renderHealthBar(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    float healthRatio = (float)enemy->health / (float)enemy->maxHealth;
    SDL_Rect enemyPos = enemy->entity.displayRect;

    float barWidth = enemyPos.w * 0.8f;
    float barHeight = 3.0f;
    float barX = enemyPos.x + (enemyPos.w - barWidth) / 2;
    float barY = enemyPos.y - 12.0f;
    float fillWidth = barWidth * healthRatio;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &(SDL_Rect){barX, barY + 1, barWidth, barHeight});

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 180);
    SDL_RenderFillRect(renderer, &(SDL_Rect){barX, barY, barWidth, barHeight});

    float pulseIntensity = 0.85f + (sinf(SDL_GetTicks() * 0.005f) * 0.15f);

    Uint8 r = 0, g = 0, b = 0;
    if (healthRatio > 0.75f) {
        // Bleu/turquoise -> vert
        r = 0;
        g = 255;
        b = (Uint8)(255 * (1.0f - (healthRatio - 0.75f) * 4.0f));
    } else if (healthRatio > 0.5f) {
        // Vert -> jaune
        r = (Uint8)(255 * (healthRatio - 0.5f) * 4.0f);
        g = 255;
        b = 0;
    } else if (healthRatio > 0.25f) {
        // Jaune -> orange
        r = 255;
        g = (Uint8)(255 * (healthRatio - 0.25f) * 4.0f);
        b = 0;
    } else {
        // Orange -> rouge
        r = 255;
        g = (Uint8)(128 * healthRatio * 4.0f);
        b = 0;
    }

    SDL_SetRenderDrawColor(renderer, (Uint8)(r * pulseIntensity), (Uint8)(g * pulseIntensity), (Uint8)(b * pulseIntensity), 255);
    SDL_RenderFillRect(renderer, &(SDL_Rect){barX, barY, fillWidth, barHeight});

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    if (enemy->isDead) {
        return;
    }

    if (enemy->isFlashing) {
        // Effet blanc pur - tout en blanc sans aucune autre couleur
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
        // Utiliser le mode additif pour assurer une luminosité maximale
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
    }

    if (!(enemy->isInvincible && fmodf(getDeltaTimer(enemy->invincibilityTimer), 0.1) < 0.05)) {
        renderEntity(renderer, &enemy->entity, camera);
    }

    // Le reste du code pour la barre de vie reste inchangé
    if (enemy->showHealthBar) {
        renderHealthBar(renderer, enemy, camera);
    }
}

void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    if (enemy->isDead) {
        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == enemy) {
                deleteObject(entities, i);
            }
        }
        return;
    }

    if (enemy->isFlashing) {
        updateDeltaTimer(enemy->flashTimer, *deltaTime);

        if (getDeltaTimer(enemy->flashTimer) >= enemy->flashDuration) {
            enemy->isFlashing = SDL_FALSE;
        }
    }

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

    if (enemy->health > 0 && enemy->update) {
        enemy->update(enemy, deltaTime, grid, entities);
    }
}

void freeEnemy(void* object) {
    t_enemy* enemy = (t_enemy*)object;
    if (enemy != NULL) {
        free(enemy);
    }
    // SDL_DestroyTexture(enemy->entity.texture);
    // freeAnimationController(enemy->entity.animationController);
}

void takeDamageAndCheckDeath(t_enemy* enemy, int damage) {
    if (!enemy || enemy->isInvincible || enemy->isDead) return;  // Ignorer si déjà mort

    enemy->showHealthBar = SDL_TRUE;
    resetDeltaTimer(enemy->healthBarTimer);

    // Activer l'effet de flash
    enemy->isFlashing = SDL_TRUE;
    resetDeltaTimer(enemy->flashTimer);

    enemy->health -= damage;
    if (enemy->health <= 0) {
        enemy->health = 0;
        enemy->showHealthBar = SDL_FALSE;

        // Marquer l'ennemi comme mort
        enemy->isDead = SDL_TRUE;

        // Désactiver les collisions pour qu'il n'interagisse plus avec le monde
        enemy->entity.useCircleCollision = SDL_FALSE;

        // Les graphismes et la suppression seront gérés par updateEnemy
        return;
    }

    enemy->isInvincible = SDL_TRUE;
    resetDeltaTimer(enemy->invincibilityTimer);
}