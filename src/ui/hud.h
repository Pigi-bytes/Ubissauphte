#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../engine/entities/player.h"
#include "text.h"

#define HUD_PADDING 10
#define HEALTH_BAR_HEIGHT 24
#define XP_BAR_HEIGHT 6
#define BAR_GAP 4
#define ICON_SIZE 32
#define BAR_WIDTH 250

typedef struct {
    TTF_Font* font;
    SDL_Texture* player_icon;
    SDL_Texture* weapon_icon;
    SDL_Texture* dash_icon;

    struct {
        SDL_Rect player;
        SDL_Rect health_bar;
        SDL_Rect xp_bar;
        SDL_Rect weapon;
        SDL_Rect dash;
    } layout;

    struct {
        t_text* health;
        t_text* level;
    } text;

    struct {
        float health_ratio;
        float target_health_ratio;
        float xp_ratio;
        float target_xp_ratio;
        float weapon_cooldown;
        float dash_cooldown;
    } state;

    SDL_Color text_color;
    SDL_Color outline_color;
    SDL_bool initialized;
} t_hud;

t_hud* createHUD(SDL_Renderer* renderer, TTF_Font* font, t_tileset* tileset);
void updateHUD(t_hud* hud, SDL_Renderer* renderer, t_joueur* player);
void renderHUD(SDL_Renderer* renderer, t_hud* hud);

void freeHUD(void* hud);
void updateHUDWrapper(t_fonctionParam* f);
void renderHUDWrapper(t_fonctionParam* f);

#endif