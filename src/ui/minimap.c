#include "minimap.h"

t_minimap* createMinimap(SDL_Renderer* renderer, int windowW, int windowH) {
    t_minimap* minimap = (t_minimap*)malloc(sizeof(t_minimap));
    if (!minimap) {
        fprintf(stderr, "Erreur: Impossible d'allouer la mémoire pour la minimap.\n");
        return NULL;
    }

    minimap->size = MINIMAP_SIZE;      // Taille de la minimap (MINIMAP_SIZExMINIMAP_SIZE pixels)
    minimap->margin = MINIMAP_MARGIN;  // Marge entre la minimap et les bords de l'écran

    minimap->area = (SDL_Rect){
        windowW - minimap->size - minimap->margin,  // X
        windowH - minimap->size - minimap->margin,  // Y
        minimap->size,                              // Largeur
        minimap->size                               // Hauteur
    };

    minimap->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, minimap->size, minimap->size);
    if (!minimap->texture) {
        fprintf(stderr, "Erreur: Impossible de créer la texture de la minimap.\n");
        free(minimap);
        return NULL;
    }

    // Activer le mode transparence
    SDL_SetTextureBlendMode(minimap->texture, SDL_BLENDMODE_BLEND);

    return minimap;
}

void resizeMinimap(SDL_Renderer* renderer, t_minimap* minimap, int* windowW, int* windowH) {
    minimap->area = (SDL_Rect){
        *windowW - minimap->size - minimap->margin,  // X
        *windowH - minimap->size - minimap->margin,  // Y
        minimap->size,                               // Largeur
        minimap->size                                // Hauteur
    };
}

void updateMinimap(t_minimap* minimap, t_camera* camera, SDL_Renderer* renderer, t_objectManager* entities, t_grid* grid) {
    int transparency = 196;

    float scaleX = (float)minimap->size / camera->levelW;
    float scaleY = (float)minimap->size / camera->levelH;

    // Changer la cible de rendu vers la texture de la minimap
    SDL_SetRenderTarget(renderer, minimap->texture);

    // Effacer la texture avec un fond transparent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Dessiner le fond de la minimap
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, transparency);
    SDL_RenderFillRect(renderer, NULL);

    if (grid != NULL) {
        SDL_SetRenderDrawColor(renderer, 64, 64, 64, transparency);  // Gris pour les murs/obstacles

        for (int z = 0; z < grid->depth; z++) {
            for (int y = 0; y < grid->height; y++) {
                for (int x = 0; x < grid->width; x++) {
                    t_tile* tile = getTile(grid, x, y, z);
                    if (tile && tile->solide) {
                        SDL_Rect tileRect = {
                            (int)(x * TILE_SIZE * scaleX),
                            (int)(y * TILE_SIZE * scaleY),
                            (int)(TILE_SIZE * scaleX) + 1,  // +1 pour éviter les trous
                            (int)(TILE_SIZE * scaleY) + 1};
                        SDL_RenderFillRect(renderer, &tileRect);
                    }
                }
            }
        }
    }

    if (entities != NULL) {
        for (int i = 0; i < entities->count; i++) {
            void* entity = getObject(entities, i);
            if (entity == NULL) continue;

            uint8_t typeId = getObjectTypeId(entities, i);
            int entitySize = 2;
            SDL_FPoint entityPos = {0, 0};
            SDL_Color entityColor = {255, 255, 255, transparency};

            if (typeId == getTypeIdByName(entities->registry, "PLAYER")) {
                t_joueur* player = (t_joueur*)entity;
                entityPos.x = player->entity.collisionCircle.x;
                entityPos.y = player->entity.collisionCircle.y;
                entityColor = (SDL_Color){255, 0, 0, 255}; 
                entitySize = 4;                            
            } else if (typeId == getTypeIdByName(entities->registry, "ENEMY")) {
                t_enemy* enemy = (t_enemy*)entity;
                entityPos.x = enemy->entity.collisionCircle.x;
                entityPos.y = enemy->entity.collisionCircle.y;
                entityColor = (SDL_Color){255, 150, 0, 220};
            } else if (typeId == getTypeIdByName(entities->registry, "TILE_ENTITY")) {
                t_tileEntity* tileEntity = (t_tileEntity*)entity;
                entityPos.x = tileEntity->entity.collisionCircle.x;
                entityPos.y = tileEntity->entity.collisionCircle.y;
                entityColor = (SDL_Color){50, 200, 50, 220};
            }

            int minimapX = (int)(entityPos.x * scaleX);
            int minimapY = (int)(entityPos.y * scaleY);

            SDL_SetRenderDrawColor(renderer, entityColor.r, entityColor.g, entityColor.b, entityColor.a);
            for (int dx = -entitySize; dx <= entitySize; dx++) {
                for (int dy = -entitySize; dy <= entitySize; dy++) {
                    if (dx * dx + dy * dy <= entitySize * entitySize) {
                        SDL_RenderDrawPoint(renderer, minimapX + dx, minimapY + dy);
                    }
                }
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, transparency);
    SDL_RenderDrawRect(renderer, NULL);

    SDL_Rect cameraView = {
        (int)(camera->x * scaleX),
        (int)(camera->y * scaleY),
        (int)(camera->w * scaleX),
        (int)(camera->h * scaleY)};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, transparency);
    SDL_RenderDrawRect(renderer, &cameraView);

    // Revenir au renderer
    SDL_SetRenderTarget(renderer, NULL);
}

void renderMinimap(SDL_Renderer* renderer, t_minimap* minimap) {
    SDL_RenderCopy(renderer, minimap->texture, NULL, &minimap->area);
}

void freeMinimap(void* object) {
    t_minimap* minimap = (t_minimap*)object;
    if (minimap) {
        if (minimap->texture) {
            SDL_DestroyTexture(minimap->texture);
        }
        free(minimap);
    }
}