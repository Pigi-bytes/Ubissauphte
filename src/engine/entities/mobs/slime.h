#include "../../../debug.h"
#include "../enemy.h"

typedef enum {
    SLIME_IDLE,
    SLIME_CHASE_PLAYER,
    SLIME_CHASE_LAST_KNOWN,
    SLIME_PATROL
} e_slime_state;

typedef struct {
    t_enemy base;
    e_slime_state state;

    t_circle detectionRange;
    float baseDetectionRange;
    float jumpCooldownDuration;
    float idleDurationBeforePatrol;
    float jumpForce;
    int patrolMovesBaseValue;

    SDL_bool playerInDetection;
    SDL_bool playerInSight;
    SDL_FPoint lastKnownPlayerPos;

    int patrolMovesLeft;
    t_deltaTimer* idleTimer;
    t_deltaTimer* jumpCooldownTimer;
} t_slime;

void updateSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);
