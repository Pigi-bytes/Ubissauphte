#ifndef CHEST_H
#define CHEST_H

#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    t_circle detectionRange;
    float baseDetectionRadius;
    SDL_bool isOpen;
    SDL_bool playerInRange;
    SDL_bool interactKeyPressed;
} t_chest;

t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene);
void updateChest(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input);
void renderChest(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera);

#endif