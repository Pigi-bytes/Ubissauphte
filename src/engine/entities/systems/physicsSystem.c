#include "physicsSystem.h"

void updatePhysicEntity(t_entity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    // Schéma d'intégration (pseudo-code)
    //  * 1. vel(t+dt) = vel(t) + a(t) * dt
    //  * 2. pos(t+dt) = pos(t) + vel(t+dt)

    // PHYSICS_LOG("Pre-update - Velocity: (%.1f,%.1f) Pos: (%.1f,%.1f)", entity->physics.velocity.x, entity->physics.velocity.y, entity->collisionCircle.x, entity->collisionCircle.y);

    SDL_Point lastPosVisuelle = {entity->collisionCircle.x, entity->collisionCircle.y};

    // Update de la vélocité avec l'accélération
    // Note: l'acceleration est déjà en unite/s
    entity->physics.velocity.x += (entity->physics.acceleration.x);
    entity->physics.velocity.y += (entity->physics.acceleration.y);
    // PHYSICS_LOG("After acceleration - Velocity: (%.1f,%.1f)", entity->physics.velocity.x, entity->physics.velocity.y);

    // Application de la friction
    // La friction est un pourcentage de reduction de la velocite par seconde
    // (1.0 = 100% de friction, arret immediat, 0.0 = pas de friction)
    entity->physics.velocity.x *= (1.0f - entity->physics.friction);
    entity->physics.velocity.y *= (1.0f - entity->physics.friction);
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

void debugDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_Color currentColor;
    SDL_GetRenderDrawColor(renderer, &currentColor.r, &currentColor.g, &currentColor.b, &currentColor.a);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
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

        bool collision = checkCircleCircleCollision(&entity->collisionCircle, &otherEntity->collisionCircle, &out);

        // SDL_Color lineColor = collision ? (SDL_Color){0, 255, 0, 255, 0} : (SDL_Color){255, 0, 0, 255};

        if (collision) {
            // Apply impulse based on the collision normal and depth
            // Assuming the entities have velocity properties (e.g., velocityX, velocityY)
            float relativeVelocityX = otherEntity->physics.velocity.x - entity->physics.velocity.x;
            float relativeVelocityY = otherEntity->physics.velocity.y - entity->physics.velocity.y;

            float normalDotVel = out.normal.x * relativeVelocityX + out.normal.y * relativeVelocityY;

            if (normalDotVel) {                                                                  // Only apply impulse if objects are moving towards each other
                float impulseMagnitude = -(1 + 1.0f) * normalDotVel;                             // 0.8 is the coefficient of restitution (elasticity)
                impulseMagnitude /= (1 / entity->physics.mass + 1 / otherEntity->physics.mass);  // Apply based on mass

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

                            if (normalDotVel) {  // Only apply impulse if objects are moving towards each other
                                float impulseMagnitude = -(1 + 1.0f) * normalDotVel;
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