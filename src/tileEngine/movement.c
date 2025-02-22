#include "movement.h"

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
                if (tile && tile->solide) {
                    return;
                }
            }
        }
    }

    player->entity.rect.x = temp.x;
    player->entity.rect.y = temp.y;
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid) {
    float speed = 2;
    float dx = 0;
    float dy = 0;

    if (input->key[player->control->left]) dx = -speed;
    if (input->key[player->control->right]) dx = speed;
    if (input->key[player->control->up]) dy = -speed;
    if (input->key[player->control->down]) dy = speed;

    if (dx && dy) {
        dx *= (1.0 / sqrt(2));
        dy *= (1.0 / sqrt(2));
    }

    movePlayer(player, dx, dy, grid);
}

t_joueur* createplayer(t_control* control, SDL_Texture* texture, SDL_Rect rect) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.rect = rect;

    return joueur;
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player) {
    SDL_RenderCopy(renderer, player->entity.texture, NULL, &player->entity.rect);
}

void freePlayer(void* object) {
    t_joueur* player = (t_joueur*)object;
    SDL_DestroyTexture(player->entity.texture);
    free(player);
}
