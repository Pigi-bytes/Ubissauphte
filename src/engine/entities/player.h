#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "../../context.h"
#include "../../debug.h"
#include "../../io/input.h"
#include "components/health/health.h"
#include "components/physic/physics.h"
#include "enemy.h"
#include "entity.h"
#include "tiles.h"

// Anticipation > contact > recovery
// https://www.youtube.com/watch?v=8X4fx-YncqA

// Particule / hit sound

typedef struct {
    SDL_bool isActive;
    float cooldown;
    float progress;   // Progression de 0.0 à 1.0
    t_sector hitBox;  // Secteur d'attaque (origine, angles, portée)
    int nbHits;
    float hit_distance;  // Distance du dernier hit pour le screen shake

    float timeSlowFactor;
    float timeSlowDuration;
    float timeSlowRemaining;
} t_attack;

typedef struct {
    float mass;
    float damage;
    float range;
    float angleAttack;
    float attackDuration;
    float attackCooldown;

    SDL_Texture* texture;
    SDL_Rect displayRect;
} t_arme;

typedef struct {
    t_entity entity;
    t_control* control;

    t_healthSystem health;

    int level;
    int xp;
    int xpToNextLevel;

    // TEMPORAIRE TEST
    t_arme* weapons[10];
    int weaponCount;
    int currentWeaponIndex;
    t_arme* currentWeapon;
    int indexCurrentRoom;

    t_attack attack;
    float aimAngle;
} t_joueur;

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset);
void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities);
void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime, t_sceneController* sceneController);
void freePlayer(void* object);

void renderWeaponDuringAttack(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip);

void addWeaponToPlayer(t_joueur* player, t_arme* weapon);
void switchToNextWeapon(t_joueur* player);
#endif