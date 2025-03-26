#ifndef BARREL_H
#define BARREL_H

#include "../../../utils/timer.h"
#include "../components/health/health.h"
#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    t_healthSystem health;
    SDL_bool isExploding;
} t_barrel;

t_tileEntity* createBarrelEntity(t_tileset* tileset, t_scene* scene);
void updateBarrel(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);
void renderBarrel(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera);
void takeDamageBarrel(t_tileEntity* entity, float damage);
void onBarrelDestroy(void* entity);

#endif