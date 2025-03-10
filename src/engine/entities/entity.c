#include "entity.h"

void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera) {
    if (isRectOnCamera(&entity->displayRect, camera)) {
        if (entity->animationController->haveAnimation) {
            t_animation* anim = getCurrentAnimation(entity->animationController);
            updateAnimation(anim);
            entity->texture = anim->frames[anim->currentFrame];
        }
        SDL_RenderCopyEx(renderer, entity->texture, NULL, &entity->displayRect, 0, NULL, entity->flip);
        if (entity->debug == SDL_TRUE) {
            DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, entity->displayRect, 1);
            if (entity->useCircleCollision) {
                DEBUG_DRAW_CIRCLE(renderer, entity->collisionCircle);
            } else {
                DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, entity->collisionRect, 1);
            }
        }
    }
}
