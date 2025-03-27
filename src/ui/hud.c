#include "hud.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_hud* createHUD(SDL_Renderer* renderer, TTF_Font* font, t_tileset* tileset) {
    t_hud* hud = malloc(sizeof(t_hud));
    if (!hud) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le HUD\n");
        return NULL;
    }

    // Configuration de base
    hud->font = font;
    hud->textColor = (SDL_Color){255, 255, 255, 255};
    hud->outlineColor = (SDL_Color){0, 0, 0, 255};
    hud->padding = 10;

    // Layout des éléments
    hud->playerIconRect = (SDL_Rect){hud->padding, hud->padding, 48, 48};

    // Barres plus fines qui ensemble font la même hauteur que l'icône
    int healthBarHeight = 24;  // Barre de vie plus fine
    int xpBarHeight = 6;       // Barre d'XP très fine
    int gapBetweenBars = 4;    // Espace entre les barres

    // La somme des hauteurs + écart doit égaler la hauteur de l'icône
    // 24 + 6 + 4 = 34 < 48, donc il y aura un petit espace en dessous (ce qui est bien)

    // Positionner la barre de vie en haut, alignée avec l'icône du joueur
    hud->healthBarRect = (SDL_Rect){
        hud->padding + hud->playerIconRect.w + 10,
        hud->padding,  // Aligné au sommet de l'icône
        250,
        healthBarHeight};

    // Positionner la barre d'XP juste en dessous de la barre de vie
    hud->xpBarRect = (SDL_Rect){
        hud->healthBarRect.x,
        hud->healthBarRect.y + hud->healthBarRect.h + gapBetweenBars,
        hud->healthBarRect.w,
        xpBarHeight};

    // L'indicateur d'arme est placé en dessous avec un espacement
    hud->weaponIndicatorRect = (SDL_Rect){
        hud->healthBarRect.x,
        hud->xpBarRect.y + hud->xpBarRect.h + 10,
        32, 32};

    hud->dashIconRect = (SDL_Rect){
        hud->healthBarRect.x + hud->weaponIndicatorRect.w,  // Aligné en X avec la barre de vie
        hud->xpBarRect.y + hud->xpBarRect.h + 10,           // Sous la barre de vie avec un décalage
        32, 32                                              // Taille de l'icône
    };

    hud->dashIcon = getObject(tileset->textureTiles, 142);  // Charger l'icône de dash
    hud->dashCooldownRatio = 0.0f;

    // Initialisation des valeurs
    hud->xpRatio = 0.0f;
    hud->targetXpRatio = 0.0f;
    hud->weaponCooldownRatio = 0.0f;
    hud->initialized = SDL_FALSE;
    hud->healthText = NULL;
    hud->levelText = NULL;
    hud->currentWeaponIcon = NULL;
    hud->playerIcon = NULL;
    return hud;
}

// Dans la fonction updateHUD, mettons le texte en blanc
void updateHUD(t_hud* hud, SDL_Renderer* renderer, t_joueur* player) {
    if (!hud || !renderer || !player) return;

    // Récupération des données du joueur
    char healthStr[16];
    snprintf(healthStr, sizeof(healthStr), "%d/%d", player->health.currentHealth, player->health.maxHealth);
    hud->playerIcon = player->entity.texture;
    hud->targetXpRatio = getPlayerXPProgress(player);

    if (player->currentWeapon) {
        hud->currentWeaponIcon = player->currentWeapon->texture;
        hud->weaponCooldownRatio = player->attack.cooldown > 0 ? player->attack.cooldown / player->currentWeapon->attackCooldown : 0;
    }

    if (!hud->initialized) {
        // Première initialisation des textes - En blanc avec contour noir
        hud->healthText = createTextOutline(renderer, healthStr, hud->font,
                                            (SDL_Color){255, 255, 255, 255},
                                            (SDL_Color){0, 0, 0, 180}, 1);

        // Centrer horizontalement et verticalement dans la barre de vie plus fine
        hud->healthText->rect.x = hud->healthBarRect.x + (hud->healthBarRect.w - hud->healthText->rect.w) / 2;
        hud->healthText->rect.y = hud->healthBarRect.y + (hud->healthBarRect.h - hud->healthText->rect.h) / 2;

        char levelStr[8];
        snprintf(levelStr, sizeof(levelStr), "%d", player->level);
        hud->levelText = createTextOutline(renderer, levelStr, hud->font, hud->textColor, hud->outlineColor, 1);
        hud->levelText->rect.x = hud->playerIconRect.x + (hud->playerIconRect.w - hud->levelText->rect.w) / 2;
        hud->levelText->rect.y = hud->playerIconRect.y + hud->playerIconRect.h + 5;

        hud->initialized = SDL_TRUE;
    } else {
        // Mise à jour des textes existants - En blanc avec contour noir
        updateTextOutline(&hud->healthText, renderer, healthStr,
                          (SDL_Color){255, 255, 255, 255},
                          (SDL_Color){0, 0, 0, 180}, 1);

        // Ajuster la position du texte de santé pour la barre plus fine
        hud->healthText->rect.x = hud->healthBarRect.x + (hud->healthBarRect.w - hud->healthText->rect.w) / 2;
        hud->healthText->rect.y = hud->healthBarRect.y + (hud->healthBarRect.h - hud->healthText->rect.h) / 2;

        char levelStr[8];
        snprintf(levelStr, sizeof(levelStr), "%d", player->level);
        updateTextOutline(&hud->levelText, renderer, levelStr, hud->textColor, hud->outlineColor, 1);
        hud->levelText->rect.x = hud->playerIconRect.x + (hud->playerIconRect.w - hud->levelText->rect.w) / 2;
    }

    Uint32 elapsed = getTicks(player->dash.cooldownTimer);
    if (elapsed < player->dash.cooldownTime) {
        hud->dashCooldownRatio = 1.0f - (float)elapsed / player->dash.cooldownTime;
    } else {
        hud->dashCooldownRatio = 0.0f;
    }
}

