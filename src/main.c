#include <time.h>

#include "./move.h"

#define window_width 1280
#define window_height 960
#define NUM_OBSTACLES 5

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    srand(time(NULL));
    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture_background = loadImage("../Ubissauphte/assets/imgs/donjon_sdl.bmp", renderer);

    t_control fleche = {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT};
    t_control j2 = {SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D};
    t_control j3 = {SDL_SCANCODE_I, SDL_SCANCODE_K, SDL_SCANCODE_J, SDL_SCANCODE_L};

    t_joueur* player1 = createplayer(&fleche, loadImage("../Ubissauphte/assets/imgs/personnage_sdl.bmp", renderer), (SDL_Rect){400, 300, 100, 100});
    t_joueur* player2 = createplayer(&j2, loadImage("../Ubissauphte/assets/imgs/squid-game_sdl.bmp", renderer), (SDL_Rect){400, 400, 100, 150});
    t_joueur* player3 = createplayer(&j3, loadImage("../Ubissauphte/assets/imgs/streetfighter_sdl.bmp", renderer), (SDL_Rect){400, 500, 150, 150});

    Obstacle obstacles[NUM_OBSTACLES];
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].rect = (SDL_Rect){rand() % (window_width - 100), rand() % (window_height - 100), 50 + rand() % 50, 50 + rand() % 50};
        obstacles[i].color = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};
    }
    t_input input;
    initInput(&input, window_width, window_height);

    while (!input.quit) {
        updateInput(&input);

        handle_input(player1, obstacles, NUM_OBSTACLES, &input);
        handle_input(player2, obstacles, NUM_OBSTACLES, &input);
        handle_input(player3, obstacles, NUM_OBSTACLES, &input);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture_background, NULL, NULL);
        drawObject(renderer, player1);
        drawObject(renderer, player2);
        drawObject(renderer, player3);

        for (int i = 0; i < NUM_OBSTACLES; i++) {
            render_rect(renderer, &obstacles[i].rect, obstacles[i].color);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture_background);
    freePlayer(player1);
    freePlayer(player2);
    freePlayer(player3);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}