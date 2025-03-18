#include "physicsSystem.h"

void updatePhysicEntity(t_entity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    SDL_Point lastPosVisuelle = {entity->collisionCircle.x, entity->collisionCircle.y};

    // Application de la friction
    // La friction est un pourcentage de reduction de la velocite par seconde
    // (1.0 = 100% de friction, arret immediat, 0.0 = pas de friction)
    entity->physics.velocity.x *= powf(entity->physics.friction, *deltaTime);
    entity->physics.velocity.y *= powf(entity->physics.friction, *deltaTime);
    // PHYSICS_LOG("After friction - Velocity: (%.1f,%.1f)", entity->physics.velocity.x, entity->physics.velocity.y);

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

void resolveCollision(t_entity* entity, t_grid* grid, t_objectManager* entities) {
    int tileSize = 16;
    int startX = (entity->collisionCircle.x - entity->collisionCircle.radius) / tileSize;
    int startY = (entity->collisionCircle.y - entity->collisionCircle.radius) / tileSize;
    int endX = (entity->collisionCircle.x + entity->collisionCircle.radius) / tileSize;
    int endY = (entity->collisionCircle.y + entity->collisionCircle.radius) / tileSize;

    t_collisionData out;

    for (int i = 0; i < entities->count; i++) {
        t_entity* otherEntity = getObject(entities, i);
        if (otherEntity == entity) continue;

        // int x1 = entity->collisionCircle.x;
        // int y1 = entity->collisionCircle.y;
        // int x2 = otherEntity->collisionCircle.x;
        // int y2 = otherEntity->collisionCircle.y;

        SDL_bool collision = checkCircleCircleCollision(&entity->collisionCircle, &otherEntity->collisionCircle, &out);

        SDL_Color lineColor = collision ? (SDL_Color){0, 255, 0, 255} : (SDL_Color){255, 0, 0, 255};
        // Debug_PushLine(x1, y1, x2, y2, 3, lineColor);

        if (collision) {
            // Apply impulse based on the collision normal and depth
            // Assuming the entities have velocity properties (e.g., velocityX, velocityY)
            float relativeVelocityX = otherEntity->physics.velocity.x - entity->physics.velocity.x;
            float relativeVelocityY = otherEntity->physics.velocity.y - entity->physics.velocity.y;

            float normalDotVel = out.normal.x * relativeVelocityX + out.normal.y * relativeVelocityY;

            if (normalDotVel) {
                float combinedRestitution = (entity->physics.restitution + otherEntity->physics.restitution) / 2;  // Only apply impulse if objects are moving towards each other
                float impulseMagnitude = -(1 + combinedRestitution) * normalDotVel;                                // 0.8 is the coefficient of restitution (elasticity)
                impulseMagnitude /= (1 / entity->physics.mass + 1 / otherEntity->physics.mass);                    // Apply based on mass

                // Apply impulse to velocities (scaled by mass)
                entity->physics.velocity.x -= impulseMagnitude / entity->physics.mass * out.normal.x;
                entity->physics.velocity.y -= impulseMagnitude / entity->physics.mass * out.normal.y;

                otherEntity->physics.velocity.x += impulseMagnitude / otherEntity->physics.mass * out.normal.x;
                otherEntity->physics.velocity.y += impulseMagnitude / otherEntity->physics.mass * out.normal.y;
            }

            // Move the entity away from the collision, based on the depth of the collision
            entity->collisionCircle.x += out.normal.x * out.depth;
            entity->collisionCircle.y += out.normal.y * out.depth;
        }

        for (int z = 0; z < grid->depth; z++) {
            for (int y = startY; y <= endY; y++) {
                for (int x = startX; x <= endX; x++) {
                    t_tile* tile = getTile(grid, x, y, z);
                    if (tile && tile->solide) {
                        if (checkCircleRectCollision(&entity->collisionCircle, &tile->entity.collisionRect, &out)) {
                            // RESOLVE_LOG("Pre-collision pos: (%.1f,%.1f)", entity->collisionCircle.x, entity->collisionCircle.y);
                            //  Résoudre la collision en déplaçant l'entité

                            float relativeVelocityX = 0.0f - entity->physics.velocity.x;
                            float relativeVelocityY = 0.0f - entity->physics.velocity.y;
                            float normalDotVel = out.normal.x * relativeVelocityX + out.normal.y * relativeVelocityY;

                            if (normalDotVel) {                                                                                    // Only apply impulse if objects are moving towards each other
                                float combinedRestitution = (entity->physics.restitution + tile->entity.physics.restitution) / 2;  // Only apply impulse if objects are moving towards each other

                                float impulseMagnitude = -(1 + combinedRestitution) * normalDotVel;
                                // Apply impulse to velocities (scaled by mass)
                                entity->physics.velocity.x -= impulseMagnitude * out.normal.x;
                                entity->physics.velocity.y -= impulseMagnitude * out.normal.y;
                            }

                            entity->collisionCircle.x += out.normal.x * out.depth;
                            entity->collisionCircle.y += out.normal.y * out.depth;
                            // RESOLVE_LOG("Post-collision pos: (%.1f,%.1f)", entity->collisionCircle.x, entity->collisionCircle.y);
                        }
                    }
                }
            }
        }
    }
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
