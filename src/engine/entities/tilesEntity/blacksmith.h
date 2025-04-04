#ifndef BLACKSMITH_H
#define BLACKSMITH_H

#include "../../../ui/affichage.h"
#include "../../../ui/text.h"
#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    t_circle detectionRange;
    SDL_bool playerInRange;
    SDL_bool interactKeyPressed;
    SDL_Keycode lastInteractKey;
    t_item *randomItem;
    t_text *interactText, *item;
    int prix;
} t_blacksmith;

void updateBlacksmith(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);
void renderBlacksmith(t_tileEntity* entity, t_context* context, t_camera* camera);
t_tileEntity* createBlacksmithEntity(t_tileset* tileset, t_scene* scene, t_context* context);

#endif