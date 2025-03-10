#include "tiles.h"

t_grid* createGrid(int width, int height, int depth) {
    t_grid* grid = (t_grid*)malloc(sizeof(t_grid));
    grid->width = width;
    grid->height = height;
    grid->depth = depth;

    grid->tiles = (t_tile***)malloc(depth * sizeof(t_tile**));
    for (int z = 0; z < depth; z++) {
        grid->tiles[z] = (t_tile**)malloc(height * sizeof(t_tile*));
        for (int y = 0; y < height; y++) {
            grid->tiles[z][y] = (t_tile*)calloc(width, sizeof(t_tile));
            for (int x = 0; x < width; x++) {
                t_tile* tile = &grid->tiles[z][y][x];
                tile->solide = SDL_FALSE;
                tile->entity.useCircleCollision = SDL_FALSE;
                tile->entity.displayRect = (SDL_Rect){x * 16, y * 16, 16, 16};
                tile->entity.collisionRect = (SDL_Rect){x * 16, y * 16, 16, 16};

                tile->entity.texture = NULL;
                tile->entity.flip = SDL_FLIP_NONE;
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
                t_tile* tile = getTile(grid, x, y, z);
                if (tile->entity.texture) {
                    renderEntity(renderer, &tile->entity, camera);
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

            int index, orientation, collisions;
            if (sscanf(token, "%d:%d:%d", &index, &orientation, &collisions) != 3) {
                fprintf(stderr, "Erreur : format de tuile invalide : %s (couche %d, ligne %d, colonne %d)\n", token, coucheActuelle, y, x);
                token = strtok(NULL, " ");
                continue;
            }

            t_tile* tile = getTile(grid, x, y, coucheActuelle);
            if (tile) {
                tile->entity.texture = (SDL_Texture*)getObject(tileset->textureTiles, index);
                tile->entity.flip = (SDL_RendererFlip)orientation;
                tile->entity.animationController = initAnimationController();
                tile->entity.animationController->haveAnimation = SDL_FALSE;
                tile->entity.debug = collisions;
                tile->solide = collisions;
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