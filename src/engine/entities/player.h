#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "../../context.h"
#include "../../debug.h"
#include "../../io/input.h"
#include "components/equipment/equipementsManager.h"
#include "components/health/health.h"
#include "components/particules/particules.h"
#include "components/physic/physics.h"
#include "entity.h"
#include "items/weapon.h"
#include "tiles.h"

#define DASH_TRAIL_COUNT 50

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
    SDL_bool isActive;       // Indique si le dash est actif
    float duration;          // Durée du dash
    float speedMultiplier;   // Multiplicateur de vitesse pendant le dash
    t_timer* cooldownTimer;  // Timer pour gérer le cooldown
    Uint32 cooldownTime;     // Temps de cooldown en millisecondes
    struct {
        SDL_Point position;      // Position d'une traînée
        SDL_bool active;         // Indique si cette traînée est active
        float opacity;           // Opacité de la traînée (0.0 à 1.0)
    } trails[DASH_TRAIL_COUNT];  // Tableau de traînées fixe

    float trailSpawnInterval;  // Intervalle de temps entre les captures de position
    float timeSinceLastTrail;  // Temps écoulé depuis la dernière capture
} t_dash;

typedef struct s_joueur {
    t_entity entity;
    t_control* control;
    t_dash dash;
    t_healthSystem health;
    t_particleEmitter* particleEmitter;

    t_stats baseStats;
    t_stats calculatedStats;

    t_equipementSlotType equipement[TOTAL_EQUIPMENT_SLOTS];
    t_inventaire* inventaire;

    int level;
    int xp;
    int xpToNextLevel;
    int gold;

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
void updatePlayer(t_joueur* player, float* deltaTime, t_salle* salle, t_objectManager* entities, t_context* context);
void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera);
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime, t_sceneController* sceneController);
void freePlayer(void* object);

void addPlayerXP(t_joueur* player, int xpAmount);
float getPlayerXPProgress(t_joueur* player);
SDL_bool checkAndProcessLevelUp(t_joueur* player);

#endif