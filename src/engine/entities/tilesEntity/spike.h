#ifndef SPIKE_H
#define SPIKE_H

#include "../../../context.h"
#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    float damage;
    SDL_bool isActive;
    SDL_bool messageShown;
    SDL_bool playerTouching;
    int direction;
    t_salle* linkedRoom;
} t_spike;

t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene);
void updateSpike(t_tileEntity* entity, t_context* context, t_salle* sale, t_objectManager* entities);
void renderSpike(t_tileEntity* entity, t_context* context, t_camera* camera);

#endif