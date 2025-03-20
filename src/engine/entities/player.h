#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "../../debug.h"
#include "../../io/input.h"
#include "enemy.h"
#include "entity.h"
#include "systems/physicsSystem.h"
#include "tiles.h"

// Anticipation > contact > recovery
// https://www.youtube.com/watch?v=8X4fx-YncqA

// Shift / FRICTION / TIME slown down scaling // ralentissement en cas d'attaque / 

typedef struct {
    SDL_bool isActive;
    float cooldown;
    float progress;   // Progression de 0.0 à 1.0
    t_sector hitBox;  // Secteur d'attaque (origine, angles, portée)
    int nbHits;

    // Propriétés pour le ralentissement du temps
    float timeSlowFactor;     // Facteur de ralentissement (1.0 = normal, 0.3 = ralenti à 30%)
    float timeSlowDuration;   // Durée totale du ralentissement
    float timeSlowRemaining;  // Temps restant de ralentissement
} t_attack;

typedef struct {
    float mass;            // Masse (influence le knockback)
    float damage;          // Dégâts infligés
    float range;           // Portée effective
    float angleAttack;     // Largeur du cone d'attaque
    float attackDuration;  // Duree de l'animation d'attaque
    float attackCooldown;  // Temps de recharge

    SDL_Texture* texture;  // Sprite optionnel de l'arme
    SDL_Rect displayRect;  // Position d'affichage sur le joueur
} t_arme;

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
    SDL_Scancode dash;
} t_control;

typedef struct {
    t_entity entity;
    t_control* control;

    t_arme* currentWeapon;
    t_attack attack;
    float aimAngle;
} t_joueur;

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset);

void updatePlayer(t_joueur* player, float* deltaTime, t_grid* grid, t_objectManager* entities);
void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime);
void freePlayer(void* object);

SDL_bool is_in_attack_sector(SDL_FPoint target, float target_radius, SDL_FPoint origin, float current_angle, float range, float arc);
void update_attack(t_joueur* player, float* deltaTime, t_objectManager* entities);
void start_attack(t_joueur* player);

#endif