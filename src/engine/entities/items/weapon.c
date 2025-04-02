#include "weapon.h"

#include <math.h>

#include "../player.h"

void renderWeaponRangeUI(SDL_Renderer* renderer, t_joueur* player) {
    if (!renderer || !player || !player->currentWeapon) return;

    SDL_FPoint origin = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    float range = player->currentWeapon->range;
    float arc = player->currentWeapon->angleAttack;
    float angle = player->aimAngle;

    // Calcul du carre de la vitesse
    float velocitySq = player->entity.physics.velocity.x * player->entity.physics.velocity.x + player->entity.physics.velocity.y * player->entity.physics.velocity.y;

    // Calcul de l'opacité: plus le joueur va vite, plus l'indicateur est transparent
    // - Minimum: 35 (toujours légèrement visible)
    // - Maximum: 55 (quand le joueur est immobile)
    // Le facteur 8 ajuste la sensibilité au mouvement
    uint8_t maxOpacity = (uint8_t)fmaxf(35, 55 - sqrtf(velocitySq) * 8);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, maxOpacity);

    // Division de l'arc en segments pour un rendu lisse
    const int arcSegments = 10;               // Nombre de segments dans l'arc
    const float arcStep = arc / arcSegments;  // Taille angulaire de chaque segment
    float startAngle = angle - arc / 2;

    for (int i = 0; i < arcSegments; i++) {
        float a1 = startAngle + i * arcStep;
        float a2 = startAngle + (i + 1) * arcStep;

        // Conversion des coordonnées polaires en coordonnées cartésiennes
        // et tracé de la ligne entre les deux points
        SDL_RenderDrawLine(renderer, origin.x + cosf(a1) * range, origin.y + sinf(a1) * range, origin.x + cosf(a2) * range, origin.y + sinf(a2) * range);
    }

    // Ligne directrice (plus légère)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, maxOpacity / 2);
    float aimX = origin.x + cosf(angle) * range;
    float aimY = origin.y + sinf(angle) * range;
    SDL_RenderDrawLine(renderer, origin.x, origin.y, aimX, aimY);

    // Dessin d'un petit cercle plein au point ciblé
    // Rayon 1.8 pixels pour une taille visible mais discrète
    for (int w = 0; w < 1.8f * 2; w++) {
        for (int h = 0; h < 1.8f * 2; h++) {
            int dx = 1.8f - w;
            int dy = 1.8f - h;
            if ((dx * dx + dy * dy) <= (1.8f * 1.8f)) {
                SDL_RenderDrawPoint(renderer, aimX + dx, aimY + dy);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderAttackConeDebug(t_joueur* player) {
    if (!player->currentWeapon) return;

    SDL_FPoint origin = {player->entity.collisionCircle.x, player->entity.collisionCircle.y};
    float range = player->currentWeapon->range;
    float angle = player->aimAngle;
    float arc = player->currentWeapon->angleAttack;

    Debug_PushSector(origin.x, origin.y, angle - arc / 2, angle + arc / 2, range, 20, 2, SDL_COLOR_CYAN);

    SDL_FPoint directionPoint = {origin.x + cosf(angle) * range, origin.y + sinf(angle) * range};
    Debug_PushLine(origin.x, origin.y, directionPoint.x, directionPoint.y, 3, SDL_COLOR_MAGENTA);
}

float calculateWeaponScale(t_joueur* player) {
    // Calcul de l'échelle idéale basée sur la portée de l'arme
    // - La hauteur de l'arme est multipliée par 0.8 pour que la portée visuelle
    //   soit légèrement plus grande que la texture (meilleur retour visuel)
    // - Plus la portée est grande, plus l'arme apparaîtra grande à l'écran
    float idealRangeScale = player->currentWeapon->range / (player->currentWeapon->displayRect.h * 0.8f);

    // Limites pour éviter des échelles extrêmes qui nuiraient à l'expérience
    float minScale = 0.1f;
    float maxScale = 3.0f;

    // fmaxf et fminf assurent que le résultat est dans l'intervalle [minScale, maxScale]
    return fmaxf(minScale, fminf(maxScale, idealRangeScale));
}

void renderWeaponDuringAttack(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip) {
    t_arme* weapon = player->currentWeapon;

    float rawProgress = player->attack.progress;
    float adjustedProgress;

    // Courbes de progression non-linéaires pour des animations plus naturelles
    // Les armes lourdes et légères ont des comportements d'animation différents
    if (weapon->mass >= 5.0f) {
        // Arme lourde: mouvement en 3 phases distinctes
        // 1. Préparation lente (wind-up): 0-25% de la durée
        // 2. Frappe rapide: 25-60% de la durée
        // 3. Récupération lente (wind-down): 60-100% de la durée
        if (rawProgress < 0.25f) {
            // Phase 1: Préparation lente (courbe exponentielle)
            // L'exposant 1.5 donne un démarrage encore plus lent
            adjustedProgress = 0.2f * powf(rawProgress / 0.25f, 1.5f);
        } else if (rawProgress < 0.6f) {
            // Phase 2: Frappe rapide (mouvement principal)
            // L'exposant 0.7 donne une accélération rapide puis ralentissement
            adjustedProgress = 0.2f + 0.6f * powf((rawProgress - 0.25f) / 0.35f, 0.7f);
        } else {
            // Phase 3: Récupération lente (linéaire)
            adjustedProgress = 0.8f + 0.2f * (rawProgress - 0.6f) / 0.4f;  // Récupération lente
        }
    } else {
        // Arme légère: mouvement en 2 phases plus simples
        // 1. Préparation rapide: 0-30% de la durée
        // 2. Frappe constante: 30-100% de la durée
        if (rawProgress < 0.3f) {
            // Phase 1: Préparation rapide (linéaire)
            adjustedProgress = 0.4f * (rawProgress / 0.3f);
        } else {
            // Phase 2: Frappe constante (légère courbe)
            // L'exposant 0.9 donne une vitesse presque constante
            adjustedProgress = 0.4f + 0.6f * powf((rawProgress - 0.3f) / 0.7f, 0.9f);
        }
    }

    // Interpolation entre l'angle de départ et de fin basée sur la progression ajustée
    float baseAngle = lerpAngle(player->attack.hitBox.startAngle, player->attack.hitBox.endAngle, adjustedProgress);

    // Ajout d'une légère oscillation (tremblement de l'arme)
    // 0.05f = amplitude de l'oscillation en radians (environ 3 degrés)
    float oscillation = sinf(rawProgress * M_PI * 2.0f) * 0.05f;

    // Angle final combinant l'interpolation et l'oscillation
    float currentAngle = baseAngle + oscillation;

    SDL_Rect displayRect = {(origin.x - pivotPoint.x), (origin.y - pivotPoint.y), scaledWidth, scaledHeight};

    if (rawProgress > 0.3f && rawProgress < 0.7f) {
        // Calcul de l'étirement avec une courbe sinus
        // Maximum d'étirement au milieu de la phase d'impact (50%)
        // 0.15f = 15% d'étirement maximum
        float stretch = sinf((rawProgress - 0.3f) / 0.4f * M_PI) * 0.15f;

        // Application de l'etirement à la hauteur de l'arme
        displayRect.h = (int)(scaledHeight * (1.0f + stretch));
    }

    // Conversion de l'angle en radians vers degrés pour SDL
    // Ajout de 90° car dans SDL, 0° = droite, alors que dans nos calculs, 0° = est
    float rotationDegrees = currentAngle * 180.0f / M_PI + 90;

    // Seulement visible pendant la phase d'attaque principale pour un meilleur effet visuel
    if ((rawProgress >= 0.25f && rawProgress < 0.6f) || (rawProgress >= 0.3f && rawProgress < 0.7f)) {
        const int numTrails = 4;

        // Calcul du facteur d'intensité de la traînée
        // - Maximum au milieu de la phase d'attaque
        // - Diminue vers le début et la fin de la phase
        float peakPoint = weapon->mass >= 5.0f ? 0.425f : 0.5f;
        float trailIntensity = 1.0f - fabsf((rawProgress - peakPoint) * 4.0f);
        trailIntensity = fmaxf(0.0f, fminf(1.0f, trailIntensity));

        // Configuration du mode de fusion pour les traînées semi-transparentes
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Dessiner plusieurs copies fantômes de l'arme avec une opacité décroissante
        for (int i = 1; i <= numTrails; i++) {
            // Calculer un angle légèrement en arrière par rapport à la position actuelle
            // Plus on s'éloigne, plus l'angle est en arrière dans la trajectoire d'attaque
            float trailFactor = (float)i / numTrails;  // 0.33, 0.66, 1.0 pour 3 traînées
            float trailAngle;

            if (player->attack.hitBox.endAngle > player->attack.hitBox.startAngle) {
                trailAngle = currentAngle - trailFactor * 0.4f;  // Réduit l'angle (sens anti-horaire)
            } else {
                trailAngle = currentAngle + trailFactor * 0.4f;  // Augmente l'angle (sens horaire)
            }

            float trailRotation = trailAngle * 180.0f / M_PI + 90;

            uint8_t alpha = (uint8_t)(180 * (1.0f - trailFactor) * trailIntensity);
            SDL_Texture* trailTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, scaledWidth, scaledHeight);

            SDL_SetTextureBlendMode(trailTexture, SDL_BLENDMODE_BLEND);

            SDL_Texture* currentTarget = SDL_GetRenderTarget(renderer);

            SDL_SetRenderTarget(renderer, trailTexture);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            SDL_RenderCopyEx(renderer, weapon->texture, NULL, &(SDL_Rect){0, 0, scaledWidth, displayRect.h}, 0, &(SDL_Point){pivotPoint.x, pivotPoint.y}, weaponFlip);

            SDL_SetRenderTarget(renderer, currentTarget);

            SDL_Color trailColor = (SDL_Color){200 - (int)(200 * trailFactor), 200, 255, alpha};

            // Application de la couleur à la texture
            SDL_SetTextureColorMod(trailTexture, trailColor.r, trailColor.g, trailColor.b);
            SDL_SetTextureAlphaMod(trailTexture, trailColor.a);

            // Rendu de la trainée
            SDL_RenderCopyEx(renderer, trailTexture, NULL, &displayRect, trailRotation, &pivotPoint, weaponFlip);

            // Nettoyage de la texture temporaire
            SDL_DestroyTexture(trailTexture);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    SDL_RenderCopyEx(renderer, weapon->texture, NULL, &displayRect, rotationDegrees, &pivotPoint, weaponFlip);
}

void renderWeaponIdle(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip) {
    // Facteur de réduction pour l'arme en état idle seulement
    const float idleScaleFactor = 0.7f;

    // Réduction de la taille de l'arme en mode idle
    int reducedHeight = (int)(scaledHeight * idleScaleFactor);

    // Constantes de décalage pour positionner l'arme par rapport au joueur
    const float OFFSET_X = 6.0f;
    const float offsetY = 12.0f;

    // Si le joueur est retourné (regarde à gauche), on inverse le décalage
    float offsetX = player->entity.flip == SDL_FLIP_HORIZONTAL ? -OFFSET_X : OFFSET_X;
    // (assure que l'arme pointe dans la même direction que le regard du joueur)
    weaponFlip = player->entity.flip;

    SDL_Rect displayRect = {(origin.x - pivotPoint.x + offsetX), (origin.y - pivotPoint.y + offsetY), scaledWidth, reducedHeight};
    SDL_RenderCopyEx(renderer, player->currentWeapon->texture, NULL, &displayRect, 0, &pivotPoint, weaponFlip);

    SDL_FPoint visualPivot = {displayRect.x + pivotPoint.x, displayRect.y + pivotPoint.y};
    Debug_PushLine(visualPivot.x - 3, visualPivot.y, visualPivot.x + 3, visualPivot.y, 2, SDL_COLOR_RED);
    Debug_PushLine(visualPivot.x, visualPivot.y - 3, visualPivot.x, visualPivot.y + 3, 2, SDL_COLOR_RED);
}

