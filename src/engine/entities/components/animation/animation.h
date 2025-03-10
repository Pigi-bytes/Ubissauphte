#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

#include "../../../../utils/objectManager.h"
#include "../../../../utils/timer.h"
#include "../../../tileset.h"
#include "string.h"

typedef struct {
    char* name;            // Nom de l'animation
    SDL_Texture** frames;  // Textures de l'animation
    int numFrames;         // Nombre total de frames
    int currentFrame;      // Frame actuelle
    int animationSpeed;    // Vitesse en ms entre les frames
    t_timer* frameTimer;   // Timer pour le changement de frame
    SDL_bool isLooping;    // Animation en boucle
    SDL_bool isActive;     // État de l'animation
} t_animation;

typedef struct {
    t_objectManager* animations;
    int currentAnim;
    SDL_bool haveAnimation;
} t_animationController;

t_animation* createAnimation(t_tileset* tileset, int* frameIndices, int numFrames, int speed, SDL_bool looping, char* name);
void startAnimation(t_animation* animation);
void stopAnimation(t_animation* animation);
void updateAnimation(t_animation* animation);

t_animationController* initAnimationController();
void addAnimation(t_animationController* controller, t_animation* animation);
void setAnimation(t_animationController* controller, char* name);
t_animation* getCurrentAnimation(t_animationController* controller);
void updateController(t_animationController* controller);

#endif