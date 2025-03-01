#include "entity.h"

void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera) {
    if (isRectOnCamera(&entity->rect, camera)) {
        if (entity->animation) {
            updateAnimation(entity->animation);
            entity->texture = entity->animation->frames[entity->animation->currentFrame];
        }
        SDL_RenderCopyEx(renderer, entity->texture, NULL, &entity->rect, 0, NULL, entity->flip);
        DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, entity->rect, 1);
    }
}
