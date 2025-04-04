#include "wizard.h"


void updateWizard(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities){
    t_wizard* wizard = (t_wizard*)entity;

    wizard->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    wizard->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        wizard->playerInRange = checkCircleCircleCollision(&wizard->detectionRange, &player->entity.collisionCircle, NULL);

        if (wizard->lastInteractKey != player->control->interact) {
            wizard->lastInteractKey = player->control->interact;

            char interactPrompt[64];
            char interactPrompt2[64];
            sprintf(interactPrompt, "%s", wizard->randomItem->name);
            sprintf(interactPrompt2, "[%s] to buy for %d coins", SDL_GetKeyName(SDL_GetKeyFromScancode(wizard->lastInteractKey)), wizard->prix);

            if (wizard->interactText) {
                updateText(&wizard->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 0, 255});
            } else {
                wizard->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 0, 255});
            }

            if (wizard->item) {
                updateText(&wizard->item, context->renderer, interactPrompt2, (SDL_Color){50, 150, 255, 255});
            } else {
                wizard->item = createText(context->renderer, interactPrompt2, context->gameFont, (SDL_Color){50, 150, 255, 255});
            }
        }

        if (wizard->playerInRange) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !wizard->interactKeyPressed) {
                if (player->gold >= wizard->prix) {
                    jouerSFX("assets/barrel.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);
                    player->gold -= wizard->prix;
                    if (wizard->randomItem) {
                        inventaireAjoutObjet(player->inventaire, wizard->randomItem, 1);
                    } 
                    equipementRecalculerStats(&player);
                }else{
                }

                // changer scène ici
            }

            wizard->interactKeyPressed = interactKeyIsPressed;
        } else {
            wizard->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(wizard->detectionRange.x, wizard->detectionRange.y, wizard->detectionRange.radius, wizard->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }
}

void renderWizard(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_wizard* wizard = (t_wizard*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (wizard->playerInRange && wizard->interactText) {
        wizard->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (wizard->interactText->rect.w / 2);
        wizard->interactText->rect.y = entity->entity.displayRect.y - wizard->interactText->rect.h - 5;

        renderText(context->renderer, wizard->interactText);
    }

    if (wizard->playerInRange && wizard->item) {
        wizard->item->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (wizard->item->rect.w / 2);
        wizard->item->rect.y = entity->entity.displayRect.y - wizard->item->rect.h * 2 - 5;

        renderText(context->renderer, wizard->item);
    }
}

t_tileEntity* createWizardEntity(t_tileset* tileset, t_scene* scene, t_context* context) {
    t_wizard* wizard = malloc(sizeof(t_wizard));
    memset(wizard, 0, sizeof(t_wizard));
    initTileEntityBase(&wizard->base, getObject(tileset->textureTiles, 85), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(wizard->base.entity.animationController, createAnimation(tileset, (int[]){85}, 1, 240, SDL_TRUE, "idle"));
    setAnimation(wizard->base.entity.animationController, "idle");

    wizard->detectionRange.radius = 2 * 20;
    wizard->detectionRange.x = 10;
    wizard->detectionRange.y = 10;
    wizard->playerInRange = SDL_FALSE;

    wizard->interactKeyPressed = SDL_FALSE;
    wizard->lastInteractKey = 0;

    wizard->randomItem = getItemByRarity(context->itemListe, context->nbItem, RARITY_EPIC);

    wizard->interactText = NULL;
    wizard->prix = 1000;

    wizard->base.update = updateWizard;
    wizard->base.render = renderWizard;


    return (t_tileEntity*)wizard;
}

