#include "tilesEntity.h"

#include "tilesEntity/barrel.h"
#include "tilesEntity/blacksmith.h"
#include "tilesEntity/chest.h"
#include "tilesEntity/merchant.h"
#include "tilesEntity/spike.h"
#include "tilesEntity/wizard.h"

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

void updateTileEntity(t_tileEntity* tileEntity, t_context* context, t_salle* salle, t_objectManager* entities) {
    if (tileEntity && tileEntity->update) {
        tileEntity->update(tileEntity, context, salle, entities);
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


    addObject(entities, entity, entityTypeId);
}

void processSpecialTiles(t_grid* grid, t_tileset* tileset, t_objectManager* entities, uint8_t entityTypeId, t_scene* scene, t_context* context) {
    SDL_Texture* floorTexture = getObject(tileset->textureTiles, 49);
    int spikeCount = 0, chestCount = 0, barrelCount = 0;

    if (!scene) {
        printf("ERROR: Scene reference is NULL\n");
        return;
    }

    SDL_Texture* spikeTexture = getObject(tileset->textureTiles, 42);
    SDL_Texture* chestTexture = getObject(tileset->textureTiles, 90);
    SDL_Texture* barrelTexture = getObject(tileset->textureTiles, 83);
    SDL_Texture* merchantTexture = getObject(tileset->textureTiles, 86);
    SDL_Texture* blacksmithTexture = getObject(tileset->textureTiles, 87);
    SDL_Texture* wizardTexture = getObject(tileset->textureTiles, 85);

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
                    } else if (tileTexture == merchantTexture) {
                        t_tileEntity* merchantEntity = createMerchantEntity(tileset, scene, context);
                        replaceTileWithEntity(tile, x, y, merchantEntity, entities, getTypeIdByName(entities->registry, "MERCHANT"), floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, merchantEntity, entityTypeId);
                    } else if (tileTexture == blacksmithTexture) {
                        t_tileEntity* blacksmithEntity = createBlacksmithEntity(tileset, scene, context);
                        replaceTileWithEntity(tile, x, y, blacksmithEntity, entities, getTypeIdByName(entities->registry, "BLACKSMITH"), floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, blacksmithEntity, entityTypeId);
                    } else if (tileTexture == wizardTexture) {
                        t_tileEntity* wizardEntity = createWizardEntity(tileset, scene, context);
                        replaceTileWithEntity(tile, x, y, wizardEntity, entities, getTypeIdByName(entities->registry, "WIZARD"), floorTexture);
                        ADD_OBJECT_TO_SCENE(scene, wizardEntity, entityTypeId);
                    }
                }
            }
        }
    }
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
    t_salle* salle = GET_PTR(f, 2, t_salle*);
    t_objectManager* entities = GET_PTR(f, 3, t_objectManager*);

    updateTileEntity(entity, context, salle, entities);
}