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
            Debug_PushRect(&entity->displayRect, 1, COLORDEFAULT);
            if (entity->useCircleCollision) {
                Debug_PushCircle(entity->collisionCircle.x, entity->collisionCircle.y, entity->collisionCircle.radius, COLORDEFAULT);
            } else {
                Debug_PushRect(&entity->collisionRect, 1, COLORDEFAULT);
            }
        }
    }
}
