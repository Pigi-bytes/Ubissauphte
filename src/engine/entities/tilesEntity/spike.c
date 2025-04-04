#include "spike.h"
int determineSpikeDirection(t_grid* grid, t_entity* entity) {
    float centerX = grid->width * 16 / 2.0f;
    float centerY = grid->height * 16 / 2.0f;

    float spikeX = entity->collisionCircle.x;
    float spikeY = entity->collisionCircle.y;

    float dx = (spikeX - centerX) / centerX;
    float dy = (spikeY - centerY) / centerY;

    if (fabsf(dx) > fabsf(dy)) {
        return (dx > 0) ? 1 : 3;  // Droite (1) ou Gauche (3)
    } else {
        return (dy > 0) ? 2 : 0;  // Bas (2) ou Haut (0)
    }
}

void placeNearTeleporter(t_grid* grid, t_entity* entity, t_objectManager* entities, t_entity* teleporter, t_grid* lastGrid) {
    // Déterminer la direction du spike dans l'ancienne salle
    int exitDirection = determineSpikeDirection(lastGrid, teleporter);

    // Trouver le spike correspondant dans la nouvelle salle
    uint8_t spikeTypeId = getTypeIdByName(entities->registry, "SPIKE");
    t_entity* targetSpike = NULL;

    for (int i = 0; i < entities->count; i++) {
        void* currentEntity = getObject(entities, i);
        if (currentEntity == NULL) continue;

        if (getObjectTypeId(entities, i) == spikeTypeId) {
            t_entity* spike = (t_entity*)currentEntity;
            int spikeDir = determineSpikeDirection(grid, spike);

            // Trouver le spike dans la direction opposée
            if ((exitDirection == 1 && spikeDir == 3) ||  // Sortie droite -> entrée gauche
                (exitDirection == 3 && spikeDir == 1) ||  // Sortie gauche -> entrée droite
                (exitDirection == 0 && spikeDir == 2) ||  // Sortie haut -> entrée bas
                (exitDirection == 2 && spikeDir == 0)) {  // Sortie bas -> entrée haut
                targetSpike = spike;
                break;
            }
        }
    }

    // Si on a trouvé un spike correspondant, placer le joueur à côté
    if (targetSpike != NULL) {
        float spawnX = targetSpike->collisionCircle.x;
        float spawnY = targetSpike->collisionCircle.y;
        int entryDirection = determineSpikeDirection(grid, targetSpike);

        // Décalage en fonction de la direction d'entrée
        switch (entryDirection) {
            case 1:  // Entrée par droite -> spawn à gauche
                spawnX -= 16;
                break;
            case 3:  // Entrée par gauche -> spawn à droite
                spawnX += 16;
                break;
            case 0:  // Entrée par haut -> spawn en bas
                spawnY += 16;
                break;
            case 2:  // Entrée par bas -> spawn en haut
                spawnY -= 16;
                break;
        }

        // Vérifier et appliquer la position
        int gridX = (int)(spawnX / 16);
        int gridY = (int)(spawnY / 16);

        t_tile* tile = getTile(grid, gridX, gridY, 0);
        if (tile && !tile->solide) {
            // Vérifier les collisions
            SDL_bool collisionFound = SDL_FALSE;
            for (int i = 0; i < entities->count; i++) {
                t_entity* other = getObject(entities, i);
                if (!other || other == entity) continue;

                float dx = spawnX - other->collisionCircle.x;
                float dy = spawnY - other->collisionCircle.y;
                float distanceSq = dx * dx + dy * dy;
                float minDist = entity->collisionCircle.radius + other->collisionCircle.radius;

                if (distanceSq < minDist * minDist) {
                    collisionFound = SDL_TRUE;
                    break;
                }
            }

            if (!collisionFound) {
                entity->collisionCircle.x = spawnX;
                entity->collisionCircle.y = spawnY;
                entity->displayRect.x = spawnX - entity->displayRect.w / 2;
                entity->displayRect.y = spawnY - entity->displayRect.h / 2;
                return;
            }
        }
    }
    // Fallback: placement aléatoire si on n'a pas trouvé de spike correspondant
    placeOnRandomTile(grid, entity, entities);
}

void updateSpike(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities) {
    t_spike* spike = (t_spike*)entity;
    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (!spike->playerTouching && spike->messageShown) {
        spike->messageShown = SDL_FALSE;
    }

    // Déterminer la direction et la salle liée si ce n'est pas déjà fait
    if (spike->direction == -1) {
        spike->direction = determineSpikeDirection(salle->grille, &entity->entity);

        // Associer la salle appropriée selon la direction
        switch (spike->direction) {
            case 0:
                spike->linkedRoom = salle->haut;
                break;
            case 1:
                spike->linkedRoom = salle->droite;
                break;
            case 2:
                spike->linkedRoom = salle->bas;
                break;
            case 3:
                spike->linkedRoom = salle->gauche;
                break;
        }
    }

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
            setAnimation(spike->base.entity.animationController, "bas");
        }
    }

    SDL_bool playerTouchingNow = checkCircleCircleCollision(&entity->entity.collisionCircle,
                                                            &player->entity.collisionCircle,
                                                            NULL);

    if (playerTouchingNow) {
        if (spike->isActive) {
            applyDamage(&player->health, 10, &player->entity, NULL);
        } else if (!spike->messageShown) {
            spike->messageShown = SDL_TRUE;

            // Téléportation vers la salle liée
            context->sceneController->currentScene =
                indiceByscene(context->sceneController, spike->linkedRoom->scene);

            t_joueur* joueur = (t_joueur*)getObject(entities, 0);
            joueur->indexCurrentRoom = spike->linkedRoom->ID;
            placeNearTeleporter(spike->linkedRoom->grille, &joueur->entity, spike->linkedRoom->entities, &spike->base.entity, salle->grille);
        }
    } else {
        spike->playerTouching = SDL_FALSE;
    }

    spike->playerTouching = playerTouchingNow;
    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, salle->grille, entities);
}

void renderSpike(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_spike* spike = (t_spike*)entity;
    renderEntity(context->renderer, &entity->entity, camera);
}

t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene) {
    t_spike* spike = malloc(sizeof(t_spike));
    memset(spike, 0, sizeof(t_spike));

    initTileEntityBase(&spike->base, getObject(tileset->textureTiles, 42), (SDL_Rect){0, 0, 16, 16}, scene);

    // Configurer les animations
    addAnimation(spike->base.entity.animationController,
                 createAnimation(tileset, (int[]){42}, 1, 240, SDL_TRUE, "haut"));
    addAnimation(spike->base.entity.animationController,
                 createAnimation(tileset, (int[]){42, 137, 137, 136}, 4, 600, SDL_FALSE, "bas"));
    setAnimation(spike->base.entity.animationController, "haut");

    // Initialisation des propriétés du spike
    spike->damage = 10;
    spike->isActive = SDL_TRUE;
    spike->messageShown = SDL_FALSE;
    spike->playerTouching = SDL_FALSE;
    spike->direction = -1;  // Non déterminée au départ
    spike->linkedRoom = NULL;

    spike->base.update = updateSpike;
    spike->base.render = renderSpike;

    return (t_tileEntity*)spike;
}