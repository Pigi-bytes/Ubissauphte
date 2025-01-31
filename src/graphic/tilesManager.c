#include "tilesManager.h"

t_tileset* initTileset(SDL_Renderer* renderer, int width, int height, int tileSize, char* filename) {
    t_tileset* tileset = (t_tileset*)malloc(sizeof(t_tileset));
    if (!tileset) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tileset\n");
        return NULL;
    }

    tileset->width = width;
    tileset->height = height;
    tileset->tileSize = tileSize;

    tileset->textureTiles = initObjectManager(SDL_TEXTURE_TYPE, SDL_DestroyTextureWrapper, 140);
    if (!tileset->textureTiles) {
        fprintf(stderr, "Erreur d'initialisation du gestionnaire d'objets\n");
        free(tileset);
        return NULL;
    }

    SDL_Texture* tilesetTexture = loadImage(filename, renderer);

    ADD_TYPED_OBJECT(tileset->textureTiles, SDL_TEXTURE_TYPE, NULL);  // NULL texture en 0

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

            ADD_TYPED_OBJECT(tileset->textureTiles, SDL_TEXTURE_TYPE, tile);
        }
    }

    SDL_DestroyTexture(tilesetTexture);

    return tileset;
}

void freeTileset(t_tileset* tileset) {
    freeObjectManager(tileset->textureTiles);
    free(tileset);
}

t_grid* createGrid(int width, int height, int depth) {
    t_grid* grid = (t_grid*)malloc(sizeof(t_grid));
    grid->width = width;
    grid->height = height;
    grid->depth = depth;

    grid->tiles = (t_tile****)malloc(width * sizeof(t_tile***));
    for (int x = 0; x < width; x++) {
        grid->tiles[x] = (t_tile***)malloc(height * sizeof(t_tile**));
        for (int y = 0; y < height; y++) {
            grid->tiles[x][y] = (t_tile**)malloc(depth * sizeof(t_tile*));
            for (int z = 0; z < depth; z++) {
                grid->tiles[x][y][z] = (t_tile*)malloc(sizeof(t_tile));
                grid->tiles[x][y][z]->x = x;
                grid->tiles[x][y][z]->y = y;
                grid->tiles[x][y][z]->z = z;
                grid->tiles[x][y][z]->flip = SDL_FLIP_NONE;  // Aucun retournement par défaut
                grid->tiles[x][y][z]->texture = NULL;
            }
        }
    }

    return grid;
}

void freeGridTiles(t_grid* grid) {
    if (!grid) return;

    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            for (int z = 0; z < grid->depth; z++) {
                if (grid->tiles[x][y][z]) {
                    free(grid->tiles[x][y][z]);
                }
            }
        }
    }
}

void dessinerGrille(SDL_Renderer* renderer, t_grid* grid, int windowWidth, int windowHeight) {
    int newTileSizeX = windowWidth / grid->width;
    int newTileSizeY = windowHeight / grid->height;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = 0; y < grid->height; y++) {
            for (int x = 0; x < grid->width; x++) {
                t_tile* tile = grid->tiles[x][y][z];

                // Dessiner uniquement si la tuile existe et possède une texture
                if (tile && tile->texture) {
                    SDL_Rect dst = {x * newTileSizeX, y * newTileSizeY, newTileSizeX, newTileSizeY};
                    SDL_RenderCopyEx(renderer, tile->texture, NULL, &dst, 0, NULL, tile->flip);
                }
            }
        }
    }
}

t_tile* getTile(t_grid* grid, int x, int y, int z) {
    if (x < 0 || x >= grid->width || y < 0 || y >= grid->height || z < 0 || z >= grid->depth) {
        return NULL;  // Les coordonnées sont hors limites
    }
    return grid->tiles[x][y][z];
}

void SDL_DestroyTextureWrapper(void* object) {
    SDL_Texture* texture = (SDL_Texture*)object;
    SDL_DestroyTexture(texture);
}

void appliquerTextureNiveau(t_grid* grid, int z, void* textureV) {
    SDL_Texture* texture = (SDL_Texture*)textureV;

    if (z < 0 || z >= grid->depth) {
        fprintf(stderr, "Niveau z invalide : %d\n", z);
        return;
    }

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            grid->tiles[x][y][z]->texture = texture;
        }
    }
}
