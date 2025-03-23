#include "physicsSystem.h"

void updatePhysicEntity(t_entity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    SDL_Point lastPosVisuelle = {entity->collisionCircle.x, entity->collisionCircle.y};

    // Application de la friction
    // La friction est un pourcentage de reduction de la velocite par seconde
    // (1.0 = 100% de friction, arret immediat, 0.0 = pas de friction)
    entity->physics.velocity.x *= powf(entity->physics.friction, *deltaTime);
    entity->physics.velocity.y *= powf(entity->physics.friction, *deltaTime);
    // PHYSICS_LOG("After friction - Velocity: (%.1f,%.1f)", entity->physics.velocity.x, entity->physics.velocity.y);

    float maxSafeSpeed = 16.0f / *deltaTime;
    float currentSpeed = sqrtf(entity->physics.velocity.x * entity->physics.velocity.x + entity->physics.velocity.y * entity->physics.velocity.y);

    if (currentSpeed > maxSafeSpeed) {
        float scale = maxSafeSpeed / currentSpeed;
        entity->physics.velocity.x *= scale;
        entity->physics.velocity.y *= scale;
    }

    // Intégration de la position
    // On multiplie la velocite (unites/s) par deltaTime (s) pour obtenir
    // le déplacement en unités pour cette frame
    entity->collisionCircle.x += entity->physics.velocity.x * *deltaTime;
    entity->collisionCircle.y += entity->physics.velocity.y * *deltaTime;

    // On resout les collisions
    resolveCollision(entity, grid, entities);

    // Update de la position visuelle du sprite
    // On applique le meme deplacement que celui du cercle de collision
    entity->displayRect.x += entity->collisionCircle.x - lastPosVisuelle.x;
    entity->displayRect.y += entity->collisionCircle.y - lastPosVisuelle.y;

    // PHYSICS_LOG("New position: (%.1f,%.1f)", entity->collisionCircle.x, entity->collisionCircle.y);
}

void resolveCircleRectCollision(t_entity* entity, SDL_Rect* rect, float rectRestitution, t_collisionData* collision) {
    float relativeVelocityX = 0.0f - entity->physics.velocity.x;
    float relativeVelocityY = 0.0f - entity->physics.velocity.y;
    float normalDotVel = collision->normal.x * relativeVelocityX + collision->normal.y * relativeVelocityY;

    if (normalDotVel > 0) {
        float combinedRestitution = (entity->physics.restitution + rectRestitution) / 2;
        float impulseMagnitude = -(1 + combinedRestitution) * normalDotVel;

        entity->physics.velocity.x -= impulseMagnitude * collision->normal.x;
        entity->physics.velocity.y -= impulseMagnitude * collision->normal.y;
    }

    entity->collisionCircle.x += collision->normal.x * collision->depth;
    entity->collisionCircle.y += collision->normal.y * collision->depth;
}

void resolveCircleCircleCollision(t_entity* entity1, t_entity* entity2, t_collisionData* collision) {
    float relativeVelocityX = entity2->physics.velocity.x - entity1->physics.velocity.x;
    float relativeVelocityY = entity2->physics.velocity.y - entity1->physics.velocity.y;
    float normalDotVel = collision->normal.x * relativeVelocityX + collision->normal.y * relativeVelocityY;

    if (normalDotVel > 0) {
        float combinedRestitution = (entity1->physics.restitution + entity2->physics.restitution) / 2;
        float impulseMagnitude = -(1 + combinedRestitution) * normalDotVel;

        float inverseMassSum = (1 / entity1->physics.mass + 1 / entity2->physics.mass);
        impulseMagnitude /= inverseMassSum;

        entity1->physics.velocity.x -= impulseMagnitude / entity1->physics.mass * collision->normal.x;
        entity1->physics.velocity.y -= impulseMagnitude / entity1->physics.mass * collision->normal.y;

        entity2->physics.velocity.x += impulseMagnitude / entity2->physics.mass * collision->normal.x;
        entity2->physics.velocity.y += impulseMagnitude / entity2->physics.mass * collision->normal.y;
    }

    // Reposition first entity to resolve penetration
    entity1->collisionCircle.x += collision->normal.x * collision->depth;
    entity1->collisionCircle.y += collision->normal.y * collision->depth;
}

void resolveGridCollisions(t_entity* entity, t_grid* grid) {
    int tileSize = 16;
    int startX = (entity->collisionCircle.x - entity->collisionCircle.radius) / tileSize;
    int startY = (entity->collisionCircle.y - entity->collisionCircle.radius) / tileSize;
    int endX = (entity->collisionCircle.x + entity->collisionCircle.radius) / tileSize;
    int endY = (entity->collisionCircle.y + entity->collisionCircle.radius) / tileSize;

    t_collisionData collision;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                t_tile* tile = getTile(grid, x, y, z);
                if (tile && tile->solide) {
                    if (checkCircleRectCollision(&entity->collisionCircle, &tile->entity.collisionRect, &collision)) {
                        resolveCircleRectCollision(entity, &tile->entity.collisionRect, tile->entity.physics.restitution, &collision);
                    }
                }
            }
        }
    }
}

void resolveEntityCollisions(t_entity* entity, t_objectManager* entities) {
    t_collisionData collision;

    for (int i = 0; i < entities->count; i++) {
        t_entity* otherEntity = getObject(entities, i);
        if (otherEntity == entity) continue;

        if (entity->useCircleCollision && otherEntity->useCircleCollision) {
            if (checkCircleCircleCollision(&entity->collisionCircle, &otherEntity->collisionCircle, &collision)) {
                resolveCircleCircleCollision(entity, otherEntity, &collision);
            }
        }
    }
}

void resolveCollision(t_entity* entity, t_grid* grid, t_objectManager* entities) {
    resolveGridCollisions(entity, grid);
    resolveEntityCollisions(entity, entities);
}

SDL_bool gridRaycast(t_grid* grid, SDL_FPoint start, SDL_FPoint end, int tileSize, SDL_FPoint* obstructionPoint) {
    int x0 = (int)(start.x / tileSize);
    int y0 = (int)(start.y / tileSize);
    int x1 = (int)(end.x / tileSize);
    int y1 = (int)(end.y / tileSize);

    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        for (int z = 0; z < grid->depth; z++) {
            if (grid->tiles[z][y0][x0].solide) {
                if (obstructionPoint != NULL) {
                    (*obstructionPoint).x = x0 * tileSize + tileSize / 2;
                    (*obstructionPoint).y = y0 * tileSize + tileSize / 2;
                }
                return SDL_FALSE;
            }
        }

        if (x0 == x1 && y0 == y1) {
            return SDL_TRUE;
        }

        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}
