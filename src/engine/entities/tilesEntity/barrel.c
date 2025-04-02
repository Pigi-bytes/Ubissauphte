#include "barrel.h"

void onBarrelDestroy(t_context* context, void* entity) {
    t_barrel* barrel = (t_barrel*)entity;
    barrel->isExploding = SDL_TRUE;

    jouerSFX("assets/barrel.wav", 100, 0, context->audioManager);

    SDL_FPoint position = {barrel->base.entity.collisionCircle.x, barrel->base.entity.collisionCircle.y};
    float radius = barrel->base.entity.collisionCircle.radius;

    emitBarrelExplosionParticles(barrel->particleEmitter, position, radius);

    if (barrel->lastDamagedBy != NULL) {
        barrel->lastDamagedBy->gold += barrel->goldReward;
        printf("Barrel destroyed! Player gained %d gold\n", barrel->goldReward);
    }
}

void takeDamageBarrel(t_tileEntity* entity, float damage, t_context* context) {
    t_barrel* barrel = (t_barrel*)entity;

    if (barrel->base.isDestructible && !barrel->isExploding) {
        applyDamage(&barrel->health, (int)damage, barrel, context);
    }
}

void updateBarrel(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities) {
    t_barrel* barrel = (t_barrel*)entity;

    if (barrel->health.isDead) {
        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == barrel) {
                deleteObject(entities, i);
                return;
            }
        }
        return;
    }

    updateEntityParticles(barrel->particleEmitter, context->frameData->deltaTime);

    updateHealthSystem(&barrel->health, context->frameData->deltaTime);

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player && player->attack.isActive && !barrel->isExploding) {
        float currentAngle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, smoothStepf(player->attack.progress));

        SDL_FPoint barrelPos = {entity->entity.collisionCircle.x, entity->entity.collisionCircle.y};

        if (cercleInSector(barrelPos, entity->entity.collisionCircle.radius, player->attack.hitBox.origin, currentAngle, player->currentWeapon->range, player->currentWeapon->angleAttack)) {
            barrel->lastDamagedBy = player;
            takeDamageBarrel(entity, player->currentWeapon->damage, context);

            SDL_FPoint hitDirection = {barrelPos.x - player->entity.collisionCircle.x, barrelPos.y - player->entity.collisionCircle.y};
            float length = sqrtf(hitDirection.x * hitDirection.x + hitDirection.y * hitDirection.y);
            if (length > 0) {
                hitDirection.x /= length;
                hitDirection.y /= length;
            }

            SDL_Color woodColor = {139, 69, 19, 220};
            emitImpactParticles(barrel->particleEmitter, barrelPos, hitDirection, entity->entity.collisionCircle.radius, woodColor);
            player->attack.nbHits++;

            float dx = barrelPos.x - player->attack.hitBox.origin.x;
            float dy = barrelPos.y - player->attack.hitBox.origin.y;
            player->attack.hit_distance = sqrtf(dx * dx + dy * dy);
        }
    }

    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, salle->grille, entities);
}

void renderBarrel(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_barrel* barrel = (t_barrel*)entity;

    renderEntityParticles(context->renderer, barrel->particleEmitter);
    if (barrel->health.isDead) return;

    if (!barrel->isExploding) {
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
    barrel->goldReward = 2 + rand() % 6;

    barrel->particleEmitter = createParticleEmitter();

    barrel->base.update = updateBarrel;
    barrel->base.render = renderBarrel;

    return (t_tileEntity*)barrel;
}