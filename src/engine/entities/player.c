#include "player.h"

t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_tileset* tileset2) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->entity.texture = texture;
    joueur->entity.rect = rect;
    joueur->entity.flip = SDL_FLIP_NONE;
    joueur->entity.animationController = initAnimationController();

    addAnimation(joueur->entity.animationController, createAnimation(tileset, (int[]){1, 2, 3, 4, 5}, 5, 240, true, "idle"));
    addAnimation(joueur->entity.animationController, createAnimation(tileset2, (int[]){1, 2}, 2, 240, true, "walk"));
    setAnimation(joueur->entity.animationController, "idle");

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
