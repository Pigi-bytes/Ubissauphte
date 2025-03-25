#ifndef SPIKE_H
#define SPIKE_H

#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    float damage;
    SDL_bool isActive;
    SDL_bool messageShown;
    SDL_bool playerTouching;
} t_spike;

t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene);
void updateSpike(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);
void renderSpike(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera);

#endif