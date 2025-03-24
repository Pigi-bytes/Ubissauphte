#include "tilesEntity.h"

void initTileEntityBase(t_tileEntity* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene) {
    base->entity.texture = texture;
    base->entity.displayRect = rect;
    base->entity.flip = SDL_FLIP_NONE;
    base->entity.animationController = initAnimationController();
    base->entity.debug = SDL_TRUE;

    base->entity.useCircleCollision = SDL_TRUE;
    base->entity.collisionCircle.x = rect.x + rect.w / 2;
    base->entity.collisionCircle.y = rect.y + rect.h / 2;
    base->entity.collisionCircle.radius = rect.w / 2;

    base->entity.physics.mass = INFINITY;
    base->entity.physics.friction = 0;
    base->entity.physics.restitution = 0;
    base->entity.physics.velocity.x = 0;
    base->entity.physics.velocity.y = 0;

    base->isDestructible = SDL_FALSE;

    base->entity.currentScene = scene;
}

void updateTileEntity(t_tileEntity* tileEntity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input) {
    if (tileEntity && tileEntity->update) {
        tileEntity->update(tileEntity, deltaTime, grid, entities, input);
    }
}

void renderTileEntity(SDL_Renderer* renderer, t_tileEntity* tileEntity, t_camera* camera) {
    if (tileEntity && tileEntity->render) {
        tileEntity->render(renderer, tileEntity, camera);
    }
}

void takeDamageBarrel(t_tileEntity* entity, float damage) {
    t_barrel* barrel = (t_barrel*)entity;
    if (barrel->base.isDestructible) {
        barrel->health -= damage;
        if (barrel->health <= 0) {
            barrel->isExploding = SDL_TRUE;
            printf("Barrel destroyed!\n");
        }
    }
}

void freeTileEntity(void* object) {
    t_tileEntity* tileEntity = (t_tileEntity*)object;
    if (tileEntity != NULL) {
        free(tileEntity);
    }
}

t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene) {
    t_spike* spike = malloc(sizeof(t_spike));
    memset(spike, 0, sizeof(t_spike));

    initTileEntityBase(&spike->base, getObject(tileset->textureTiles, 42), (SDL_Rect){0, 0, 16, 16}, scene);
    spike->base.entity.animationController->haveAnimation = SDL_FALSE;

    spike->damage = 10;
    spike->isActive = SDL_TRUE;

    spike->base.update = updateSpike;
    spike->base.render = renderSpike;

    return (t_tileEntity*)spike;
}

t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene) {
    t_chest* chest = malloc(sizeof(t_chest));
    memset(chest, 0, sizeof(t_chest));

    initTileEntityBase(&chest->base, getObject(tileset->textureTiles, 90), (SDL_Rect){0, 0, 16, 16}, scene);
    chest->base.entity.animationController->haveAnimation = SDL_FALSE;

    addAnimation(chest->base.entity.animationController, createAnimation(tileset, (int[]){90}, 1, 240, SDL_TRUE, "close"));
    addAnimation(chest->base.entity.animationController, createAnimation(tileset, (int[]){90, 91, 92}, 3, 240, SDL_FALSE, "open"));
    setAnimation(chest->base.entity.animationController, "close");

    chest->baseDetectionRadius = 2 * 16;
    chest->detectionRange.radius = chest->baseDetectionRadius;
    chest->detectionRange.x = 8;
    chest->detectionRange.y = 8;
    chest->isOpen = SDL_FALSE;
    chest->playerInRange = SDL_FALSE;

    chest->interactKeyPressed = SDL_FALSE;

    chest->base.update = updateChest;
    chest->base.render = renderChest;

    return (t_tileEntity*)chest;
}

t_tileEntity* createBarrelEntity(t_tileset* tileset, t_scene* scene) {
    t_barrel* barrel = malloc(sizeof(t_barrel));
    memset(barrel, 0, sizeof(t_barrel));

    initTileEntityBase(&barrel->base, getObject(tileset->textureTiles, 83), (SDL_Rect){0, 0, 16, 16}, scene);
    barrel->base.entity.animationController->haveAnimation = SDL_FALSE;

    barrel->base.isDestructible = SDL_TRUE;
    barrel->health = 50;
    barrel->isExploding = SDL_FALSE;

    barrel->base.update = updateBarrel;
    barrel->base.render = renderBarrel;

    return (t_tileEntity*)barrel;
}

