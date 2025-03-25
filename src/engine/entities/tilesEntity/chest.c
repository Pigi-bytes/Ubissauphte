#include "chest.h"

void updateChest(t_tileEntity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities, t_input* input) {
    t_chest* chest = (t_chest*)entity;

    chest->detectionRange.x = entity->entity.displayRect.x + entity->entity.displayRect.w / 2;
    chest->detectionRange.y = entity->entity.displayRect.y + entity->entity.displayRect.h / 2;

    t_joueur* player = (t_joueur*)getObject(entities, 0);

    if (player) {
        chest->playerInRange = checkCircleCircleCollision(&chest->detectionRange, &player->entity.collisionCircle, NULL);

        if (chest->playerInRange && !chest->isOpen) {
            SDL_bool interactKeyIsPressed = keyPressOnce(input, player->control->interact);

            if (interactKeyIsPressed && !chest->interactKeyPressed) {
                chest->isOpen = SDL_TRUE;
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

    updatePhysicEntity(&entity->entity, deltaTime, grid, entities);
}

void renderChest(SDL_Renderer* renderer, t_tileEntity* entity, t_camera* camera) {
    t_chest* chest = (t_chest*)entity;

    if (chest->isOpen) {
        // Animation or special rendering for open chest
    }

    renderEntity(renderer, &entity->entity, camera);

    if (entity->entity.debug) {
        // Debug rendering
    }
}

t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene) {
    t_chest* chest = malloc(sizeof(t_chest));
    memset(chest, 0, sizeof(t_chest));

    initTileEntityBase(&chest->base, getObject(tileset->textureTiles, 90), (SDL_Rect){0, 0, 16, 16}, scene);

    addAnimation(chest->base.entity.animationController, createAnimation(tileset, (int[]){90}, 1, 240, SDL_TRUE, "close"));
    addAnimation(chest->base.entity.animationController, createAnimation(tileset, (int[]){90, 91, 92}, 3, 240, SDL_FALSE, "open"));
    setAnimation(chest->base.entity.animationController, "close");

    chest->baseDetectionRadius = 2 * 16;
    chest->detectionRange.radius = chest->baseDetectionRadius;
    chest->detectionRange.x = 8;
    chest->detectionRange.y = 8;
    chest->isOpen = SDL_FALSE;
    chest->playerInRange = SDL_FALSE;

    chest->interactKeyPressed = SDL_FALSE;

    chest->base.update = updateChest;
    chest->base.render = renderChest;

    return (t_tileEntity*)chest;
}