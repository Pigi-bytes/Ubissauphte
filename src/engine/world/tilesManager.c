#include "tilesManager.h"

#define MAX_LINE_LENGTH 2048

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

t_grid* createGrid(int width, int height, int depth) {
    t_grid* grid = (t_grid*)malloc(sizeof(t_grid));
    grid->width = width;
    grid->height = height;
    grid->depth = depth;

    // Allocation en [depth][height][width]
    grid->tiles = (t_tile***)malloc(depth * sizeof(t_tile**));
    for (int z = 0; z < depth; z++) {
        grid->tiles[z] = (t_tile**)malloc(height * sizeof(t_tile*));
        for (int y = 0; y < height; y++) {
            grid->tiles[z][y] = (t_tile*)calloc(width, sizeof(t_tile));
            for (int x = 0; x < width; x++) {
                grid->tiles[z][y][x].x = x;
                grid->tiles[z][y][x].y = y;
                grid->tiles[z][y][x].z = z;
                grid->tiles[z][y][x].flip = SDL_FLIP_NONE;
                grid->tiles[z][y][x].texture = NULL;
            }
        }
    }

    return grid;
}

void freeGrid(void* object) {
    t_grid* grid = (t_grid*)object;

    if (!grid) return;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = 0; y < grid->height; y++) {
            free(grid->tiles[z][y]);
        }
        free(grid->tiles[z]);
    }
    free(grid->tiles);
    free(grid);
}

void renderGrid(SDL_Renderer* renderer, t_grid* grid, t_camera* camera) {
    for (int z = 0; z < grid->depth; z++) {
        for (int y = 0; y < grid->height; y++) {
            for (int x = 0; x < grid->width; x++) {
                t_tile* tile = &grid->tiles[z][y][x];
                // Calcul de la position relative à la caméra
                SDL_Rect dst_rect = {x * 16, y * 16, 16, 16};
                if (isRectOnCamera(&dst_rect, camera)) {
                    SDL_RenderCopyEx(renderer, tile->texture, NULL, &dst_rect, 0, NULL, tile->flip);
                }
            }
        }
    }
}

t_tile* getTile(t_grid* grid, int x, int y, int z) {
    if (x < 0 || x >= grid->width ||
        y < 0 || y >= grid->height ||
        z < 0 || z >= grid->depth) {
        return NULL;
    }
    return &grid->tiles[z][y][x];
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
            grid->tiles[z][y][x].texture = texture;
        }
    }
}

t_grid* loadMap(char* filename, t_tileset* tileset) {
    DEBUG_PRINT("Début du chargement de la carte depuis %s\n", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);

    int width, height, depth;
    if (sscanf(line, "%dx%dx%d", &width, &height, &depth) != 3) {
        fclose(file);
        fprintf(stderr, "Erreur : format de dimensions invalide dans %s\n", filename);
        return NULL;
    }
    DEBUG_PRINT("Dimensions lues : %dx%dx%d\n", width, height, depth);

    t_grid* grid = createGrid(width, height, depth);
    int coucheActuelle = 0;
    int y = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) && coucheActuelle < depth) {
        // Nettoyer la ligne
        line[strcspn(line, "\n")] = '\0';

        // Si la ligne est vide, passer à la couche suivante
        if (strlen(line) == 0) {
            coucheActuelle++;
            y = 0;
            continue;
        }

        DEBUG_PRINT("Lecture de la ligne %d de la couche %d : %s\n", y, coucheActuelle, line);

        char* token = strtok(line, " ");
        for (int x = 0; x < width; x++) {
            if (!token) {
                fprintf(stderr, "Erreur : ligne incomplète dans la couche %d, ligne %d\n", coucheActuelle, y);
                break;
            }

            int index, orientation;
            if (sscanf(token, "%d:%d", &index, &orientation) != 2) {
                fprintf(stderr, "Erreur : format de tuile invalide : %s (couche %d, ligne %d, colonne %d)\n", token, coucheActuelle, y, x);
                token = strtok(NULL, " ");
                continue;
            }

            SDL_Texture* texture = NULL;
            texture = (SDL_Texture*)getObject(tileset->textureTiles, index);

            t_tile* tile = getTile(grid, x, y, coucheActuelle);
            if (tile) {
                tile->texture = texture;
                tile->flip = (SDL_RendererFlip)orientation;
            } else {
                fprintf(stderr, "Erreur : impossible d'accéder à la tuile [%d][%d][%d]\n", coucheActuelle, y, x);
            }

            token = strtok(NULL, " ");
        }
        y++;
    }
    fclose(file);
    DEBUG_PRINT("Chargement de la carte terminé\n");
    return grid;
}