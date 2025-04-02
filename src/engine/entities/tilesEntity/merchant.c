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

            char interactPrompt[32];
            sprintf(interactPrompt, "[%s] to speak", SDL_GetKeyName(SDL_GetKeyFromScancode(merchant->lastInteractKey)));

            if (merchant->interactText) {
                updateText(&merchant->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 255, 255});
            } else {
                merchant->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 255, 255});
            }
        }

        if (merchant->playerInRange) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !merchant->interactKeyPressed) {
                jouerSFX("assets/hitWood.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);

                // changer scène ici

                printf("Parle au marchant\n");
            }

            merchant->interactKeyPressed = interactKeyIsPressed;
        } else {
            merchant->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(merchant->detectionRange.x, merchant->detectionRange.y, merchant->detectionRange.radius, merchant->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }

    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, salle->grille, entities);
}

void renderMerchant(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_merchant* merchant = (t_merchant*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (merchant->playerInRange && merchant->interactText) {
        merchant->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (merchant->interactText->rect.w / 2);
        merchant->interactText->rect.y = entity->entity.displayRect.y - merchant->interactText->rect.h - 5;

        renderText(context->renderer, merchant->interactText);
    }
}

t_tileEntity* createMerchantEntity(t_tileset* tileset, t_scene* scene) {
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
    merchant->interactText = NULL;

    merchant->base.update = updateMerchant;
    merchant->base.render = renderMerchant;

    return (t_tileEntity*)merchant;
}

