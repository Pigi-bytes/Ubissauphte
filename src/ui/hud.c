#include "hud.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ANIMATION_SPEED 0.05f
#define EPSILON 0.001f
#define HEALTH_BAR_PADDING 2
#define HEALTH_BAR_BORDER 2
#define HEALTH_HIGHLIGHT_HEIGHT 3

const SDL_Color HEALTH_BG = {40, 40, 40, 255};
const SDL_Color HEALTH_BORDER = {80, 80, 80, 255};
const SDL_Color HEALTH_HIGHLIGHT = {255, 255, 255, 100};
const SDL_Color XP_BG = {15, 20, 50, 200};
const SDL_Color XP_BORDER = {30, 35, 80, 255};

void update_text(t_text** text, SDL_Renderer* renderer, char* content, TTF_Font* font, SDL_Color fg, SDL_Color bg, int outline) {
    if (*text == NULL) {
        *text = createTextOutline(renderer, content, font, fg, bg, outline);
    } else {
        updateTextOutline(text, renderer, content, fg, bg, outline);
    }
}

t_hud* createHUD(SDL_Renderer* renderer, TTF_Font* font, t_tileset* tileset) {
    t_hud* hud = calloc(1, sizeof(t_hud));
    if (!hud) return NULL;

    hud->font = font;
    hud->text_color = (SDL_Color){255, 255, 255, 255};
    hud->outline_color = (SDL_Color){0, 0, 0, 180};

    hud->layout.player = (SDL_Rect){HUD_PADDING, HUD_PADDING, 48, 48};
    hud->layout.health_bar = (SDL_Rect){HUD_PADDING + 48 + HUD_PADDING, HUD_PADDING, BAR_WIDTH, HEALTH_BAR_HEIGHT};
    hud->layout.xp_bar = (SDL_Rect){hud->layout.health_bar.x, hud->layout.health_bar.y + HEALTH_BAR_HEIGHT + BAR_GAP, BAR_WIDTH, XP_BAR_HEIGHT};
    hud->layout.weapon = (SDL_Rect){hud->layout.health_bar.x, hud->layout.xp_bar.y + XP_BAR_HEIGHT + HUD_PADDING, ICON_SIZE, ICON_SIZE};
    hud->layout.dash = (SDL_Rect){hud->layout.weapon.x + ICON_SIZE + HUD_PADDING, hud->layout.weapon.y, ICON_SIZE, ICON_SIZE};

    hud->dash_icon = getObject(tileset->textureTiles, 139);

    hud->state.health_ratio = 0.0f;
    hud->state.xp_ratio = 0.0f;
    hud->state.target_xp_ratio = 0.0f;
    hud->state.weapon_cooldown = 0.0f;
    hud->state.dash_cooldown = 0.0f;

    return hud;
}

void updateHUD(t_hud* hud, SDL_Renderer* renderer, t_joueur* player) {
    char health_str[16];
    snprintf(health_str, sizeof(health_str), "%d/%d", player->health.currentHealth, player->health.maxHealth);
    update_text(&hud->text.health, renderer, health_str, hud->font, hud->text_color, hud->outline_color, 1);
    hud->text.health->rect.x = hud->layout.health_bar.x + (hud->layout.health_bar.w - hud->text.health->rect.w) / 2;
    hud->text.health->rect.y = hud->layout.health_bar.y + (HEALTH_BAR_HEIGHT - hud->text.health->rect.h) / 2;

    char level_str[8];
    snprintf(level_str, sizeof(level_str), "%d", player->level);
    update_text(&hud->text.level, renderer, level_str, hud->font, hud->text_color, hud->outline_color, 1);
    hud->text.level->rect.x = hud->layout.player.x + (hud->layout.player.w - hud->text.level->rect.w) / 2;
    hud->text.level->rect.y = hud->layout.player.y + hud->layout.player.h + 5;

    hud->state.target_xp_ratio = getPlayerXPProgress(player);
    hud->state.target_health_ratio = (float)player->health.currentHealth / player->health.maxHealth;

    hud->player_icon = player->entity.texture;
    hud->weapon_icon = player->currentWeapon ? player->currentWeapon->texture : NULL;

    Uint32 dash_elapsed = getTicks(player->dash.cooldownTimer);
    hud->state.dash_cooldown = (dash_elapsed < player->dash.cooldownTime) ? 1.0f - (float)dash_elapsed / player->dash.cooldownTime : 0.0f;
    hud->state.weapon_cooldown = player->attack.cooldown > 0 ? player->attack.cooldown / player->currentWeapon->attackCooldown : 0;
}

