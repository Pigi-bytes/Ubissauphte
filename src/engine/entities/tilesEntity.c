#include "tilesEntity.h"

#include "tilesEntity/barrel.h"
#include "tilesEntity/chest.h"
#include "tilesEntity/spike.h"

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

void updateTileEntity(t_tileEntity* tileEntity, t_context* context, t_grid* grid, t_objectManager* entities) {
    if (tileEntity && tileEntity->update) {
        tileEntity->update(tileEntity, context, grid, entities);
    }
}

void renderTileEntity(t_tileEntity* tileEntity, t_context* context, t_camera* camera) {
    if (tileEntity && tileEntity->render) {
        tileEntity->render(tileEntity, context, camera);
    }
}

void freeTileEntity(void* object) {
    t_tileEntity* tileEntity = (t_tileEntity*)object;
    if (tileEntity != NULL) {
        free(tileEntity);
    }
}

void replaceTileWithEntity(t_tile* tile, int x, int y, t_tileEntity* entity, t_objectManager* entities, uint8_t entityTypeId, SDL_Texture* floorTexture) {
    int tileX = x * 16;
    int tileY = y * 16;

    tile->entity.texture = floorTexture;
    tile->solide = SDL_FALSE;

    entity->entity.collisionCircle.x = tileX + 8;
    entity->entity.collisionCircle.y = tileY + 8;
    entity->entity.collisionCircle.radius = 8;

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
    t_context* context = GET_PTR(f, 0, t_context*);
    t_tileEntity* entity = GET_PTR(f, 1, t_tileEntity*);
    t_camera* camera = GET_PTR(f, 2, t_camera*);

    renderTileEntity(entity, context, camera);
}

void updateTileEntityWrapper(t_fonctionParam* f) {
    t_context* context = GET_PTR(f, 0, t_context*);
    t_tileEntity* entity = GET_PTR(f, 1, t_tileEntity*);
    t_grid* grid = GET_PTR(f, 2, t_grid*);
    t_objectManager* entities = GET_PTR(f, 3, t_objectManager*);

    updateTileEntity(entity, context, grid, entities);
}