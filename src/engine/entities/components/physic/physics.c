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