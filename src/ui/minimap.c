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

void resizeMinimap(SDL_Renderer* renderer, t_minimap* minimap, int *windowW, int *windowH) {
    minimap->area = (SDL_Rect){
        *windowW - minimap->size - minimap->margin,  // X
        *windowH - minimap->size - minimap->margin,  // Y
        minimap->size,                              // Largeur
        minimap->size                               // Hauteur
    };
}

void updateMinimap(t_minimap* minimap, t_camera* camera, SDL_Renderer* renderer) {
    int transparency = 196;
    // Changer la cible de rendu vers la texture de la minimap
    SDL_SetRenderTarget(renderer, minimap->texture);

    // Effacer la texture avec un fond transparent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Dessiner le fond de la minimap
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, transparency);
    SDL_RenderFillRect(renderer, NULL);

    // Dessiner la bordure de la minimap
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, transparency);
    SDL_RenderDrawRect(renderer, NULL);

    // Calculer l'échelle
    float scaleX = (float)minimap->size / camera->levelW;
    float scaleY = (float)minimap->size / camera->levelH;

    // Position de la caméra sur la minimap
    SDL_Rect cameraView = {
        (int)(camera->x * scaleX),
        (int)(camera->y * scaleY),
        (int)(camera->w * scaleX),
        (int)(camera->h * scaleY)};

    // Dessiner le rectangle représentant la caméra
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