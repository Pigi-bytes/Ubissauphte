#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

#include "../utils/objectManager.h"
#include "../utils/timer.h"
#include "world/tilesManager.h"

typedef struct {
    SDL_Texture** frames;  // Textures de l'animation
    int numFrames;         // Nombre total de frames
    int currentFrame;      // Frame actuelle
    int animationSpeed;    // Vitesse en ms entre les frames
    t_timer* frameTimer;   // Timer pour le changement de frame
    SDL_bool isLooping;    // Animation en boucle
    SDL_bool isActive;     // État de l'animation
} t_animation;

t_animation* createAnimation(t_tileset* tileset, int* frameIndices, int numFrames, int speed, SDL_bool looping);
void startAnimation(t_animation* animation);
void stopAnimation(t_animation* animation);
void updateAnimation(t_animation* animation);

#endif