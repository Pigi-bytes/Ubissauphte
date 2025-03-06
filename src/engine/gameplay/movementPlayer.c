#include "movementPlayer.h"

SDL_bool checkCircleRectCollision(t_circle* circle, SDL_Rect* rect, float* penetrationDepth, SDL_FPoint* normal) {
    SDL_FPoint impactPoint;
    float closestX = fmaxf(rect->x, fminf(circle->x, rect->x + rect->w));
    float closestY = fmaxf(rect->y, fminf(circle->y, rect->y + rect->h));

    if (circle->x >= rect->x && circle->x <= rect->x + rect->w) {
        closestX = circle->x;
        closestY = fmaxf(rect->y, fminf(circle->y, rect->y + rect->h));
    }
    if (circle->y >= rect->y && circle->y <= rect->y + rect->h) {
        closestX = fmaxf(rect->x, fminf(circle->x, rect->x + rect->w));
        closestY = circle->y;
    }

    float distanceX = circle->x - closestX;
    float distanceY = circle->y - closestY;

    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    float radiusSquared = circle->radius * circle->radius;

    if (distanceSquared < radiusSquared) {
        impactPoint.x = closestX;
        impactPoint.y = closestY;

        // Pen / 2 en cas d'autre cercle entity
        *penetrationDepth = -(circle->radius - sqrtf(distanceSquared));
        normal->x = impactPoint.x - circle->x;
        normal->y = impactPoint.y - circle->y;
        float norm = sqrtf((normal->x * normal->x) + (normal->y * normal->y));
        normal->x /= norm;
        normal->y /= norm;
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool checkGridCollision(t_grid* grid, t_circle* circle, float* penetrationDepth, SDL_FPoint* normal) {
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
                    if (checkCircleRectCollision(circle, &tile->entity.collisionRect, penetrationDepth, normal)) {
                        normal->x *= *penetrationDepth;
                        normal->y *= *penetrationDepth;
                        circle->x += normal->x;
                        circle->y += normal->y;
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

    float penetrationDepth;
    SDL_FPoint normal;

    SDL_Point lastPos = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};

    if (dx != 0) {
        player->entity.collisionCircle.x += dx;
    }

    if (dy != 0) {
        player->entity.collisionCircle.y += dy;
    }

    checkGridCollision(grid, &player->entity.collisionCircle, &penetrationDepth, &normal);

    player->entity.displayRect.x += player->entity.collisionCircle.x - lastPos.x;
    player->entity.displayRect.y += player->entity.collisionCircle.y - lastPos.y;
}