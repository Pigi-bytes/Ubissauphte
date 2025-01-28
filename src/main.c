#include <SDL2/SDL.h>

#include "utils/imageLoader.h"
#include "utils/objectManager.h"

typedef struct {
    int width;
    int height;
    int tileSize;
    t_objectManager* textureTiles;
} t_tileset;

typedef struct {
    int x;
    int y;
    int z;
    SDL_Texture* texture;
    SDL_RendererFlip flip;
} t_tile;

typedef struct {
    int width;
    int height;
    int depth;
    t_tile**** tiles;
} t_grid;

void SDL_DestroyTextureWrapper(void* object);

t_tileset* initTileset(SDL_Renderer* renderer, int width, int height, int tileSize, char* filename) {
    t_tileset* tileset = (t_tileset*)malloc(sizeof(t_tileset));
    if (!tileset) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tileset\n");
        return NULL;
    }

    tileset->width = width;
    tileset->height = height;
    tileset->tileSize = tileSize;

    tileset->textureTiles = initObjectManager(SDL_TEXTURE_TYPE, SDL_DestroyTextureWrapper);
    if (!tileset->textureTiles) {
        fprintf(stderr, "Erreur d'initialisation du gestionnaire d'objets\n");
        free(tileset);
        return NULL;
    }

    SDL_Texture* tilesetTexture = loadImage(filename, renderer);

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

t_grid* create_grid(int width, int height, int depth) {
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
                grid->tiles[x][y][z] = NULL;  // Initialisation à NULL
            }
        }
    }

    return grid;
}

void free_grid(t_grid* grid) {
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            free(grid->tiles[x][y]);
        }
        free(grid->tiles[x]);
    }
    free(grid->tiles);
    free(grid);
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

void* GET_TYPED_OBJECT(t_objectManager* manager, size_t index) {
    if (manager == NULL || index >= manager->count) {
        return NULL;
    }

    t_typedObject* obj = manager->items[index];
    return obj->data;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_tileset* tileset = initTileset(renderer, 192, 176, 16, "../assets/imgs/tileMapDungeon.bmp");
    t_grid* level = create_grid(4, 5, 5);

    int tileSize = tileset->tileSize;
    int tilesX = tileset->width / tileSize;
    int tilesY = tileset->height / tileSize;

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            SDL_Rect dstRect = {x * tileSize, y * tileSize, tileSize, tileSize};
            SDL_Texture* tile = GET_TYPED_OBJECT(tileset->textureTiles, y * tilesX + x);
            SDL_RenderCopy(renderer, tile, NULL, &dstRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(100);  // Delay pour voir chaque tile individuellement
        }
    }

    free_grid(level);
    freeTileset(tileset);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}
