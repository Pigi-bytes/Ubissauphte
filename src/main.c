#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "genmap.h"
#include "io/input.h"
#include "mapview.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

int main() {
    int nbRoom = 15;  // Nombre de salles

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_mapAffichage map;
    SDL_Rect* roomCoords = malloc(sizeof(SDL_Rect) * nbRoom);
    clock_t start = clock();
    t_salle** listRoom = genMap(nbRoom, roomCoords);
    generateMap(roomCoords, listRoom, nbRoom, &map);
    clock_t end = clock();

    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("printMap prend %.2fms\n", duration);

    t_input input;
    initInput(&input, WINDOW_WIDTH, WINDOW_HEIGHT);
    while (!input.quit) {
        updateInput(&input);
        affichage(renderer, &map);
        SDL_RenderPresent(renderer);
    }

    free(roomCoords);
    free(listRoom);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
