#ifndef BOSS_SLIME_H
#define BOSS_SLIME_H

#include "../../../_scene/mainWorld.h"
#include "../../../debug.h"
#include "../../../ui/text.h"
#include "../enemy.h"

typedef enum {
    BOSS_SLIME_IDLE,
    BOSS_SLIME_CHASE_PLAYER,
    BOSS_SLIME_CHASE_LAST_KNOWN,
    BOSS_SLIME_ENRAGED,       // Phase 2 - plus rapide et agressif
    BOSS_SLIME_DESPERATE,     // Phase 3 - attaques désespérées
    BOSS_SLIME_GROUND_POUND,  // Attaque spéciale 1 - saut puis impact
    BOSS_SLIME_CHARGE_ATTACK  // Attaque spéciale 2 - charge rapide
} e_boss_slime_state;

typedef struct {
    t_enemy base;
    e_boss_slime_state state;

    t_circle detectionRange;
    float baseDetectionRange;

    float jumpCooldownDuration;
    float idleDurationBeforeMove;
    float jumpForce;
    int movesBaseValue;

    float groundPoundForce;
    float chargeForce;
    float specialAttackCooldown;

    SDL_bool playerInDetection;
    SDL_bool playerInSight;
    SDL_FPoint lastKnownPlayerPos;

    int movesLeft;
    int currentPhase;
    float phase2HealthThreshold;
    float phase3HealthThreshold;
    float invulnerabilityPhaseDuration;
    SDL_bool isPhaseTransition;

    t_deltaTimer* idleTimer;
    t_deltaTimer* jumpCooldownTimer;
    t_deltaTimer* specialAttackTimer;
    t_deltaTimer* phaseTransitionTimer;

    t_particleEmitter* particles;
    SDL_Color particleColor;
} t_boss_slime;

typedef struct {
    SDL_bool isActive;
    TTF_Font* font;
    t_enemy* boss;
    char* bossName;
    float displayedHealthRatio;
    float targetHealthRatio;
    SDL_Color phaseColors[3];
    int currentPhase;
    t_deltaTimer* phaseTransitionTimer;
    float lastDamageTime;
    t_text* nameText;
} t_bossHealthBar;

void updateBossSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
t_enemy* createBossSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);

t_bossHealthBar* createBossHealthBar(TTF_Font* font, t_enemy* boss);
void updateBossHealthBar(t_bossHealthBar* bar, float deltaTime);
void renderBossHealthBar(SDL_Renderer* renderer, t_bossHealthBar* bar, int windowWidth, int windowHeight);
void freeBossHealthBar(void* obj);
void updateBossHealthBarWrapper(t_fonctionParam* f);
void renderBossHealthBarWrapper(t_fonctionParam* f);

#endif