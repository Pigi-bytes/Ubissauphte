

#include "blacksmith.h"


void updateBlacksmith(t_tileEntity* entity, t_context* context, t_grid* grid, t_objectManager* entities){
    t_blacksmith* blacksmith = (t_blacksmith*)entity;

    blacksmith->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    blacksmith->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        blacksmith->playerInRange = checkCircleCircleCollision(&blacksmith->detectionRange, &player->entity.collisionCircle, NULL);

        if (blacksmith->lastInteractKey != player->control->interact) {
            blacksmith->lastInteractKey = player->control->interact;

            char interactPrompt[32];
            sprintf(interactPrompt, "[%s] to speak", SDL_GetKeyName(SDL_GetKeyFromScancode(blacksmith->lastInteractKey)));

            if (blacksmith->interactText) {
                updateText(&blacksmith->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 255, 255});
            } else {
                blacksmith->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 255, 255});
            }
        }

        if (blacksmith->playerInRange) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !blacksmith->interactKeyPressed) {
                jouerSFX("assets/barrel.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);

                // changer scène ici

                printf("Parle au forgeron\n");
            }

            blacksmith->interactKeyPressed = interactKeyIsPressed;
        } else {
            blacksmith->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(blacksmith->detectionRange.x, blacksmith->detectionRange.y, blacksmith->detectionRange.radius, blacksmith->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }

    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, grid, entities);
}

void renderBlacksmith(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_blacksmith* blacksmith = (t_blacksmith*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (blacksmith->playerInRange && blacksmith->interactText) {
        blacksmith->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (blacksmith->interactText->rect.w / 2);
        blacksmith->interactText->rect.y = entity->entity.displayRect.y - blacksmith->interactText->rect.h - 5;

        renderText(context->renderer, blacksmith->interactText);
    }
}

t_tileEntity* createBlacksmithEntity(t_tileset* tileset, t_scene* scene) {
    t_blacksmith* blacksmith = malloc(sizeof(t_blacksmith));
    memset(blacksmith, 0, sizeof(t_blacksmith));

    initTileEntityBase(&blacksmith->base, getObject(tileset->textureTiles, 87), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(blacksmith->base.entity.animationController, createAnimation(tileset, (int[]){87}, 1, 240, SDL_TRUE, "idle"));
    setAnimation(blacksmith->base.entity.animationController, "idle");

    blacksmith->detectionRange.radius = 2 * 20;
    blacksmith->detectionRange.x = 10;
    blacksmith->detectionRange.y = 10;
    blacksmith->playerInRange = SDL_FALSE;

    blacksmith->interactKeyPressed = SDL_FALSE;
    blacksmith->lastInteractKey = 0;
    blacksmith->interactText = NULL;

    blacksmith->base.update = updateBlacksmith;
    blacksmith->base.render = renderBlacksmith;

    return (t_tileEntity*)blacksmith;
}

