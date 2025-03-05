#include "movementPlayer.h"

SDL_bool checkCircleRectCollision(t_circle* circle, SDL_Rect* rect, SDL_Point* impactPoint, float* penetrationDepth) {
    float closestX = fmaxf(rect->x, fminf(circle->x, rect->x + rect->w));
    float closestY = fmaxf(rect->y, fminf(circle->y, rect->y + rect->h));

    float distanceX = circle->x - closestX;
    float distanceY = circle->y - closestY;

    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    float radiusSquared = circle->radius * circle->radius;

    if (distanceSquared < radiusSquared) {
        if (impactPoint) {
            impactPoint->x = (int)closestX;
            impactPoint->y = (int)closestY;
        }

        *penetrationDepth = circle->radius - sqrtf(distanceSquared);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool checkGridCollision(t_grid* grid, t_circle* circle) {
    int tileSize = 16;
    int startX = (circle->x - circle->radius) / tileSize;
    int startY = (circle->y - circle->radius) / tileSize;
    int endX = (circle->x + circle->radius) / tileSize;
    int endY = (circle->y + circle->radius) / tileSize;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                t_tile* tile = getTile(grid, x, y, z);
                if (tile->solide) {
                    SDL_Point impactPoint;
                    float penetrationDepth;
                    if (checkCircleRectCollision(circle, &tile->entity.collisionRect, &impactPoint, &penetrationDepth)) {
                        return SDL_TRUE;
                    }
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

    SDL_Point impactPoint;
    float penetrationDepth;
    SDL_bool collision = SDL_FALSE;

    if (dx != 0) {
        t_circle tempCircle = player->entity.collisionCircle;
        tempCircle.x += dx;
        if (checkGridCollision(grid, &tempCircle)) {
            collision = SDL_TRUE;
        } else {
            player->entity.displayRect.x += dx;
            player->entity.collisionCircle.x = player->entity.displayRect.x + player->entity.displayRect.w / 2;
        }
    }

    if (dy != 0) {
        t_circle tempCircle = player->entity.collisionCircle;
        tempCircle.y += dy;
        if (checkGridCollision(grid, &tempCircle)) {
            collision = SDL_TRUE;
        } else {
            player->entity.displayRect.y += dy;
            player->entity.collisionCircle.y = player->entity.displayRect.y + player->entity.displayRect.h / 2;
        }
    }
}