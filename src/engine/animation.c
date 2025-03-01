// animation.c
#include "animation.h"

t_animation* createAnimation(t_tileset* tileset, int* frameIndices, int numFrames, int speed, SDL_bool looping) {
    t_animation* animation = malloc(sizeof(t_animation));

    animation->frames = malloc(numFrames * sizeof(SDL_Texture*));
    animation->numFrames = numFrames;
    animation->currentFrame = 0;
    animation->animationSpeed = speed;
    animation->isLooping = looping;
    animation->isActive = false;
    animation->frameTimer = initTimer();

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
    animation->isActive = true;
    animation->currentFrame = 0;
    startTimer(animation->frameTimer);
}

void stopAnimation(t_animation* animation) {
    animation->isActive = false;
}