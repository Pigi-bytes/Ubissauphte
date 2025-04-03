#ifndef BLACKSMITH_H
#define BLACKSMITH_H

#include "../../../ui/text.h"
#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    t_circle detectionRange;
    SDL_bool playerInRange;
    SDL_bool interactKeyPressed;
    SDL_Keycode lastInteractKey;
    t_text* interactText;
} t_blacksmith;


void updateBlacksmith(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);
void renderBlacksmith(t_tileEntity* entity, t_context* context, t_camera* camera);
t_tileEntity* createBlacksmithEntity(t_tileset* tileset, t_scene* scene);

#endif