#include "animation.h"

t_animation* createAnimation(t_tileset* tileset, int* frameIndices, int numFrames, int speed, SDL_bool looping, char* name) {
    t_animation* animation = malloc(sizeof(t_animation));

    animation->frames = malloc(numFrames * sizeof(SDL_Texture*));
    animation->numFrames = numFrames;
    animation->currentFrame = 0;
    animation->animationSpeed = speed;
    animation->isLooping = looping;
    animation->isActive = SDL_FALSE;
    animation->frameTimer = initTimer();
    animation->name = strdup(name);

    for (int i = 0; i < numFrames; i++) {
        animation->frames[i] = getObject(tileset->textureTiles, frameIndices[i]);
    }

    return animation;
}

void updateAnimation(t_animation* animation) {
    if (animation->isActive && getTicks(animation->frameTimer) > animation->animationSpeed) {
        animation->currentFrame++;

        if (animation->currentFrame >= animation->numFrames) {
            if (animation->isLooping) {
                animation->currentFrame = 0;
            } else {
                stopAnimation(animation);
            }
        }
        startTimer(animation->frameTimer);
    }
}

void startAnimation(t_animation* animation) {
    animation->isActive = SDL_TRUE;
    animation->currentFrame = 0;
    startTimer(animation->frameTimer);
}

void stopAnimation(t_animation* animation) {
    animation->isActive = SDL_FALSE;
}

t_animationController* initAnimationController() {
    t_animationController* controller = malloc(sizeof(t_animationController));

    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, NULL, "ANIMATION_TYPE");

    controller->animations = initObjectManager(registre);
    controller->currentAnim = -1;
    controller->haveAnimation = SDL_FALSE;

    return controller;
}

void addAnimation(t_animationController* controller, t_animation* animation) {
    addObject(controller->animations, animation, getTypeIdByName(controller->animations->registry, "ANIMATION_TYPE"));
    controller->haveAnimation = SDL_TRUE;
}

void setAnimation(t_animationController* controller, char* name) {
    if (!controller->haveAnimation) return;

    for (int i = 0; i < controller->animations->count; i++) {
        t_animation* anim = (t_animation*)getObject(controller->animations, i);
        if (strcmp(anim->name, name) == 0) {
            if (i == controller->currentAnim) return;

            for (int j = 0; j < controller->animations->count; j++) {
                t_animation* anim2 = (t_animation*)getObject(controller->animations, j);

                if (j == controller->currentAnim) {
                    stopAnimation(anim2);
                    break;
                }
            }

            startAnimation(anim);
            controller->currentAnim = i;
            return;
        }
    }
}

t_animation* getCurrentAnimation(t_animationController* controller) {
    if (!controller->haveAnimation) return NULL;

    return (t_animation*)getObject(controller->animations, controller->currentAnim);
}

void updateController(t_animationController* controller) {
    updateAnimation(getCurrentAnimation(controller));
}
