#include "chest.h"

void updateChest(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities) {
    t_chest* chest = (t_chest*)entity;

    chest->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    chest->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        chest->playerInRange = checkCircleCircleCollision(&chest->detectionRange, &player->entity.collisionCircle, NULL);

        if (chest->lastInteractKey != player->control->interact) {
            chest->lastInteractKey = player->control->interact;

            char interactPrompt[32];
            sprintf(interactPrompt, "[%s] to open", SDL_GetKeyName(SDL_GetKeyFromScancode(chest->lastInteractKey)));

            if (chest->interactText) {
                updateText(&chest->interactText, context->renderer, interactPrompt, (SDL_Color){255, 255, 255, 255});
            } else {
                chest->interactText = createText(context->renderer, interactPrompt, context->gameFont, (SDL_Color){255, 255, 255, 255});
            }
        }

        if (chest->playerInRange && !chest->isOpen) {
            SDL_bool interactKeyIsPressed = keyPressOnce(context->input, player->control->interact);

            if (interactKeyIsPressed && !chest->interactKeyPressed) {
                chest->isOpen = SDL_TRUE;
                jouerSFX("assets/chestOpening.wav", SDL_MIX_MAXVOLUME, 0, context->audioManager);

                setAnimation(chest->base.entity.animationController, "open");
                printf("Coffre ouvert ! Vous avez trouvé un trésor !\n");
            }

            chest->interactKeyPressed = interactKeyIsPressed;
        } else {
            chest->interactKeyPressed = SDL_FALSE;
        }

        if (entity->entity.debug) {
            Debug_PushCircle(chest->detectionRange.x, chest->detectionRange.y, chest->detectionRange.radius, chest->playerInRange ? SDL_COLOR_GREEN : SDL_COLOR_BLUE);
        }
    }

    updatePhysicEntity(&entity->entity, &context->frameData->deltaTime, salle->grille, entities);
}

void renderChest(t_tileEntity* entity, t_context* context, t_camera* camera) {
    t_chest* chest = (t_chest*)entity;

    renderEntity(context->renderer, &entity->entity, camera);

    if (chest->playerInRange && !chest->isOpen && chest->interactText) {
        chest->interactText->rect.x = entity->entity.displayRect.x + (entity->entity.displayRect.w / 2) - (chest->interactText->rect.w / 2);
        chest->interactText->rect.y = entity->entity.displayRect.y - chest->interactText->rect.h - 5;

        renderText(context->renderer, chest->interactText);
    }
}

t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene) {
    t_chest* chest = malloc(sizeof(t_chest));
    memset(chest, 0, sizeof(t_chest));

    initTileEntityBase(&chest->base, getObject(tileset->textureTiles, 90), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(chest->base.entity.animationController, createAnimation(tileset, (int[]){90}, 1, 240, SDL_TRUE, "close"));
    addAnimation(chest->base.entity.animationController, createAnimation(tileset, (int[]){90, 91, 92}, 3, 240, SDL_FALSE, "open"));
    setAnimation(chest->base.entity.animationController, "close");

    chest->detectionRange.radius = 2 * 16;
    chest->detectionRange.x = 8;
    chest->detectionRange.y = 8;
    chest->isOpen = SDL_FALSE;
    chest->playerInRange = SDL_FALSE;

    chest->interactKeyPressed = SDL_FALSE;
    chest->lastInteractKey = 0;
    chest->interactText = NULL;

    chest->base.update = updateChest;
    chest->base.render = renderChest;

    return (t_tileEntity*)chest;
}