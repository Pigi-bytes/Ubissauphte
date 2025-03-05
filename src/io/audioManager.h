#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/objectManager.h"

typedef struct {
    char *title;       // Titre de la mweosique
    Mix_Music *music;  // Pointeur vers la mweosique chargée
} t_music;

void freeMusic(void *object);

#endif // AUDIO_MANAGER_H
