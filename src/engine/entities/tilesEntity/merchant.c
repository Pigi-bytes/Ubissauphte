#include "merchant.h"


void updateMerchant(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities){
    t_merchant* merchant = (t_merchant*)entity;

    merchant->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    merchant->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        merchant->playerInRange = checkCircleCircleCollision(&merchant->detectionRange, &player->entity.collisionCircle, NULL);

        if (merchant->lastInteractKey != player->control->interact) {
            merchant->lastInteractKey = player->control->interact;

            char interactPrompt[64];
            char interactPrompt2[64];
            sprintf(interactPrompt, "%s", merchant->randomItem->name);
            sprintf(interactPrompt2, "[%s] to buy for %d coins", SDL_GetKeyName(SDL_GetKeyFromScancode(merchant->lastInteractKey)), merchant->prix);

            if (merchant->interactText) {
                updateText(&merchant->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 0, 255});
            } else {
                merchant->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 0, 255});
            }

            if (merchant->item) {
                updateText(&merchant->item, context->renderer, interactPrompt2, (SDL_Color){50, 150, 255, 255});
            } else {
                merchant->item = createText(context->renderer, interactPrompt2, context->gameFont, (SDL_Color){50, 150, 255, 255});
            }
        }

        if (merchant->playerInRange) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !merchant->interactKeyPressed) {
                if (player->gold >= merchant->prix) {
                    jouerSFX("assets/barrel.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);
                    player->gold -= merchant->prix;
                    if (merchant->randomItem) {
                        inventaireAjoutObjet(player->inventaire, merchant->randomItem, 1);
                    } 
                    equipementRecalculerStats(&player);
                }else{
                }

                // changer scène ici
            }

            merchant->interactKeyPressed = interactKeyIsPressed;
        } else {
            merchant->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(merchant->detectionRange.x, merchant->detectionRange.y, merchant->detectionRange.radius, merchant->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }
}

void renderMerchant(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_merchant* merchant = (t_merchant*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (merchant->playerInRange && merchant->interactText) {
        merchant->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (merchant->interactText->rect.w / 2);
        merchant->interactText->rect.y = entity->entity.displayRect.y - merchant->interactText->rect.h - 5;

        renderText(context->renderer, merchant->interactText);
    }

    if (merchant->playerInRange && merchant->item) {
        merchant->item->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (merchant->item->rect.w / 2);
        merchant->item->rect.y = entity->entity.displayRect.y - merchant->item->rect.h * 2 - 5;

        renderText(context->renderer, merchant->item);
    }
}

t_tileEntity* createMerchantEntity(t_tileset* tileset, t_scene* scene, t_context* context){
    t_merchant* merchant = malloc(sizeof(t_merchant));
    memset(merchant, 0, sizeof(t_merchant));
    initTileEntityBase(&merchant->base, getObject(tileset->textureTiles, 86), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(merchant->base.entity.animationController, createAnimation(tileset, (int[]){86}, 1, 240, SDL_TRUE, "idle"));
    setAnimation(merchant->base.entity.animationController, "idle");

    merchant->detectionRange.radius = 2 * 20;
    merchant->detectionRange.x = 10;
    merchant->detectionRange.y = 10;
    merchant->playerInRange = SDL_FALSE;

    merchant->interactKeyPressed = SDL_FALSE;
    merchant->lastInteractKey = 0;

    merchant->randomItem = getItemByRarity(context->itemListe, context->nbItem, RARITY_LEGENDARY);

    merchant->interactText = NULL;

    switch (context->difficulty) {
        case EASY:
            merchant->prix = 200;
            break;
        case NORMAL:
            merchant->prix = 1000;
            break;
        case HARD:
            merchant->prix = 1500;
            break;
        case DEMONIC:
            merchant->prix = 2000;
            break;
        case LEGEND:
            merchant->prix = 3000;
            break;
    }

    merchant->base.update = updateMerchant;
    merchant->base.render = renderMerchant;


    return (t_tileEntity*)merchant;
}

