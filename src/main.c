#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "io/input.h"
#include "genmap.h"
#include "mapview.h"

#include <unistd.h>

#define window_width 1280
#define window_height 960
#define EXIT_SUCCESS 0

int main() {
    int niveau = 15;


    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_input input;
    initInput(&input, window_width, window_height);

    int *tabCoordsX = malloc(sizeof(int)*niveau);
    int *tabCoordsY = malloc(sizeof(int)*niveau);

    t_salle **listRoom = genMap(niveau, tabCoordsX, tabCoordsY);

        
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    clock_t start = clock();
    printMap(tabCoordsX, tabCoordsY, listRoom, niveau, renderer);
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("genMapCoords prend %.2fms\n", duration);



    SDL_RenderPresent(renderer);

    while(!input.quit){
        updateInput(&input);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}
