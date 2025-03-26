#ifndef CHEST_H
#define CHEST_H

#include "../../../ui/text.h"
#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    t_circle detectionRange;
    SDL_bool isOpen;
    SDL_bool playerInRange;
    SDL_bool interactKeyPressed;
    SDL_Keycode lastInteractKey;
    t_text* interactText;
} t_chest;

t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene);
void updateChest(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);
void renderChest(t_tileEntity* entity, t_context* context, t_camera* camera);

#endif