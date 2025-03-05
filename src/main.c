#include <time.h>

#include "io/input.h"
#include "utils/objectManager.h"
#include "io/audioManager.h"

#define window_width 1280
#define window_height 960

int main(int argc, char* argv[]) {
    if (initAudioSystem() != 0) {
        return 1;  // Arrêter si l'initialisation échoue
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    printf("Bonjoir\n");
    t_audiomanager *audioManager = initAudioManager();
    /*t_objectManager *audioManager = initObjectManager(AUDIO_TYPE, freeMusic, INITIAL_CAPACITY);
    t_music *mwusique1 = (t_music*)malloc(sizeof(t_music));
    t_music *mwusique2 = (t_music*)malloc(sizeof(t_music));
    mwusique1 = initMusic("meow", "src/io/meow.wav");
    ADD_TYPED_OBJECT(audioManager, AUDIO_TYPE, mwusique1);
    mwusique2 = initMusic("skibidi", "src/io/skibidi.wav");
    ADD_TYPED_OBJECT(audioManager, AUDIO_TYPE, mwusique2);*/
    
    printf("Bonjoir2\n");
    jouerSFX("src/io/start.wav", MIX_MAX_VOLUME, 0, audioManager);     
    
    printf("Bonjoir3\n");
    jouerMusique("src/io/speed.mp3", 30, -1, audioManager);    
    t_input input;
    int i = 0; 
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        if (input.key[SDL_SCANCODE_W]){
            jouerSFX("src/io/skibidi.wav", 7, 0, audioManager);      
            i++;
            printf("%d\n",i);
        }if (input.key[SDL_SCANCODE_S]){
            jouerSFX("src/io/meow.wav", MIX_MAX_VOLUME, 0, audioManager);      
            i++;
            printf("%d\n",i);
        }if (input.key[SDL_SCANCODE_A]){
            jouerSFX("src/io/hollande.wav", MIX_MAX_VOLUME, 0, audioManager);      
            i++;
            printf("%d\n",i);
        }if (input.key[SDL_SCANCODE_Q]){  
            jouerMusique("src/io/speed.mp3", 100, -1, audioManager); // change le volume si la musique est déjà jouée
            i++;
            printf("%d\n",i);
        }if (input.key[SDL_SCANCODE_E]){  
            jouerMusique("src/io/speed.mp3", 30, -1, audioManager); // change le volume si la musique est déjà jouée       
            i++;
            printf("%d\n",i);
        }
        updateInput(&input);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    //freeObjectManager(&audioManager);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS; 
}