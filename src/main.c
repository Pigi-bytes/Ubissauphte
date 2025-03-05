#include <time.h>

#include "io/input.h"
#include "utils/objectManager.h"
#include "io/audioManager.h"

#define window_width 1280
#define window_height 960

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    t_objectManager *audioManager = initObjectManager(AUDIO_TYPE, freeMusic, INITIAL_CAPACITY);
    t_music *mwusique = (t_music*)malloc(sizeof(t_music));
    ADD_TYPED_OBJECT(audioManager, AUDIO_TYPE, mwusique);


    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        updateInput(&input);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }
    freeObjectManager(&audioManager);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}