void renderXPBar(SDL_Renderer* renderer, SDL_Rect barRect, float xpRatio) {
    if (!renderer) return;

    // Variables d'animation
    static float time = 0.0f;
    time += 0.016f;  // ~60 FPS

    // Effet subtil de scintillement pour l'XP
    float shimmerIntensity = 0.7f + (sinf(time * 3.0f) * 0.3f);

    // ---- Contour léger pour la barre d'XP ----
    SDL_Rect outerBorder = barRect;
    outerBorder.x -= 1;
    outerBorder.y -= 1;
    outerBorder.w += 2;
    outerBorder.h += 2;

    // Contour fin bleu foncé
    SDL_SetRenderDrawColor(renderer, 30, 35, 80, 255);
    SDL_RenderDrawRect(renderer, &outerBorder);

    // Fond plus foncé et légèrement transparent
    SDL_SetRenderDrawColor(renderer, 15, 20, 50, 200);
    SDL_RenderFillRect(renderer, &barRect);

    // Si la barre est vide, ne pas dessiner le remplissage
    if (xpRatio <= 0.0f) return;

    // Calcul de la partie remplie
    SDL_Rect fillRect = barRect;
    fillRect.w = (int)(barRect.w * xpRatio);

    // Palette de couleurs bleutées pour l'XP
    Uint8 baseR = 45;
    Uint8 baseG = 120;
    Uint8 baseB = 255;

    // Dégradé horizontal pour la barre d'XP (plus clair vers la droite)
    for (int x = 0; x < fillRect.w; x++) {
        // Facteur de position (0.0 à 1.0)
        float posFactor = x / (float)fillRect.w;

        // Léger dégradé de gauche à droite
        float brightness = 0.85f + (posFactor * 0.3f);

        // Effet de scintillement en fonction de la position
        float sparkle = shimmerIntensity * (0.9f + 0.1f * sinf(posFactor * 10.0f + time * 2.0f));

        // Couleur finale avec effets
        Uint8 r = (Uint8)(baseR * brightness * sparkle);
        Uint8 g = (Uint8)(baseG * brightness * sparkle);
        Uint8 b = (Uint8)(baseB * brightness * sparkle);

        // Dessiner une ligne verticale de cette couleur
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_Rect lineRect = {fillRect.x + x, fillRect.y, 1, fillRect.h};
        SDL_RenderFillRect(renderer, &lineRect);
    }

    // Segments stylisés pour effet de jauge
    int segmentCount = 10;
    int segmentWidth = barRect.w / segmentCount;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 1; i < segmentCount; i++) {
        int x = barRect.x + i * segmentWidth;

        // Ne dessiner les segments que sur la partie non remplie
        if (x < fillRect.x + fillRect.w) continue;

        SDL_SetRenderDrawColor(renderer, 50, 70, 120, 70);
        SDL_Rect segRect = {x, barRect.y, 1, barRect.h};
        SDL_RenderFillRect(renderer, &segRect);
    }

    // Reflet lumineux en haut de la barre
    SDL_SetRenderDrawColor(renderer, 150, 190, 255, 120);
    SDL_Rect highlightRect = {fillRect.x, fillRect.y, fillRect.w, 1};
    SDL_RenderFillRect(renderer, &highlightRect);

    // Effet de brillance qui se déplace (pour donner un sentiment de progression)
    float glowPos = fmodf(time * 0.3f, 2.0f) - 0.5f;  // -0.5 à 1.5
    if (glowPos > 0.0f && glowPos < 1.0f && fillRect.w > 10) {
        int glowX = fillRect.x + (int)(glowPos * fillRect.w);
        int glowWidth = fillRect.w / 7;

        for (int i = 0; i < glowWidth; i++) {
            float intensity = 1.0f - fabsf((i - glowWidth / 2.0f) / (glowWidth / 2.0f));
            SDL_SetRenderDrawColor(renderer, 200, 225, 255, (Uint8)(80 * intensity));

            if (glowX + i >= fillRect.x && glowX + i < fillRect.x + fillRect.w) {
                SDL_Rect glowPixel = {glowX + i, fillRect.y, 1, fillRect.h};
                SDL_RenderFillRect(renderer, &glowPixel);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderHealthBar(SDL_Renderer* renderer, SDL_Rect barRect, float healthRatio) {
    if (!renderer) return;

    // Variables pour une légère pulsation
    static float time = 0.0f;
    time += 0.016f;  // Approximation ~60 FPS
    float pulseIntensity = 0.85f + (sinf(time * 0.005f) * 0.15f);

    // ---- Contour extérieur stylisé ----

    // Créer une bordure plus grande pour avoir un effet 3D
    SDL_Rect outerBorder = barRect;
    outerBorder.x -= 2;
    outerBorder.y -= 2;
    outerBorder.w += 4;
    outerBorder.h += 4;

    // Fond du contour avec léger dégradé
    for (int i = 0; i < 2; i++) {
        SDL_Rect borderLine = outerBorder;
        borderLine.y += i;
        borderLine.h = 1;
        // Plus foncé en haut, plus clair en bas
        Uint8 borderBrightness = 30 + i * 25;
        SDL_SetRenderDrawColor(renderer, borderBrightness, borderBrightness, borderBrightness, 255);
        SDL_RenderFillRect(renderer, &borderLine);

        // Faire la même chose pour le bas
        borderLine.y = outerBorder.y + outerBorder.h - 1 - i;
        // Plus clair en bas, plus foncé en haut
        borderBrightness = 80 - i * 25;
        SDL_SetRenderDrawColor(renderer, borderBrightness, borderBrightness, borderBrightness, 255);
        SDL_RenderFillRect(renderer, &borderLine);
    }

    // Côtés du contour
    for (int i = 0; i < 2; i++) {
        SDL_Rect borderLine = outerBorder;
        borderLine.x += i;
        borderLine.w = 1;
        // Plus foncé à gauche, plus clair à droite
        Uint8 borderBrightness = 40 + i * 20;
        SDL_SetRenderDrawColor(renderer, borderBrightness, borderBrightness, borderBrightness, 255);
        SDL_RenderFillRect(renderer, &borderLine);

        // Faire la même chose pour la droite
        borderLine.x = outerBorder.x + outerBorder.w - 1 - i;
        // Plus clair à droite, plus foncé à gauche
        borderBrightness = 70 - i * 20;
        SDL_SetRenderDrawColor(renderer, borderBrightness, borderBrightness, borderBrightness, 255);
        SDL_RenderFillRect(renderer, &borderLine);
    }

    // 1. Fond sombre semi-transparent
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 180);
    SDL_RenderFillRect(renderer, &barRect);

    // Si la barre est vide, ne pas dessiner le remplissage
    if (healthRatio <= 0.0f) return;

    // 3. Déterminer la couleur en fonction du niveau de santé
    Uint8 r, g, b;
    if (healthRatio > 0.75f) {
        // Vert
        r = 0;
        g = 255;
        b = 0;
    } else if (healthRatio > 0.5f) {
        // Jaune-vert
        r = 180;
        g = 255;
        b = 0;
    } else if (healthRatio > 0.25f) {
        // Orange
        r = 255;
        g = 165;
        b = 0;
    } else {
        // Rouge
        r = 255;
        g = 50;
        b = 50;
    }

    // 4. Dessiner la partie remplie avec une marge minimale pour voir le contour
    SDL_Rect fillRect = barRect;
    fillRect.x += 2;                                    // Marge horizontale de 2px
    fillRect.y += 2;                                    // Marge verticale de 2px
    fillRect.w = (int)((barRect.w - 4) * healthRatio);  // Ajuster pour la marge
    fillRect.h -= 4;                                    // Ajuster pour la marge

    // Protection contre les valeurs négatives
    if (fillRect.w < 0) fillRect.w = 0;

    // Appliquer la pulsation à la couleur
    SDL_SetRenderDrawColor(renderer,
                           (Uint8)(r * pulseIntensity),
                           (Uint8)(g * pulseIntensity),
                           (Uint8)(b * pulseIntensity),
                           255);
    SDL_RenderFillRect(renderer, &fillRect);

    // Ajouter un joli effet de brillance au sommet de la barre
    if (fillRect.w > 0) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        SDL_Rect highlightRect = {fillRect.x, fillRect.y, fillRect.w, 1};
        SDL_RenderFillRect(renderer, &highlightRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}

// Modification de la fonction renderHUD pour utiliser notre nouvelle fonction
void renderHUD(SDL_Renderer* renderer, t_hud* hud) {
    if (!hud || !renderer || !hud->initialized) return;

    // Animation de la barre d'XP
    if (hud->xpRatio < hud->targetXpRatio) {
        hud->xpRatio += (hud->targetXpRatio - hud->xpRatio) * 0.05f;
        if (hud->targetXpRatio - hud->xpRatio < 0.001f) {
            hud->xpRatio = hud->targetXpRatio;
        }
    } else if (hud->xpRatio > hud->targetXpRatio) {
        hud->xpRatio = hud->targetXpRatio;
    }

    // Cadre et icône du joueur
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect playerIconBorder = {
        hud->playerIconRect.x - 2,
        hud->playerIconRect.y - 2,
        hud->playerIconRect.w + 4,
        hud->playerIconRect.h + 4};
    SDL_RenderFillRect(renderer, &playerIconBorder);

    if (hud->playerIcon) {
        SDL_RenderCopy(renderer, hud->playerIcon, NULL, &hud->playerIconRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &hud->playerIconRect);
    }

    // Analyse du texte de santé pour obtenir les valeurs
    int currentHealth = 0, maxHealth = 1;
    if (hud->healthText && hud->healthText->text) {
        sscanf(hud->healthText->text, "%d/%d", &currentHealth, &maxHealth);
    }

    float healthRatio = (float)currentHealth / maxHealth;

    // Rendu de la barre de vie avec notre nouvelle fonction
    renderHealthBar(renderer, hud->healthBarRect, healthRatio);  // SDL_TRUE pour utiliser le dégradé

    // Affichage du texte de vie en semi-transparent
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    renderText(renderer, hud->healthText);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Barre d'XP
    SDL_SetRenderDrawColor(renderer, 30, 30, 70, 255);
    SDL_RenderFillRect(renderer, &hud->xpBarRect);

    SDL_SetRenderDrawColor(renderer, 45, 105, 255, 255);
    SDL_Rect fillXpRect = hud->xpBarRect;
    fillXpRect.w = (int)(hud->xpBarRect.w * hud->xpRatio);
    SDL_RenderFillRect(renderer, &fillXpRect);

    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    SDL_RenderDrawRect(renderer, &hud->xpBarRect);

    // Affichage du niveau
    renderText(renderer, hud->levelText);

    // Affichage de l'arme et cooldown
    if (hud->currentWeaponIcon) {
        SDL_RenderCopy(renderer, hud->currentWeaponIcon, NULL, &hud->weaponIndicatorRect);

        if (hud->weaponCooldownRatio > 0.0f) {
            SDL_Rect cooldownRect = hud->weaponIndicatorRect;
            cooldownRect.h = (int)(hud->weaponIndicatorRect.h * hud->weaponCooldownRatio);
            cooldownRect.y = hud->weaponIndicatorRect.y + hud->weaponIndicatorRect.h - cooldownRect.h;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, &cooldownRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
    }

    if (hud->dashIcon) {
        SDL_RenderCopy(renderer, hud->dashIcon, NULL, &hud->dashIconRect);

        if (hud->dashCooldownRatio > 0.0f) {
            SDL_Rect cooldownOverlay = hud->dashIconRect;

            // Calculer la hauteur de l'overlay en fonction du cooldown
            cooldownOverlay.h = (int)(hud->dashIconRect.h * hud->dashCooldownRatio);

            // Ajuster la position Y pour que l'overlay commence en bas et monte
            cooldownOverlay.y = hud->dashIconRect.y + hud->dashIconRect.h - cooldownOverlay.h;

            // Dessiner l'overlay
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);  // Couleur sombre semi-transparente
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, &cooldownOverlay);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
    }
}

void freeHUD(void* object) {
    t_hud* hud = (t_hud*)object;
    if (!hud) return;

    if (hud->healthText) {
        freeText(hud->healthText);
        free(hud->healthText);
    }

    if (hud->levelText) {
        freeText(hud->levelText);
        free(hud->levelText);
    }

    free(hud);
}

void updateHUDWrapper(t_fonctionParam* f) {
    updateHUD(GET_PTR(f, 0, t_hud*), GET_PTR(f, 1, SDL_Renderer*), GET_PTR(f, 2, t_joueur*));
}

void renderHUDWrapper(t_fonctionParam* f) {
    renderHUD(GET_PTR(f, 0, SDL_Renderer*), GET_PTR(f, 1, t_hud*));
}