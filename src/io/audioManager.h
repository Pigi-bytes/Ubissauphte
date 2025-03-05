#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/objectManager.h"

typedef struct{
    char *title;       // Titre du SFX
    Mix_Chunk *sfx;  // Pointeur vers le SFX chargée
}t_sfx;

typedef struct{
    char *title;       // Titre de la mweosique
    Mix_Music *music;  // Pointeur vers la mweosique chargée
}t_music;  

typedef struct{
    t_objectManager* sons;
}t_audiomanager;
 
t_audiomanager* initAudioManager();
void *chercheSon(char * path, t_audiomanager *audioManager);
int initAudioSystem(void);
void freeMusic(void *object);
void freeSFX(void *object);
t_music* initMusic(char *path);
t_sfx* initSFX(char *path);
void finMusique(const int ms);
void jouerSFX(char * path, int volume, int loops, t_audiomanager *audioManager);
void jouerMusique(char * path, int volume, int loops, t_audiomanager *audioManager);
void addSFX(t_audiomanager *audioManager, t_sfx *sfx);
void addMusique(t_audiomanager *audioManager, t_music *musique);
void freeAudioManager(t_audiomanager *audioManager);

#endif // AUDIO_MANAGER_H
 