#include "audioManager.h"

// Free la mweosique
void freeMusic(void *object) {
    t_music *music = (t_music*)object;
    if (music) {
        if (music->music) {
            Mix_FreeMusic(music->music);  // Libérer la mweosique SDL
        }
        if (music->title) {
            free(music->title);  // Libérer la mwémoire du titre
        }
        free(music);
    }
}
