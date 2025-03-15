#include "../../debug.h"
#include "enemy.h"

typedef struct {
    t_enemy base;
    t_circle detectionRange;
    float jumpCooldown;
    SDL_bool isJumping;
    float slimeJumpPower;
    float jumpCooldownValue;
} t_slime;

void updateSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);
t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset);

#define SLIME_JUMP_ACCELERATION 300.0f
#define JUMP_COOLDOWN 1.0f