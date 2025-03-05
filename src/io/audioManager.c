// Crédits Loup Picault (mrc le goat, il m'a coaché)

#include "audioManager.h"
 
static Mix_Music * current_music = NULL;


t_audiomanager* initAudioManager(){
    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, freeSFX, "SFX_TYPE");
    registerType(registre, freeMusic, "MUSIQUE_TYPE");

    t_objectManager *objManager = initObjectManager(registre);
    
    t_audiomanager *audioManager = malloc(sizeof(t_audiomanager));
    audioManager->sons = objManager; // Assignation manquante
    return audioManager;
}

void *chercheSon(char * path, t_audiomanager *audioManager){
    printf("Le comptage : %d\n", audioManager->sons->count);
    for (int i = 1; i < audioManager->sons->count+1; i++){
        printf("TEST MUSIQUE; %d, %d\n", getObjectTypeId(audioManager->sons, i), getTypeIdByName(audioManager->sons->registry, "MUSIQUE_TYPE"));
        if (getObjectTypeId(audioManager->sons, i) == getTypeIdByName(audioManager->sons->registry, "MUSIQUE_TYPE")){
            if (strcmp(path, ((t_music*)(audioManager->sons->registry))->title) == 0){
                    printf("Musique %s trouvée à l'index %d\n", path, i);
                    return ((t_music*)(audioManager->sons->registry))->music;
            }
        }
        printf("TEST SFX; %d, %d\n", getObjectTypeId(audioManager->sons, i), getTypeIdByName(audioManager->sons->registry, "SFX_TYPE"));
        if (getObjectTypeId(audioManager->sons, i) == getTypeIdByName(audioManager->sons->registry, "SFX_TYPE")){
            if (strcmp(path, ((t_sfx*)(audioManager->sons->registry))->title) == 0){
                printf("SFX %s trouvée à l'index %d\n", path, i);
                return ((t_sfx*)(audioManager->sons->registry))->sfx;
            }
        }
    }
    printf("Musique %s pas trouvé\n", path);
    return NULL;
}


// J'ai pas tout compris, mais en gros ça initialise les paramètres audio pour que SDL fonctionne bien
int initAudioSystem(void){
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return -1; 
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    Mix_AllocateChannels(1024);

    return 0; 
}

// Free la mweosique
void freeMusic(void *object){
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

// Free la SFX
void freeSFX(void *object){
    t_sfx *sfx = (t_sfx*)object;
    if (sfx) {
        if (sfx->sfx) {
            Mix_FreeChunk(sfx->sfx);  // Libérer le SFX SDL
        }
        if (sfx->title) {
            free(sfx->title);  // Libérer la SFX du titre
        }
        free(sfx);
    }
}

// Fonction pour initialiser une mweosique
t_music* initMusic(char *path){
    t_music *music = (t_music*)malloc(sizeof(t_music));
    if (!music) {
        printf("Erreur d'alloc mémoire pour la musique\n");
        return NULL;
    }

    music->title = (char*)malloc(strlen(path) + 1);  // +1 pour le '\0' de fin de chaîne
    if (!music->title) {
        printf("Erreur d'alloc mémoire pour le titre\n");
        free(music);  // Libérer la mwémoire allouée pour music
        return NULL;
    }
    strcpy(music->title, path); // Met le titre dans la mwémoire qui est allouée


    music->music = Mix_LoadMUS(path); // Charger la mweosique
    if (!music->music) {
        printf("Erreur lors du chargement de la musique (%s) : %s\n", path, Mix_GetError());
        free(music->title);
        free(music);
        return NULL;
    }

    return music;
}


// Fonction pour initialiser une mweosique
t_sfx* initSFX(char *path){
    t_sfx *sfx = (t_sfx*)malloc(sizeof(t_sfx));
    if (!sfx) {
        printf("Erreur d'alloc mémoire pour le SFX\n");
        return NULL;
    }

    sfx->title = (char*)malloc(strlen(path) + 1);  // +1 pour le '\0' de fin de chaîne
    if (!sfx->title) {
        printf("Erreur d'alloc mémoire pour le titre SFX\n");
        free(sfx);  // Libérer la mwémoire allouée pour music
        return NULL;
    }
    strcpy(sfx->title, path); // Met le titre dans la mwémoire qui est allouée


    sfx->sfx = Mix_LoadWAV(path); // Charger la mweosique
    if (!sfx->sfx) {
        printf("Erreur lors du chargement du SFX (%s) : %s\n", path, Mix_GetError());
        free(sfx->title);
        free(sfx);
        return NULL;
    }

    return sfx;
}

void finMusique(const int ms){
    Mix_FadeOutMusic(ms);
    current_music = NULL;
}

void jouerSFX(char *path, int volume, int loops, t_audiomanager *audioManager){
    t_sfx *sfx = initSFX(path);

    sfx->sfx = (Mix_Chunk*)chercheSon(path, audioManager);
    strcpy(sfx->title, path);

    if (!sfx->sfx) {
        sfx->sfx = Mix_LoadWAV(path);
        addSFX(audioManager, sfx);
    }
    
    if (!sfx->sfx) {
        printf("Failed to load SFX audio file %s\n", path);
        return; 
    }
    printf("Nb de sons enregistrées : %d\n", audioManager->sons->count);
    Mix_VolumeChunk(sfx->sfx, volume);
    Mix_PlayChannel(-1, sfx->sfx, loops);
    
    printf("Bajour4\n");
}

void jouerMusique(char * path, int volume, int loops, t_audiomanager *audioManager){
    t_music *music = initMusic(path);
    music->music = (Mix_Music*)chercheSon(path, audioManager);
    strcpy(music->title, path);

    if (!music->music) {
        music->music = Mix_LoadMUS(path);
        addMusique(audioManager, music);
    }
    if (!music->music) {
        printf("Failed to load music audio file %s\n", path);
        return;
    }
    printf("Nb de sons enregistrées : %d\n", audioManager->sons->count);
    
    Mix_VolumeMusic(volume);
    if (current_music == music->music) return;
    current_music = music->music;
 
    Mix_PlayMusic(current_music, loops);
}

void addSFX(t_audiomanager *audioManager, t_sfx *sfx) {
    addObject(audioManager->sons, sfx, getTypeIdByName(audioManager->sons->registry, "SFX_TYPE"));
}

void addMusique(t_audiomanager *audioManager, t_music *musique) {
    addObject(audioManager->sons, musique, getTypeIdByName(audioManager->sons->registry, "MUSIQUE_TYPE"));
}
 



