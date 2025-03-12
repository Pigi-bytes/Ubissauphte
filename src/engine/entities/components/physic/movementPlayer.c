#include "movementPlayer.h"

#define COLLISION_LOG(...) \
    SDL_Log("\033[1;33m[COLLISION]\033[0m " __VA_ARGS__)
#define PHYSICS_LOG(...) \
    SDL_Log("\033[1;34m[PHYSICS]\033[0m " __VA_ARGS__)
#define INPUT_LOG(...) \
    SDL_Log("\033[1;32m[INPUT]\033[0m " __VA_ARGS__)
#define RESOLVE_LOG(...) \
    SDL_Log("\033[1;35m[RESOLVE]\033[0m " __VA_ARGS__)

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

        COLLISION_LOG("Collision Cercle-Rectangle:");
        COLLISION_LOG("  Cercle: (%.2f,%.2f) R=%.2f", circle->x, circle->y, circle->radius);
        COLLISION_LOG("  Rectangle: (%d,%d) %dx%d", rect->x, rect->y, rect->w, rect->h);
        COLLISION_LOG("  Normal: (%.2f,%.2f) Depth=%.2f", out->normal.x, out->normal.y, out->depth);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

void resolveCollisions(t_entity* entity, t_grid* grid) {
    int tileSize = 16;
    int startX = (entity->collisionCircle.x - entity->collisionCircle.radius) / tileSize;
    int startY = (entity->collisionCircle.y - entity->collisionCircle.radius) / tileSize;
    int endX = (entity->collisionCircle.x + entity->collisionCircle.radius) / tileSize;
    int endY = (entity->collisionCircle.y + entity->collisionCircle.radius) / tileSize;

    t_collisionData out;

    for (int z = 0; z < grid->depth; z++) {
        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                t_tile* tile = getTile(grid, x, y, z);
                if (tile->solide) {
                    if (checkCircleRectCollision(&entity->collisionCircle, &tile->entity.collisionRect, &out)) {
                        RESOLVE_LOG("Pre-collision pos: (%.1f,%.1f)", entity->collisionCircle.x, entity->collisionCircle.y);
                        // Résoudre la collision en déplaçant l'entité
                        entity->collisionCircle.x += out.normal.x * out.depth;
                        entity->collisionCircle.y += out.normal.y * out.depth;
                        RESOLVE_LOG("Post-collision pos: (%.1f,%.1f)", entity->collisionCircle.x, entity->collisionCircle.y);
                    }
                }
            }
        }
    }
}

void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, float* deltaTime) {
    float force = 5.0f;

    // Reset de l'accelaration a chaque frame
    player->entity.physics.acceleration.x = 0.0f;
    player->entity.physics.acceleration.y = 0.0f;

    if (input->key[player->control->left]) {
        player->entity.physics.acceleration.x -= force;
        player->entity.flip = SDL_FLIP_HORIZONTAL;
    }
    if (input->key[player->control->right]) {
        player->entity.physics.acceleration.x += force;
        player->entity.flip = SDL_FLIP_NONE;
    }
    if (input->key[player->control->up]) player->entity.physics.acceleration.y -= force;
    if (input->key[player->control->down]) player->entity.physics.acceleration.y += force;

    // Dash
    if (input->key[SDL_SCANCODE_SPACE]) {
        if (player->entity.flip == SDL_FLIP_NONE) {
            player->entity.physics.acceleration.x += 4000;
        } else {
            player->entity.physics.acceleration.x -= 4000;
        }
    }

    if (player->entity.physics.acceleration.x != 0.0f || player->entity.physics.acceleration.y != 0.0f) {
        setAnimation(player->entity.animationController, "walk");
    } else {
        setAnimation(player->entity.animationController, "idle");
    }

    INPUT_LOG("Accélération finale: (%.1f,%.1f)", player->entity.physics.acceleration.x, player->entity.physics.acceleration.y);
}

// masse que dans les collisions avec d'autre object

void updatePhysics(t_joueur* joueur, float* deltaTime, t_grid* grid) {
    // // Schéma d'intégration (pseudo-code)
    // Velocity(t+dt) = Velocity(t) + (Acceleration(t)/Mass) * dt
    // Position(t+dt) = Position(t) + Velocity(t) * dt

    PHYSICS_LOG("Pre-update - Velocity: (%.1f,%.1f) Pos: (%.1f,%.1f)", joueur->entity.physics.velocity.x, joueur->entity.physics.velocity.y, joueur->entity.collisionCircle.x, joueur->entity.collisionCircle.y);

    SDL_Point lastPosVisuelle = {joueur->entity.collisionCircle.x, joueur->entity.collisionCircle.y};
    // Application de l'accéleration a la velocite : velocity += (acceleratio) * deltaTime
    // Division par mass pour simuler une certaine interie differente selon la masse
    // DeltaTime pour conversion force par frame en force par seconde
    joueur->entity.physics.velocity.x += (joueur->entity.physics.acceleration.x);
    joueur->entity.physics.velocity.y += (joueur->entity.physics.acceleration.y);
    PHYSICS_LOG("After acceleration - Velocity: (%.1f,%.1f)", joueur->entity.physics.velocity.x, joueur->entity.physics.velocity.y);

    // Reduction progressive de la velocite (1.0 = 100% de friction) pour simuler les frottements
    joueur->entity.physics.velocity.x *= (1.0f - joueur->entity.physics.friction);
    joueur->entity.physics.velocity.y *= (1.0f - joueur->entity.physics.friction);

    PHYSICS_LOG("After friction - Velocity: (%.1f,%.1f)", joueur->entity.physics.velocity.x, joueur->entity.physics.velocity.y);

    // Intégration de la vélocité : position += velocity * deltaTime
    joueur->entity.collisionCircle.x += joueur->entity.physics.velocity.x * *deltaTime;
    joueur->entity.collisionCircle.y += joueur->entity.physics.velocity.y * *deltaTime;

    // On resout les collisions
    resolveCollisions(&joueur->entity, grid);

    joueur->entity.displayRect.x += joueur->entity.collisionCircle.x - lastPosVisuelle.x;
    joueur->entity.displayRect.y += joueur->entity.collisionCircle.y - lastPosVisuelle.y;

    PHYSICS_LOG("New position: (%.1f,%.1f)", joueur->entity.collisionCircle.x, joueur->entity.collisionCircle.y);
}

SDL_bool checkCircleCircleCollision(t_circle* a, t_circle* b, t_collisionData* out) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float distanceSquared = dx * dx + dy * dy;  // On utilise le carré de la distance
    float radii = a->radius + b->radius;
    float radiiSquared = radii * radii;  // On utilise le carré de la somme des rayons

    if (distanceSquared < radiiSquared) {
        float distance = sqrtf(distanceSquared);  // On ne calcule sqrtf que si nécessaire
        out->depth = radii - distance;
        out->normal.x = dx / distance;
        out->normal.y = dy / distance;
        return SDL_TRUE;
    }
    return SDL_FALSE;
}