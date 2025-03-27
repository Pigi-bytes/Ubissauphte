#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../engine/entities/player.h"
#include "text.h"

typedef struct {
    TTF_Font* font;
    SDL_Color textColor;
    SDL_Color outlineColor;

    int padding;
    SDL_Rect playerIconRect;
    SDL_Rect healthBarRect;
    SDL_Rect xpBarRect;
    SDL_Rect weaponIndicatorRect;

    t_text* healthText;
    t_text* levelText;  // Suppression de weaponNameText
    SDL_Texture* currentWeaponIcon;
    SDL_Texture* playerIcon;

    SDL_Rect dashIconRect;    // Rectangle pour l'icône de dash
    SDL_Texture* dashIcon;    // Texture de l'icône de dash
    float dashCooldownRatio;  // Ratio de cooldown pour le dash

    float xpRatio;        // Ratio d'XP actuel pour l'affichage
    float targetXpRatio;  // Ratio d'XP cible (valeur réelle)
    float xpAnimSpeed;    // Vitesse d'animation de la barre d'XP

    float weaponCooldownRatio;  // Ratio de cooldown pour l'arme

    SDL_bool initialized;
} t_hud;

t_hud* createHUD(SDL_Renderer* renderer, TTF_Font* font, t_tileset* tileset);
void updateHUD(t_hud* hud, SDL_Renderer* renderer, t_joueur* player);
void renderHUD(SDL_Renderer* renderer, t_hud* hud);

void freeHUD(void* hud);
void updateHUDWrapper(t_fonctionParam* f);
void renderHUDWrapper(t_fonctionParam* f);

#endif