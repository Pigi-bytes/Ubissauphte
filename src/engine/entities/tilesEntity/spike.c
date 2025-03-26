#include "spike.h"

void updateSpike(t_tileEntity* entity, t_context* context, t_grid* grid, t_objectManager* entities) {
    t_spike* spike = (t_spike*)entity;
    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (spike->isActive) {
        uint8_t enemyTypeId = getTypeIdByName(entities->registry, "ENEMY");
        SDL_bool enemiesRemaining = SDL_FALSE;

        for (int i = 0; i < entities->count; i++) {
            void* obj = getObject(entities, i);
            if (obj && getObjectTypeId(entities, i) == enemyTypeId) {
                enemiesRemaining = SDL_TRUE;
                break;
            }
        }

        if (!enemiesRemaining) {
            spike->isActive = SDL_FALSE;
            entity->entity.useCircleCollision = SDL_FALSE;
            printf("Les pièges se désactivent, Vous pouvez passer à travers.\n");
            setAnimation(spike->base.entity.animationController, "bas");
        }
    }

    SDL_bool playerTouchingNow = checkCircleCircleCollision(&entity->entity.collisionCircle, &player->entity.collisionCircle, NULL);

    if (playerTouchingNow) {
        if (spike->isActive) {
            printf("Spike damaged player for %.1f points\n", spike->damage);
            applyDamage(&player->health, 10, &player->entity, NULL);
        } else if (!spike->messageShown) {
            printf("*WHOOSH* Teleportation \n");
            spike->messageShown = SDL_TRUE;
        }
    } else {
        spike->playerTouching = SDL_FALSE;
    }

    spike->playerTouching = playerTouchingNow;
    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, grid, entities);
}

void renderSpike(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_spike* spike = (t_spike*)entity;

    renderEntity(context->renderer, &entity->entity, camera);
}

t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene) {
    t_spike* spike = malloc(sizeof(t_spike));
    memset(spike, 0, sizeof(t_spike));

    initTileEntityBase(&spike->base, getObject(tileset->textureTiles, 42), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(spike->base.entity.animationController, createAnimation(tileset, (int[]){42}, 1, 240, SDL_TRUE, "haut"));
    addAnimation(spike->base.entity.animationController, createAnimation(tileset, (int[]){42, 137, 137, 136}, 4, 600, SDL_FALSE, "bas"));
    setAnimation(spike->base.entity.animationController, "haut");

    spike->damage = 10;
    spike->isActive = SDL_TRUE;
    spike->messageShown = SDL_FALSE;
    spike->playerTouching = SDL_FALSE;

    spike->base.update = updateSpike;
    spike->base.render = renderSpike;

    return (t_tileEntity*)spike;
}