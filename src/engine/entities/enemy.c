#include "enemy.h"

void onEnemyDeath(t_context* context, void* entity) {
    t_enemy* enemy = (t_enemy*)entity;

    enemy->entity.useCircleCollision = SDL_FALSE;

    printf("Ennemi détruit !\n");
}

void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene) {
    base->entity.texture = texture;
    base->entity.displayRect = rect;
    base->entity.flip = SDL_FLIP_NONE;
    base->entity.animationController = initAnimationController();

    base->entity.useCircleCollision = SDL_TRUE;
    base->entity.collisionCircle.x = rect.x + rect.w / 2;
    base->entity.collisionCircle.y = rect.y + rect.h / 2;
    base->entity.collisionCircle.radius = fminf(rect.w, rect.h) / 2;

    // Initialise le système de santé
    initHealthSystem(&base->health, 100);  // Santé par défaut
    base->health.healthBarDuration = 3.0f;
    base->health.flashDuration = 0.17f;
    base->health.invincibilityDuration = 1.0f;
    base->health.onDeathCallback = onEnemyDeath;
    base->health.healthBareRender = renderStandardHealthBar;

    // Associer la scène
    base->entity.currentScene = scene;
}

void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera) {
    if (enemy->health.isDead) {
        return;
    }

    if (enemy->health.isFlashing) {
        // Effet blanc pur - tout en blanc sans aucune autre couleur
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
        // Utiliser le mode additif pour assurer une luminosité maximale
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(enemy->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(enemy->entity.texture, 255, 255, 255);
    }

    if (!(enemy->health.isInvincible &&
          fmodf(getDeltaTimer(enemy->health.invincibilityTimer), 0.1) < 0.05)) {
        renderEntity(renderer, &enemy->entity, camera);
    }

    if (enemy->health.showHealthBar && enemy->health.healthBareRender) {
        enemy->health.healthBareRender(renderer, &enemy->health, enemy->entity.displayRect, camera);
    }
}

void updateEnemy(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    if (enemy->health.isDead) {
        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == enemy) {
                deleteObject(entities, i);
                return;
            }
        }
        return;
    }

    updateHealthSystem(&enemy->health, *deltaTime);

    if (enemy->update) {
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

void takeDamageAndCheckDeath(t_enemy* enemy, int damage, t_context* context) {
    applyDamage(&enemy->health, damage, enemy, context);
}