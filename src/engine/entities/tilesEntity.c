#include "tilesEntity.h"

// Initialize base tile entity properties
void initTileEntityBase(t_tileEntity* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene) {
    base->entity.texture = texture;
    base->entity.displayRect = rect;
    base->entity.flip = SDL_FLIP_NONE;
    base->entity.animationController = initAnimationController();
    base->entity.animationController->haveAnimation = SDL_FALSE;
    base->entity.debug = SDL_TRUE;

    base->entity.useCircleCollision = SDL_FALSE;
    base->entity.collisionRect = rect;

    base->entity.physics.mass = 1.0;
    base->entity.physics.friction = 0.5;
    base->entity.physics.restitution = 0.1;
    base->entity.physics.velocity.x = 0;
    base->entity.physics.velocity.y = 0;

    base->isInteractable = SDL_FALSE;
    base->isDestructible = SDL_FALSE;

    base->entity.currentScene = scene;
}

void updateTileEntity(t_tileEntity* tileEntity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    if (tileEntity && tileEntity->update) {
        tileEntity->update(tileEntity, deltaTime, grid, entities);
    }
}

void renderTileEntity(SDL_Renderer* renderer, t_tileEntity* tileEntity, t_camera* camera) {
    if (tileEntity && tileEntity->render) {
        tileEntity->render(renderer, tileEntity, camera);
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

    spike->damage = 10;
    spike->isActive = SDL_TRUE;

    spike->base.entity.physics.mass = 0;
    spike->base.entity.physics.friction = 1.0;
    spike->base.entity.physics.restitution = 0.0;

    spike->base.update = updateSpike;
    spike->base.render = renderSpike;

    return (t_tileEntity*)spike;
}

t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene) {
    t_chest* chest = malloc(sizeof(t_chest));
    memset(chest, 0, sizeof(t_chest));

    initTileEntityBase(&chest->base, getObject(tileset->textureTiles, 90), (SDL_Rect){0, 0, 16, 16}, scene);

    chest->base.isInteractable = SDL_TRUE;
    chest->baseDetectionRadius = 2 * 16;
    chest->detectionRange.radius = chest->baseDetectionRadius;
    chest->detectionRange.x = 8;
    chest->detectionRange.y = 8;
    chest->isOpen = SDL_FALSE;
    chest->playerInRange = SDL_FALSE;

    chest->base.entity.physics.mass = 5.0;
    chest->base.entity.physics.friction = 0.9;
    chest->base.entity.physics.restitution = 0.1;

    chest->base.update = updateChest;
    chest->base.render = renderChest;

    return (t_tileEntity*)chest;
}

t_tileEntity* createBarrelEntity(t_tileset* tileset, t_scene* scene) {
    t_barrel* barrel = malloc(sizeof(t_barrel));
    memset(barrel, 0, sizeof(t_barrel));

    initTileEntityBase(&barrel->base, getObject(tileset->textureTiles, 83), (SDL_Rect){0, 0, 16, 16}, scene);

    barrel->base.isDestructible = SDL_TRUE;
    barrel->health = 50;
    barrel->isExploding = SDL_FALSE;

    barrel->base.entity.physics.mass = 2.0;
    barrel->base.entity.physics.friction = 0.7;
    barrel->base.entity.physics.restitution = 0.3;

    barrel->base.update = updateBarrel;
    barrel->base.render = renderBarrel;

    return (t_tileEntity*)barrel;
}

void updateChest(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_chest* chest = (t_chest*)entity;

    chest->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    chest->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_entity* player = getObject(entities, 0);
    if (player) {
        chest->playerInRange = checkCircleCircleCollision(&chest->detectionRange, &player->collisionCircle, NULL);

        if (entity->entity.debug) {
            Debug_PushCircle(chest->detectionRange.x, chest->detectionRange.y, chest->detectionRange.radius, chest->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }

    updatePhysicEntity(&entity->entity, deltaTime, grid, entities);
}

void updateSpike(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_spike* spike = (t_spike*)entity;

    t_entity* player = getObject(entities, 0);  // Assuming player is at index 0
    if (player && spike->isActive) {
        SDL_bool collision = SDL_FALSE;

        if (entity->entity.useCircleCollision) {
            collision = checkCircleCircleCollision(&entity->entity.collisionCircle, &player->collisionCircle, NULL);
        } else {
            collision = checkCircleRectCollision(&player->collisionCircle, &entity->entity.collisionRect, NULL);
        }

        if (collision) {
            printf("Spike damaged player for %d points\n", spike->damage);
        }
    }

    updatePhysicEntity(&entity->entity, deltaTime, grid, entities);
}

void updateBarrel(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    t_barrel* barrel = (t_barrel*)entity;

    if (barrel->isExploding) {
        printf("Barrel exploding!\n");

        for (int i = 0; i < entities->count; i++) {
            if (getObject(entities, i) == entity) {
                deleteObject(entities, i);
                return;
            }
        }
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
    }

    renderEntity(renderer, &entity->entity, camera);
}

void replaceTileWithEntity(t_tile* tile, int x, int y, t_tileEntity* entity, t_objectManager* entities, uint8_t entityTypeId, SDL_Texture* floorTexture) {
    int tileX = x * 16;
    int tileY = y * 16;

    tile->entity.texture = floorTexture;
    tile->solide = SDL_FALSE;

    if (entity->entity.useCircleCollision) {
        entity->entity.collisionCircle.x = tileX + 8;
        entity->entity.collisionCircle.y = tileY + 8;
    } else {
        entity->entity.collisionRect = (SDL_Rect){.x = tileX, .y = tileY, .h = 16, .w = 16};
    }

    entity->entity.displayRect.x = tileX;
    entity->entity.displayRect.y = tileY;

    printf("Placing entity at tile (%d,%d) => coords (%d,%d)\n", x, y, tileX, tileY);

    addObject(entities, entity, getTypeIdByName(entities->registry, "ENTITY"));
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
                        replaceTileWithEntity(tile, x, y, spikeEntity, entities, entityTypeId, floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, spikeEntity, entityTypeId);
                        spikeCount++;
                    } else if (tileTexture == chestTexture) {
                        t_tileEntity* chestEntity = createChestEntity(tileset, scene);
                        replaceTileWithEntity(tile, x, y, chestEntity, entities, entityTypeId, floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, chestEntity, entityTypeId);
                        chestCount++;
                    } else if (tileTexture == barrelTexture) {
                        t_tileEntity* barrelEntity = createBarrelEntity(tileset, scene);
                        replaceTileWithEntity(tile, x, y, barrelEntity, entities, entityTypeId, floorTexture);
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

    updateTileEntity(entity, deltaTime, grid, entities);
}