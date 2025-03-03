#include "movementPlayer.h"

SDL_bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2) {
    return (SDL_bool)((rect1->x < rect2->x + rect2->w &&
                       rect1->x + rect1->w > rect2->x &&
                       rect1->y < rect2->y + rect2->h &&
                       rect1->y + rect1->h > rect2->y));
}

void movePlayer(t_joueur* player, int dx, int dy, t_grid* grid) {
    SDL_Rect temp = player->entity.rect;
    temp.x += dx;
    temp.y += dy;

    int tileSize = 16;
    int startX = temp.x / tileSize;
    int startY = temp.y / tileSize;
    int endX = (temp.x + temp.w) / tileSize;
    int endY = (temp.y + temp.h) / tileSize;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                t_tile* tile = getTile(grid, x, y, z);
                // tile->entity.debug = SDL_TRUE;
                if (tile && tile->solide) {
                    return;
                }
            }
        }
    }

    player->entity.rect.x = temp.x;
    player->entity.rect.y = temp.y;
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, float* deltaTime) {
    float speed = 20.0f;
    float dx = 0.0f;
    float dy = 0.0f;

    if (input->key[player->control->left]) {
        dx -= 1.0f;
        player->entity.flip = SDL_FLIP_HORIZONTAL;
    }
    if (input->key[player->control->right]) {
        dx += 1.0f;
        player->entity.flip = SDL_FLIP_NONE;
    }
    if (input->key[player->control->up]) dy -= 1.0f;
    if (input->key[player->control->down]) dy += 1.0f;

    if (dx != 0.0f || dy != 0.0f) {
        setAnimation(player->entity.animationController, "walk");
    } else {
        setAnimation(player->entity.animationController, "idle");
    }

    dx *= speed * (*deltaTime * 10);
    dy *= speed * (*deltaTime * 10);

    if (dx != 0.0f || dy != 0.0f) {
        movePlayer(player, dx, dy, grid);
    }
}