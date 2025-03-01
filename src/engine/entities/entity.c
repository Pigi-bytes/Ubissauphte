#include "entity.h"

void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera) {
    if (isRectOnCamera(&entity->rect, camera)) {
        if (entity->animationController->haveAnimation) {
            t_animation* anim = getCurrentAnimation(entity->animationController);
            updateAnimation(anim);
            entity->texture = anim->frames[anim->currentFrame];
        }
        SDL_RenderCopyEx(renderer, entity->texture, NULL, &entity->rect, 0, NULL, entity->flip);
        DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, entity->rect, 1);
    }
}