void updateChest(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input) {
    t_chest* chest = (t_chest*)entity;

    chest->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    chest->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        chest->playerInRange = checkCircleCircleCollision(&chest->detectionRange, &player->entity.collisionCircle, NULL);

        if (chest->playerInRange && !chest->isOpen) {
            SDL_bool interactKeyIsPressed = keyPressOnce(input, player->control->interact);

            if (interactKeyIsPressed && !chest->interactKeyPressed) {
                chest->isOpen = SDL_TRUE;
                setAnimation(chest->base.entity.animationController, "open");
                printf("Coffre ouvert ! Vous avez trouvé un trésor !\n");
            }

            chest->interactKeyPressed = interactKeyIsPressed;
        } else {
            chest->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(chest->detectionRange.x, chest->detectionRange.y, chest->detectionRange.radius, chest->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }

    updatePhysicEntity(&entity->entity, deltaTime, grid, entities);
}

void updateSpike(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input) {
    t_spike* spike = (t_spike*)entity;

    t_entity* player = getObject(entities, 0);  // Assuming player is at index 0
    if (player && spike->isActive) {
        SDL_bool collision = checkCircleCircleCollision(&entity->entity.collisionCircle, &player->collisionCircle, NULL);

        if (collision) {
            printf("Spike damaged player for %d points\n", spike->damage);
        }
    }

    updatePhysicEntity(&entity->entity, deltaTime, grid, entities);
}

void updateBarrel(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input) {
    t_barrel* barrel = (t_barrel*)entity;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player && player->attack.isActive && !barrel->isExploding) {
        float currentAngle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, smoothStepf(player->attack.progress));

        SDL_FPoint barrelPos = {entity->entity.collisionCircle.x, entity->entity.collisionCircle.y};

        if (cercleInSector(barrelPos, entity->entity.collisionCircle.radius, player->attack.hitBox.origin, currentAngle, player->currentWeapon->range, player->currentWeapon->angleAttack)) {
            takeDamageBarrel(entity, barrel->health);

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

    updatePhysicEntity(&entity->entity, deltaTime, grid, entities);
}

void renderChest(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera) {
    t_chest* chest = (t_chest*)entity;

    if (chest->isOpen) {
    }

    renderEntity(renderer, &entity->entity, camera);

    if (entity->entity.debug) {
    }
}

void renderSpike(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera) {
    t_spike* spike = (t_spike*)entity;

    if (!spike->isActive) {
    }

    renderEntity(renderer, &entity->entity, camera);
}

void renderBarrel(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera) {
    t_barrel* barrel = (t_barrel*)entity;

    if (barrel->isExploding) {
        // Get screen position
        SDL_FPoint screenPos = {entity->entity.collisionCircle.x, entity->entity.collisionCircle.y};

        // Draw wooden debris effect
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Small splinters/fragments (brown)
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 200);  // Darker wood color

        // Draw some random wooden fragments flying outward
        const int NUM_FRAGMENTS = 12;
        for (int i = 0; i < NUM_FRAGMENTS; i++) {
            float angle = (i * (2 * M_PI / NUM_FRAGMENTS));
            float distance = 8 + (i % 3) * 3;  // Vary the distance

            int x1 = screenPos.x + cosf(angle) * distance;
            int y1 = screenPos.y + sinf(angle) * distance;
            int x2 = x1 + cosf(angle) * (3 + (i % 4));
            int y2 = y1 + sinf(angle) * (3 + (i % 4));

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }

        // Inner dust cloud (light brown/beige)
        SDL_SetRenderDrawColor(renderer, 210, 180, 140, 120);  // Light wood dust color
        int dustRadius = 12;
        for (int w = -dustRadius; w <= dustRadius; w++) {
            for (int h = -dustRadius; h <= dustRadius; h++) {
                if (w * w + h * h <= dustRadius * dustRadius) {
                    // Only draw some points for a more sparse, dust-like effect
                    if ((w + h) % 3 == 0) {
                        SDL_RenderDrawPoint(renderer, screenPos.x + w, screenPos.y + h);
                    }
                }
            }
        }

        // Draw a few larger wood chunks
        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 230);  // Medium wood color
        const int NUM_CHUNKS = 6;
        for (int i = 0; i < NUM_CHUNKS; i++) {
            float angle = (i * (2 * M_PI / NUM_CHUNKS)) + (M_PI / NUM_CHUNKS);
            float distance = 5 + (i % 3) * 2;

            int x = screenPos.x + cosf(angle) * distance;
            int y = screenPos.y + sinf(angle) * distance;

            // Draw small rectangles representing wood chunks
            SDL_Rect chunk = {x - 2, y - 2, 3 + (i % 3), 3 + (i % 2)};
            SDL_RenderFillRect(renderer, &chunk);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        return;
    }

    renderEntity(renderer, &entity->entity, camera);
}

