#include "physics.h"

#include <math.h>

#include "../../../../utils/objectManager.h"
#include "../../entity.h"
#include "../../tiles.h"

// ... rest of the code remains the same ...
SDL_bool checkCircleRectCollision(t_circle* circle, SDL_Rect* rect, t_collisionData* out) {
    // Calcul du point le plus proche sur le rectangle par rapport au centre du cercl,
    // On "contraint" la position du centre du cercle dans les limites du rectangle
    // / C'est une projection du centre sur les bords du rectangle
    float closestX = fmaxf(rect->x, fminf(circle->x, rect->x + rect->w));
    float closestY = fmaxf(rect->y, fminf(circle->y, rect->y + rect->h));

    // Calcul de la distance entre le centre du cercle et le point projete
    // C'est le vecteur qui va du point sur le rectangle vers le centre du cercle
    float distanceX = circle->x - closestX;
    float distanceY = circle->y - closestY;

    // On calcule la distance au carré pour éviter un appel a sqrtf qui est couteux
    float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    // Collision : si la distance au carré est inférieure au rayon au carre,
    // cela signifie que le cercle et le rectangle se chevauchent. -> c^2 > d^2 <=> c > d
    if (distanceSquared < (circle->radius * circle->radius)) {
        // On calcul la vraie distance maintenant que on sais que collision
        float distance = SDL_sqrtf(distanceSquared);

        // On calcul le vecteur normal (la direction vers laquelle le rectangle nous repousse)
        // On inverse le vecteur distance pour pointer hors du rectangle
        // c'est la direction dans laquelle il faut pousser le cercle pour annuler la collision
        out->normal.x = (circle->x - closestX) / distance;
        out->normal.y = (circle->y - closestY) / distance;

        // Calcul de la profondeur de penetration, de "combien" le cercle est rentre dans le rectangle
        // Depth est positif, Si distance = 5 et radius = 8 -> depth = 3 (le cercle a penetre de 3)
        // C'est la longeur qu'il faut multiplier avec la direction (donc la normal) pour sortir le cercle du rect
        out->depth = circle->radius - distance;

        // COLLISION_LOG("Collision Cercle-Rectangle:");
        // COLLISION_LOG("  Cercle: (%.2f,%.2f) R=%.2f", circle->x, circle->y, circle->radius);
        // COLLISION_LOG("  Rectangle: (%d,%d) %dx%d", rect->x, rect->y, rect->w, rect->h);
        // COLLISION_LOG("  Normal: (%.2f,%.2f) Depth=%.2f", out->normal.x, out->normal.y, out->depth);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool checkCircleCircleCollision(t_circle* a, t_circle* b, t_collisionData* out) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float distanceSquared = dx * dx + dy * dy;  // On utilise le carré de la distance
    float radii = a->radius + b->radius;
    float radiiSquared = radii * radii;  // On utilise le carré de la somme des rayons

    if (distanceSquared < radiiSquared) {
        float distance = sqrtf(distanceSquared);  // On ne calcule sqrtf que si nécessaire
        if (out != NULL) {
            out->depth = radii - distance;
            out->normal.x = dx / distance;
            out->normal.y = dy / distance;
        }
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

SDL_bool cercleInSector(SDL_FPoint target, float target_radius, SDL_FPoint origin, float currentAngle, float range, float arc) {
    // Calcul du vecteur différence entre la cible et l'origine
    float dx = target.x - origin.x;
    float dy = target.y - origin.y;

    // Vérification rapide de la distance au carré (évite sqrt qui est lourd)
    // On utilise le carré de la distance pour comparer avec le carré du rayon maximum
    float distSq = dx * dx + dy * dy;

    // Distance maximale possible = portée du secteur + rayon de la cible
    // (permet de détecter les cibles qui touchent le bord extérieur du secteur)
    float maxDistSq = (range + target_radius) * (range + target_radius);

    // Si la distance au carré est supérieure à la distance maximale au carré,
    // alors la cible est hors de portée
    if (distSq > maxDistSq)
        return SDL_FALSE;

    // Si la cible englobe l'origine (sa distance est inférieure à son rayon),
    // alors elle intersecte forcément le secteur quel que soit l'angle
    if (distSq <= target_radius * target_radius)
        return SDL_TRUE;

    // Maintenant que les cas extremes sont traités, on calcule la vraie distance
    float distance = sqrtf(distSq);

    // Calcul de l'angle vers la cible (en radians)
    float targetAngle = atan2f(dy, dx);

    // Normalisation optimisée de la différence d'angle
    // Cela garantit que angleDiff est toujours dans l'intervalle [-pi, pi]
    // (important pour comparer correctement les angles)
    float angleDiff = targetAngle - currentAngle;
    if (angleDiff > M_PI) angleDiff -= 2 * M_PI;
    if (angleDiff < -M_PI) angleDiff += 2 * M_PI;

    // Extension de l'angle autorisé pour tenir compte du rayon de la cible
    // Plus la cible est proche, plus son rayon occupe un angle important vu depuis l'origine
    // theta = arcsin(r/d) = angle sous lequel on voit le rayon de la cible depuis l'origine
    float theta = asinf(fminf(target_radius / distance, 1.0f));  // Prévention contre les valeurs >1

    // L'arc effectif est la moitié de l'arc total + l'angle supplémentaire dû au rayon
    float half_arc = arc / 2.0f + theta;

    // L'angle est-il dans l'arc d'attaque étendu?
    // On utilise la valeur absolue car l'arc est symétrique par rapport à currentAngle
    return fabsf(angleDiff) <= half_arc;
}

void updatePhysicEntity(t_entity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities) {
    SDL_Point lastPosVisuelle = {entity->collisionCircle.x, entity->collisionCircle.y};

    // Application de la friction
    // La friction est un pourcentage de reduction de la velocite par seconde
    // (1.0 = 100% de friction, arret immediat, 0.0 = pas de friction)
    entity->physics.velocity.x *= powf(entity->physics.friction, *deltaTime);
    entity->physics.velocity.y *= powf(entity->physics.friction, *deltaTime);

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