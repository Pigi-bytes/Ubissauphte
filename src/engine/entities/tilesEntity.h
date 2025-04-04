#ifndef TILES_ENTITY_H
#define TILES_ENTITY_H

#include <SDL2/SDL.h>

#include "../../utils/fscene.h"
#include "../tileset.h"
#include "components/equipment/equipementsManager.h"
#include "components/physic/physics.h"
#include "entity.h"
#include "player.h"
#include "tiles.h"

typedef struct s_tileEntity {
    t_entity entity;

    void (*update)(struct s_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);
    void (*render)(struct s_tileEntity* entity, t_context* context, t_camera* camera);

    SDL_bool isDestructible;
} t_tileEntity;

void initTileEntityBase(t_tileEntity* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene);

void updateTileEntity(t_tileEntity* tileEntity, t_context* context, t_salle* grid, t_objectManager* entities);
void renderTileEntity(t_tileEntity* tileEntity, t_context* context, t_camera* camera);
void freeTileEntity(void* object);

void replaceTileWithEntity(t_tile* tile, int x, int y, t_tileEntity* entity, t_objectManager* entities, uint8_t entityTypeId, SDL_Texture* floorTexture);
void processSpecialTiles(t_grid* grid, t_tileset* tileset, t_objectManager* entities, uint8_t entityTypeId, t_scene* scene, t_context* context);

void renderTileEntityWrapper(t_fonctionParam* f);
void updateTileEntityWrapper(t_fonctionParam* f);

#endif