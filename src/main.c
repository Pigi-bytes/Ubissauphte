#include <time.h>

#include "io/imageLoader.h"
#include "io/input.h"
#include "tileEngine/camera.h"
#include "tileEngine/movement.h"
#include "tileEngine/tilesManager.h"
#include "utils/objectManager.h"

#define window_width 1280
#define window_height 960

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    srand(time(NULL));
    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_tileset* tileset = initTileset(renderer, 192, 176, 16, "assets/imgs/tileMapDungeon.bmp");
    t_grid* level = loadMap("assets/map/map01.txt", tileset);

    t_control fleche = {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT};
    t_joueur* player1 = createplayer(&fleche, (SDL_Texture*)getObject(tileset->textureTiles, 97), (SDL_Rect){400, 300, 64, 64});

    t_camera camera = {
        .centerCameraX = 0, .centerCameraY = 0,
        .windowWidth = window_width,    //
        .windowHeight = window_height,  //
        .worldWidth = 1600,             // Exemple
        .worldHeight = 1200,
        .camWidth = 800,  // Zone à afficher dans le monde
        .camHeight = 600,
        .scaleX = window_width / 800,
        .scaleY = window_height / 600};

    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        updateInput(&input);
        handleInputPlayer(player1, level, &input);

        cameraHandleResize(&camera, input.windowWidth, input.windowHeight);
        cameraCenterOnRect(&camera, &player1->entity.rect);

        SDL_RenderClear(renderer);

        renderGrid(renderer, level, &camera, tileset);
        renderEntity(renderer, &camera, &player1->entity);

        if (input.mouseYWheel != 0) {
            cameraHandleZoom(&camera, input.mouseYWheel, input.windowWidth, input.windowHeight);
            input.mouseYWheel = 0;
        }

        renderMinimap(renderer, &camera, input.windowWidth, input.windowHeight);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderPresent(renderer);
    }

    freePlayer(player1);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}