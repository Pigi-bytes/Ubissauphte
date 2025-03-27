#ifndef MERCHANT_H
#define MERCHANT_H

#include "../../../ui/text.h"
#include "../tilesEntity.h"

typedef struct {
    t_tileEntity base;
    t_circle detectionRange;
    SDL_bool playerInRange;
    SDL_bool interactKeyPressed;
    SDL_Keycode lastInteractKey;
    t_text* interactText;
} t_merchant;


void updateMerchant(t_tileEntity* entity, t_context* context, t_grid* grid, t_objectManager* entities);
void renderMerchant(t_tileEntity* entity, t_context* context, t_camera* camera);
t_tileEntity* createMerchantEntity(t_tileset* tileset, t_scene* scene);

#endif