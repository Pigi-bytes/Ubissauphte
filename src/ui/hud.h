#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../engine/entities/player.h"
#include "text.h"

typedef struct {
    TTF_Font* font;
    SDL_Texture* player_icon;
    SDL_Texture* weapon_icon;
    SDL_Texture* dash_icon;
    t_animation* gold_animation;

    struct {
        SDL_Rect player;
        SDL_Rect health_bar;
        SDL_Rect xp_bar;
        SDL_Rect weapon;
        SDL_Rect dash;
        SDL_Rect gold;
    } layout;

    struct {
        t_text* health;
        t_text* level;
        t_text* gold;
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
} t_hud;

t_hud* createHUD(SDL_Renderer* renderer, TTF_Font* font, t_tileset* tileset);
void updateHUD(t_hud* hud, SDL_Renderer* renderer, t_joueur* player);
void renderHUD(SDL_Renderer* renderer, t_hud* hud);

void freeHUD(void* hud);
void updateHUDWrapper(t_fonctionParam* f);
void renderHUDWrapper(t_fonctionParam* f);

#endif