#include "barrel.h"

void onBarrelDestroy(void* entity) {
    t_barrel* barrel = (t_barrel*)entity;
    barrel->isExploding = SDL_TRUE;
    printf("Barrel destroyed!\n");
}

void takeDamageBarrel(t_tileEntity* entity, float damage) {
    t_barrel* barrel = (t_barrel*)entity;
    if (barrel->base.isDestructible) {
        applyDamage(&barrel->health, (int)damage, barrel);
    }
}

void updateBarrel(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities) {
    t_barrel* barrel = (t_barrel*)entity;

    updateHealthSystem(&barrel->health, context->frameData->deltaTime);

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player && player->attack.isActive && !barrel->isExploding) {
        float currentAngle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, smoothStepf(player->attack.progress));

        SDL_FPoint barrelPos = {entity->entity.collisionCircle.x, entity->entity.collisionCircle.y};

        if (cercleInSector(barrelPos, entity->entity.collisionCircle.radius, player->attack.hitBox.origin, currentAngle, player->currentWeapon->range, player->currentWeapon->angleAttack)) {
            takeDamageBarrel(entity, player->currentWeapon->damage);

            player->attack.nbHits++;

            float dx = barrelPos.x - player->attack.hitBox.origin.x;
            float dy = barrelPos.y - player->attack.hitBox.origin.y;
            player->attack.hit_distance = sqrtf(dx * dx + dy * dy);
        }
    }

    if (barrel->isExploding) {
        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == barrel) {
                deleteObject(entities, i);
            }
        }
        return;
    }

    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, salle->grille, entities);
}

void renderBarrel(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_barrel* barrel = (t_barrel*)entity;

    if (barrel->isExploding) {
        SDL_FPoint screenPos = {entity->entity.collisionCircle.x, entity->entity.collisionCircle.y};

        SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(context->renderer, 139, 69, 19, 200);

        const int NUM_FRAGMENTS = 12;
        for (int i = 0; i < NUM_FRAGMENTS; i++) {
            float angle = (i * (2 * M_PI / NUM_FRAGMENTS));
            float distance = 8 + (i % 3) * 3;  // Vary the distance

            int x1 = screenPos.x + cosf(angle) * distance;
            int y1 = screenPos.y + sinf(angle) * distance;
            int x2 = x1 + cosf(angle) * (3 + (i % 4));
            int y2 = y1 + sinf(angle) * (3 + (i % 4));

            SDL_RenderDrawLine(context->renderer, x1, y1, x2, y2);
        }

        SDL_SetRenderDrawColor(context->renderer, 210, 180, 140, 120);
        int dustRadius = 12;
        for (int w = -dustRadius; w <= dustRadius; w++) {
            for (int h = -dustRadius; h <= dustRadius; h++) {
                if (w * w + h * h <= dustRadius * dustRadius) {
                    if ((w + h) % 3 == 0) {
                        SDL_RenderDrawPoint(context->renderer, screenPos.x + w, screenPos.y + h);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(context->renderer, 160, 82, 45, 230);
        const int NUM_CHUNKS = 6;
        for (int i = 0; i < NUM_CHUNKS; i++) {
            float angle = (i * (2 * M_PI / NUM_CHUNKS)) + (M_PI / NUM_CHUNKS);
            float distance = 5 + (i % 3) * 2;

            int x = screenPos.x + cosf(angle) * distance;
            int y = screenPos.y + sinf(angle) * distance;

            SDL_Rect chunk = {x - 2, y - 2, 3 + (i % 3), 3 + (i % 2)};
            SDL_RenderFillRect(context->renderer, &chunk);
        }

        SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_NONE);
        return;
    }

    if (barrel->health.isFlashing) {
        SDL_SetTextureColorMod(entity->entity.texture, 255, 255, 255);
        SDL_SetTextureBlendMode(entity->entity.texture, SDL_BLENDMODE_ADD);
    } else {
        SDL_SetTextureBlendMode(entity->entity.texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureColorMod(entity->entity.texture, 255, 255, 255);
    }

    renderEntity(context->renderer, &entity->entity, camera);

    if (barrel->health.showHealthBar && barrel->health.healthBareRender) {
        barrel->health.healthBareRender(context->renderer, &barrel->health, entity->entity.displayRect, camera);
    }
}

t_tileEntity* createBarrelEntity(t_tileset* tileset, t_scene* scene) {
    t_barrel* barrel = malloc(sizeof(t_barrel));
    memset(barrel, 0, sizeof(t_barrel));

    initTileEntityBase(&barrel->base, getObject(tileset->textureTiles, 83), (SDL_Rect){0, 0, 16, 16}, scene);
    barrel->base.entity.animationController->haveAnimation = SDL_FALSE;

    barrel->base.isDestructible = SDL_TRUE;

    initHealthSystem(&barrel->health, 50);
    barrel->health.healthBarDuration = 2.5f;
    barrel->health.flashDuration = 0.1f;
    barrel->health.invincibilityDuration = 0.3f;
    barrel->health.onDeathCallback = onBarrelDestroy;

    barrel->health.healthBareRender = renderStandardHealthBar;

    barrel->isExploding = SDL_FALSE;

    barrel->base.update = updateBarrel;
    barrel->base.render = renderBarrel;

    return (t_tileEntity*)barrel;
}