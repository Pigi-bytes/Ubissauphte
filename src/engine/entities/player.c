#include "player.h"

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.rect = rect;

    t_animation* animation = malloc(sizeof(t_animation));
    SDL_Texture** anim = malloc(5 * sizeof(SDL_Texture*));

    for (int i = 1; i < 6; i++) {
        anim[i - 1] = (SDL_Texture*)getObject(tileset->textureTiles, i);
    }

    animation->numFrames = 5;
    animation->currentFrame = 0;
    animation->animationSpeed = 240;
    animation->isLooping = true;
    animation->isActive = false;
    animation->frameTimer = initTimer();
    animation->frames = anim;

    startAnimation(animation);

    joueur->entity.animation = animation;
    //animation = NULL;

    return joueur;
}

void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera) {
    renderEntity(renderer, &player->entity, camera);
}

void freePlayer(void* object) {
    t_joueur* player = (t_joueur*)object;
    SDL_DestroyTexture(player->entity.texture);
    free(player);
}
