#include "entity.h"

void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera) {
    if (isRectOnCamera(&entity->displayRect, camera)) {
        if (entity->animationController->haveAnimation) {
            t_animation* anim = getCurrentAnimation(entity->animationController);

            if (anim != NULL && anim->isActive) {
                updateAnimation(anim);

                if (anim->currentFrame >= 0 && anim->currentFrame < anim->numFrames) {
                    entity->texture = anim->frames[anim->currentFrame];
                }
            }
        }

        if (entity->texture != NULL) {
            SDL_RenderCopyEx(renderer, entity->texture, NULL, &entity->displayRect, 0, NULL, entity->flip);
        }

        if (entity->debug == SDL_TRUE) {
            Debug_PushRect(&entity->displayRect, 1, SDL_COLOR_BLUE);
            if (entity->useCircleCollision) {
                Debug_PushCircle(entity->collisionCircle.x, entity->collisionCircle.y, entity->collisionCircle.radius, SDL_COLOR_RED);
            } else {
                Debug_PushRect(&entity->collisionRect, 1, SDL_COLOR_RED);
            }
        }
    }
}