#include "physics.h"

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