void replaceTileWithEntity(t_tile* tile, int x, int y, t_tileEntity* entity, t_objectManager* entities, uint8_t entityTypeId, SDL_Texture* floorTexture) {
    int tileX = x * 16;
    int tileY = y * 16;

    tile->entity.texture = floorTexture;
    tile->solide = SDL_FALSE;

    // Toujours configurer la collision circulaire, puisque nous les utilisons maintenant pour toutes les entités
    entity->entity.collisionCircle.x = tileX + 8;  // Centre X
    entity->entity.collisionCircle.y = tileY + 8;  // Centre Y
    entity->entity.collisionCircle.radius = 8;     // Rayon (moitié de la largeur de la tuile)

    entity->entity.displayRect.x = tileX;
    entity->entity.displayRect.y = tileY;

    printf("Placing entity at tile (%d,%d) => coords (%d,%d) with circle collision\n", x, y, tileX, tileY);

    addObject(entities, entity, entityTypeId);
}

void processSpecialTiles(t_grid* grid, t_tileset* tileset, t_objectManager* entities, uint8_t entityTypeId, t_scene* scene) {
    SDL_Texture* floorTexture = getObject(tileset->textureTiles, 49);
    int spikeCount = 0, chestCount = 0, barrelCount = 0;

    if (!scene) {
        printf("ERROR: Scene reference is NULL\n");
        return;
    }

    SDL_Texture* spikeTexture = getObject(tileset->textureTiles, 42);
    SDL_Texture* chestTexture = getObject(tileset->textureTiles, 90);
    SDL_Texture* barrelTexture = getObject(tileset->textureTiles, 83);

    for (int z = 0; z < grid->depth; z++) {
        for (int y = 0; y < grid->height; y++) {
            for (int x = 0; x < grid->width; x++) {
                t_tile* tile = getTile(grid, x, y, z);
                if (!tile) continue;

                if (tile->entity.texture) {
                    SDL_Texture* tileTexture = tile->entity.texture;

                    if (tileTexture == spikeTexture) {
                        t_tileEntity* spikeEntity = createSpikeEntity(tileset, scene);
                        replaceTileWithEntity(tile, x, y, spikeEntity, entities, getTypeIdByName(entities->registry, "SPIKE"), floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, spikeEntity, entityTypeId);
                        spikeCount++;
                    } else if (tileTexture == chestTexture) {
                        t_tileEntity* chestEntity = createChestEntity(tileset, scene);
                        replaceTileWithEntity(tile, x, y, chestEntity, entities, getTypeIdByName(entities->registry, "CHEST"), floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, chestEntity, entityTypeId);
                        chestCount++;
                    } else if (tileTexture == barrelTexture) {
                        t_tileEntity* barrelEntity = createBarrelEntity(tileset, scene);
                        replaceTileWithEntity(tile, x, y, barrelEntity, entities, getTypeIdByName(entities->registry, "BARREL"), floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, barrelEntity, entityTypeId);
                        barrelCount++;
                    }
                }
            }
        }
    }
    printf("Summary: Placed %d spikes, %d chests, and %d barrels\n", spikeCount, chestCount, barrelCount);
}

void renderTileEntityWrapper(t_fonctionParam* f) {
    SDL_Renderer* renderer = GET_PTR(f, 0, SDL_Renderer*);
    t_tileEntity* entity = GET_PTR(f, 1, t_tileEntity*);
    t_camera* camera = GET_PTR(f, 2, t_camera*);

    renderTileEntity(renderer, entity, camera);
}

void updateTileEntityWrapper(t_fonctionParam* f) {
    float* deltaTime = GET_PTR(f, 0, float*);
    t_tileEntity* entity = GET_PTR(f, 1, t_tileEntity*);
    t_grid* grid = GET_PTR(f, 2, t_grid*);
    t_objectManager* entities = GET_PTR(f, 3, t_objectManager*);
    t_input* input = GET_PTR(f, 4, t_input*);

    updateTileEntity(entity, deltaTime, grid, entities, input);
}