void render_cooldown(SDL_Renderer* renderer, SDL_Rect rect, float ratio) {
    if (ratio <= 0.0f) return;

    SDL_Rect overlay = {rect.x, rect.y + rect.h - (int)(rect.h * ratio), rect.w, (int)(rect.h * ratio)};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderHealthBar(SDL_Renderer* renderer, SDL_Rect barRect, float* currentRatio, float targetRatio) {
    *currentRatio += (targetRatio - *currentRatio) * ANIMATION_SPEED;
    if (fabs(*currentRatio - targetRatio) < EPSILON) *currentRatio = targetRatio;

    float HEALTH_SEGMENTS[] = {0.75f, 0.5f, 0.25f, 0.0f};
    SDL_Color HEALTH_COLORS[] = {
        {0, 255, 0, 255},    // >75%
        {180, 255, 0, 255},  // 50-75%
        {255, 165, 0, 255},  // 25-50%
        {255, 50, 50, 255}   // <25%
    };

    SDL_Color color = HEALTH_COLORS[3];
    for (int i = 0; i < 3; i++) {
        if (*currentRatio > HEALTH_SEGMENTS[i]) {
            color = HEALTH_COLORS[i];
            break;
        }
    }

    SDL_Rect fillRect = {barRect.x + HEALTH_BAR_PADDING, barRect.y + HEALTH_BAR_PADDING, ((barRect.w - HEALTH_BAR_BORDER) * *currentRatio), barRect.h - HEALTH_BAR_BORDER};

    SDL_SetRenderDrawColor(renderer, HEALTH_BG.r, HEALTH_BG.g, HEALTH_BG.b, HEALTH_BG.a);
    SDL_RenderFillRect(renderer, &barRect);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &fillRect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, HEALTH_HIGHLIGHT.r, HEALTH_HIGHLIGHT.g, HEALTH_HIGHLIGHT.b, HEALTH_HIGHLIGHT.a);
    SDL_RenderFillRect(renderer, &(SDL_Rect){fillRect.x, fillRect.y, fillRect.w, HEALTH_HIGHLIGHT_HEIGHT});

    SDL_SetRenderDrawColor(renderer, HEALTH_BORDER.r, HEALTH_BORDER.g, HEALTH_BORDER.b, HEALTH_BORDER.a);
    SDL_RenderDrawRect(renderer, &barRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderXPBar(SDL_Renderer* renderer, SDL_Rect barRect, float* currentRatio, float targetRatio) {
    if (!renderer) return;

    *currentRatio += (targetRatio - *currentRatio) * ANIMATION_SPEED;
    if (fabs(*currentRatio - targetRatio) < EPSILON) *currentRatio = targetRatio;

    SDL_SetRenderDrawColor(renderer, XP_BG.r, XP_BG.g, XP_BG.b, XP_BG.a);
    SDL_RenderFillRect(renderer, &barRect);

    SDL_Rect fillRect = {barRect.x, barRect.y, (int)(barRect.w * *currentRatio), barRect.h};
    SDL_SetRenderDrawColor(renderer, 45, 120, 255, 255);
    SDL_RenderFillRect(renderer, &fillRect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 150, 190, 255, 120);
    SDL_RenderFillRect(renderer, &(SDL_Rect){fillRect.x, fillRect.y, fillRect.w, 1});

    SDL_SetRenderDrawColor(renderer, XP_BORDER.r, XP_BORDER.g, XP_BORDER.b, XP_BORDER.a);
    SDL_RenderDrawRect(renderer, &barRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderHUD(SDL_Renderer* renderer, t_hud* hud) {
    if (hud->player_icon) {
        SDL_RenderCopy(renderer, hud->player_icon, NULL, &hud->layout.player);
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &hud->layout.player);
    }

    renderHealthBar(renderer, hud->layout.health_bar, &hud->state.health_ratio, hud->state.target_health_ratio);
    renderXPBar(renderer, hud->layout.xp_bar, &hud->state.xp_ratio, hud->state.target_xp_ratio);

    if (hud->weapon_icon) {
        SDL_RenderCopy(renderer, hud->weapon_icon, NULL, &hud->layout.weapon);
        render_cooldown(renderer, hud->layout.weapon, hud->state.weapon_cooldown);
    }

    if (hud->dash_icon) {
        SDL_RenderCopy(renderer, hud->dash_icon, NULL, &hud->layout.dash);
        render_cooldown(renderer, hud->layout.dash, hud->state.dash_cooldown);
    }

    renderText(renderer, hud->text.health);
    renderText(renderer, hud->text.level);
}

void freeHUD(void* object) {
    t_hud* hud = (t_hud*)object;
    if (!hud) return;

    if (hud->text.health) freeText(hud->text.health);
    if (hud->text.level) freeText(hud->text.level);

    free(hud);
}

void updateHUDWrapper(t_fonctionParam* f) {
    updateHUD(GET_PTR(f, 0, t_hud*), GET_PTR(f, 1, SDL_Renderer*), GET_PTR(f, 2, t_joueur*));
}

void renderHUDWrapper(t_fonctionParam* f) {
    renderHUD(GET_PTR(f, 0, SDL_Renderer*), GET_PTR(f, 1, t_hud*));
}