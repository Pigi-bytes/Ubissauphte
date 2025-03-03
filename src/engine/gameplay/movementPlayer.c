#include "movementPlayer.h"

SDL_bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2) {
    return (SDL_bool)((rect1->x < rect2->x + rect2->w &&
                       rect1->x + rect1->w > rect2->x &&
                       rect1->y < rect2->y + rect2->h &&
                       rect1->y + rect1->h > rect2->y));
}

SDL_bool checkGridCollision(t_grid* grid, SDL_Rect* rect) {
    int tileSize = 16;
    int startX = rect->x / tileSize;
    int startY = rect->y / tileSize;
    int endX = (rect->x + rect->w) / tileSize;
    int endY = (rect->y + rect->h) / tileSize;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                t_tile* tile = getTile(grid, x, y, z);
                if (tile->solide && checkCollision(rect, &tile->entity.rect)) {
                    return SDL_TRUE;
                }
            }
        }
    }
    return SDL_FALSE;
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

    if (dx != 0) {
        SDL_Rect tempX = player->entity.rect;
        tempX.x += dx;
        if (!checkGridCollision(grid, &tempX)) {
            player->entity.rect.x = tempX.x;
        }
    }

    if (dy != 0) {
        SDL_Rect tempY = player->entity.rect;
        tempY.y += dy;
        if (!checkGridCollision(grid, &tempY)) {
            player->entity.rect.y = tempY.y;
        }
    }
}