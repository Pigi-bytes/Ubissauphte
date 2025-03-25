#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <SDL2/SDL.h>

#include "../../utils/fscene.h"
#include "../tileset.h"
#include "components/physic/physics.h"  // Include physics.h before defining t_entity
#include "entity.h"
#include "player.h"
#include "tiles.h"

typedef struct t_tileEntity {
    t_entity entity;

    void (*update)(struct t_tileEntity*, float*, t_grid*, t_objectManager*, t_input*);
    void (*render)(SDL_Renderer*, struct t_tileEntity*, t_camera*);

    SDL_bool isDestructible;
} t_tileEntity;

typedef struct {
    t_tileEntity base;
    t_circle detectionRange;
    float baseDetectionRadius;
    SDL_bool isOpen;
    SDL_bool playerInRange;
    SDL_bool interactKeyPressed;
} t_chest;

typedef struct {
    t_tileEntity base;
    float damage;
    SDL_bool isActive;
    SDL_bool messageShown;
    SDL_bool playerTouching;
} t_spike;

typedef struct {
    t_tileEntity base;
    int health;
    SDL_bool isExploding;
} t_barrel;

void initTileEntityBase(t_tileEntity* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene);

void updateTileEntity(t_tileEntity* tileEntity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);
void renderTileEntity(SDL_Renderer* renderer, t_tileEntity* tileEntity, t_camera* camera);
void freeTileEntity(void* object);

t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene);
t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene);
t_tileEntity* createBarrelEntity(t_tileset* tileset, t_scene* scene);

void updateChest(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);
void updateSpike(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);
void updateBarrel(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);

void renderChest(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera);
void renderSpike(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera);
void renderBarrel(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera);

void replaceTileWithEntity(t_tile* tile, int x, int y, t_tileEntity* entity, t_objectManager* entities, uint8_t entityTypeId, SDL_Texture* floorTexture);
void processSpecialTiles(t_grid* grid, t_tileset* tileset, t_objectManager* entities, uint8_t entityTypeId, t_scene* scene);

void renderTileEntityWrapper(t_fonctionParam* f);
void updateTileEntityWrapper(t_fonctionParam* f);

#endif