#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>

#include "../../../debug.h"
#include "../../../utils/fonctionManager.h"
#include "../entity.h"

typedef struct {
    float mass;
    float damage;
    float range;
    float angleAttack;
    float attackDuration;
    float attackCooldown;
    t_fonctionParam* onEquipe;
    int indice;
    SDL_Texture* texture;
    SDL_Rect displayRect;
} t_arme;

typedef struct s_joueur t_joueur;

void renderWeaponRangeUI(SDL_Renderer* renderer, t_joueur* player);
void renderWeaponDuringAttack(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip);
void renderWeaponIdle(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip);

void addWeaponToPlayer(t_joueur* player, t_arme* weapon);
void switchToNextWeapon(t_joueur* player);

float calculateWeaponScale(t_joueur* player);
void renderAttackConeDebug(t_joueur* player);

#endif