#include "enemy.h"

#include "player.h"

void onEnemyDeath(t_context* context, void* entity) {
    t_enemy* enemy = (t_enemy*)entity;

    enemy->entity.useCircleCollision = SDL_FALSE;

    printf("Ennemi détruit !\n");
    if (enemy->lastDamagedBy != NULL) {
        addPlayerXP(enemy->lastDamagedBy, enemy->xpReward);
    }
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

    base->xpReward = 10;
    base->lastDamagedBy = NULL;
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

void updateEnemy(t_enemy* enemy, float* deltaTime, t_salle* salle, t_objectManager* entities) {
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
        enemy->update(enemy, deltaTime, salle->grille, entities);
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

void takeDamageFromPlayer(t_enemy* enemy, int damage, t_joueur* player, t_context* context) {
    enemy->lastDamagedBy = player;

    applyDamage(&enemy->health, damage, enemy, context);
}

void renderEnemyWrapper(t_fonctionParam* f) {
    renderEnemy(((SDL_Renderer*)(f)->param[0]), ((t_enemy*)(f)->param[1]), ((t_camera*)(f)->param[2]));
}
void updateEnemyWrapper(t_fonctionParam* f) {
    updateEnemy(((t_enemy*)(f)->param[0]), ((float*)(f)->param[1]), ((t_salle*)(f)->param[2]), ((t_objectManager*)(f)->param[3]));
}