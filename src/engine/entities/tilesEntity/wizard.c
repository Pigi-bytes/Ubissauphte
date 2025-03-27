#include "wizard.h"


void updateWizard(t_tileEntity* entity, t_context* context, t_grid* grid, t_objectManager* entities){
    t_wizard* wizard = (t_wizard*)entity;

    wizard->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    wizard->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        wizard->playerInRange = checkCircleCircleCollision(&wizard->detectionRange, &player->entity.collisionCircle, NULL);

        if (wizard->lastInteractKey != player->control->interact) {
            wizard->lastInteractKey = player->control->interact;

            char interactPrompt[32];
            sprintf(interactPrompt, "[%s] to speak", SDL_GetKeyName(SDL_GetKeyFromScancode(wizard->lastInteractKey)));

            if (wizard->interactText) {
                updateText(&wizard->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 255, 255});
            } else {
                wizard->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 255, 255});
            }
        }

        if (wizard->playerInRange) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !wizard->interactKeyPressed) {
                jouerSFX("assets/chestOpening.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);

                // changer scène ici

                printf("Parle au marchant\n");
            }

            wizard->interactKeyPressed = interactKeyIsPressed;
        } else {
            wizard->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(wizard->detectionRange.x, wizard->detectionRange.y, wizard->detectionRange.radius, wizard->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }

    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, grid, entities);
}

void renderWizard(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_wizard* wizard = (t_wizard*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (wizard->playerInRange && wizard->interactText) {
        wizard->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (wizard->interactText->rect.w / 2);
        wizard->interactText->rect.y = entity->entity.displayRect.y - wizard->interactText->rect.h - 5;

        renderText(context->renderer, wizard->interactText);
    }
}

t_tileEntity* createWizardEntity(t_tileset* tileset, t_scene* scene) {
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
    wizard->interactText = NULL;

    wizard->base.update = updateWizard;
    wizard->base.render = renderWizard;

    return (t_tileEntity*)wizard;
}

