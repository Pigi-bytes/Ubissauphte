#include "tileset.h"


t_tileset* initTileset(SDL_Renderer* renderer, int width, int height, int tileSize, char* filename) {
    t_tileset* tileset = (t_tileset*)malloc(sizeof(t_tileset));
    if (!tileset) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tileset\n");
        return NULL;
    }

    tileset->width = width;
    tileset->height = height;
    tileset->tileSize = tileSize;

    tileset->textureTiles = initObjectManager(createTypeRegistry());
    const uint8_t TEXTURE_TYPE = registerType(tileset->textureTiles->registry, SDL_DestroyTextureWrapper, "texture");

    if (!tileset->textureTiles) {
        fprintf(stderr, "Erreur d'initialisation du gestionnaire d'objets\n");
        free(tileset);
        return NULL;
    }

    SDL_Texture* tilesetTexture = loadImage(filename, renderer);
    addObject(tileset->textureTiles, NULL, TEXTURE_TYPE);

    int tilesX = tileset->width / tileset->tileSize;
    int tilesY = tileset->height / tileset->tileSize;

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            SDL_Rect srcRect = {x * tileset->tileSize, y * tileset->tileSize, tileset->tileSize, tileset->tileSize};
            SDL_Texture* tile = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tileset->tileSize, tileset->tileSize);
            if (!tile) {
                fprintf(stderr, "Erreur SDL_CreateTexture : %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }

            SDL_SetTextureBlendMode(tile, SDL_BLENDMODE_BLEND);
            SDL_SetRenderTarget(renderer, tile);
            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, NULL);
            SDL_SetRenderTarget(renderer, NULL);

            addObject(tileset->textureTiles, tile, TEXTURE_TYPE);
        }
    }

    SDL_DestroyTexture(tilesetTexture);
    return tileset;
}

void freeTileset(t_tileset* tileset) {
    freeObjectManager(tileset->textureTiles);
    free(tileset);
}

void SDL_DestroyTextureWrapper(void* object) {
    SDL_Texture* texture = (SDL_Texture*)object;
    SDL_DestroyTexture(texture);
}
