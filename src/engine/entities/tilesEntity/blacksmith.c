

#include "blacksmith.h"

void updateBlacksmith(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities) {
    t_blacksmith* blacksmith = (t_blacksmith*)entity;

    blacksmith->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    blacksmith->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        blacksmith->playerInRange = checkCircleCircleCollision(&blacksmith->detectionRange, &player->entity.collisionCircle, NULL);

        if (blacksmith->lastInteractKey != player->control->interact) {
            blacksmith->lastInteractKey = player->control->interact;

            char interactPrompt[32];
            char interactPrompt2[32];
            sprintf(interactPrompt, "*%s* for %d coins", "Golden Axe", 100);
            sprintf(interactPrompt2, "[%s] to buy", SDL_GetKeyName(SDL_GetKeyFromScancode(blacksmith->lastInteractKey)));

            if (blacksmith->interactText) {
                updateText(&blacksmith->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 0, 255});
            } else {
                blacksmith->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 0, 255});
            }

            if (blacksmith->item) {
                updateText(&blacksmith->item, context->renderer, interactPrompt2, (SDL_Color){50, 150, 255, 255});
            } else {
                blacksmith->item = createText(context->renderer, interactPrompt2, context->gameFont, (SDL_Color){50, 150, 255, 255});
            }
        }

        t_item** liste = context->itemListe;

        if (blacksmith->playerInRange) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !blacksmith->interactKeyPressed) {
                printf("*Parle au forgeron*\n");
                if (player->gold >= 5000) {
                    jouerSFX("assets/barrel.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);
                    player->gold -= 100;

                    for (int i = 0; i < context->nbItem; i++) {
                        if (i != 2)
                            inventaireAjoutObjet(player->inventaire, liste[i], 1);
                    }

                    equipementRecalculerStats(&player);
                }

                // changer scène ici
            }

            blacksmith->interactKeyPressed = interactKeyIsPressed;
        } else {
            blacksmith->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(blacksmith->detectionRange.x, blacksmith->detectionRange.y, blacksmith->detectionRange.radius, blacksmith->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }
}

void renderBlacksmith(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_blacksmith* blacksmith = (t_blacksmith*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (blacksmith->playerInRange && blacksmith->interactText) {
        blacksmith->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (blacksmith->interactText->rect.w / 2);
        blacksmith->interactText->rect.y = entity->entity.displayRect.y - blacksmith->interactText->rect.h - 5;

        renderText(context->renderer, blacksmith->interactText);
    }

    if (blacksmith->playerInRange && blacksmith->item) {
        blacksmith->item->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (blacksmith->item->rect.w / 2);
        blacksmith->item->rect.y = entity->entity.displayRect.y - blacksmith->item->rect.h * 2 - 5;

        renderText(context->renderer, blacksmith->item);
    }
}

t_tileEntity* createBlacksmithEntity(t_tileset* tileset, t_scene* scene, t_context* context) {
    t_blacksmith* blacksmith = malloc(sizeof(t_blacksmith));
    memset(blacksmith, 0, sizeof(t_blacksmith));
    initTileEntityBase(&blacksmith->base, getObject(tileset->textureTiles, 87), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(blacksmith->base.entity.animationController, createAnimation(tileset, (int[]){87}, 1, 240, SDL_TRUE, "idle"));
    setAnimation(blacksmith->base.entity.animationController, "idle");

    blacksmith->detectionRange.radius = 2 * 20;
    blacksmith->detectionRange.x = 10;
    blacksmith->detectionRange.y = 10;
    blacksmith->playerInRange = SDL_FALSE;

    blacksmith->objet = context->itemListe[3];

    blacksmith->interactKeyPressed = SDL_FALSE;
    blacksmith->lastInteractKey = 0;
    blacksmith->interactText = NULL;
    blacksmith->item = NULL;

    blacksmith->base.update = updateBlacksmith;
    blacksmith->base.render = renderBlacksmith;

    return (t_tileEntity*)blacksmith;
}
