#include <time.h>

#include "io/imageLoader.h"
#include "io/input.h"
#include "tileEngine/camera.h"
#include "tileEngine/movement.h"
#include "tileEngine/tilesManager.h"
#include "ui/minimap.h"
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

    t_joueur* player1 = createplayer(&(t_control){SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT}, (SDL_Texture*)getObject(tileset->textureTiles, 97), (SDL_Rect){0, 0, 16, 16});

    int levelWidth = level->width * tileset->tileSize;
    int levelHeight = level->height * tileset->tileSize;

    t_camera* camera = createCamera(levelWidth, levelHeight, levelWidth / 2, levelHeight / 2);
    t_viewPort* viewport = createViewport(renderer, camera, window_width, window_height);
    t_minimap* minimap = createMinimap(renderer, window_width, window_height);

    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        updateInput(&input);
        handleInputPlayer(player1, level, &input);

        if (input.mouseYWheel != 0) {
            cameraHandleZoom(viewport, input.mouseYWheel);
            input.mouseYWheel = 0;
        }

        updateViewport(viewport, input.windowWidth, input.windowHeight);
        updateMinimap(renderer, minimap, camera, 196);

        centerCameraOn(camera, player1->entity.rect.x, player1->entity.rect.y);

        setRenderTarget(renderer, viewport);
        renderGrid(renderer, level);
        SDL_RenderCopy(renderer, player1->entity.texture, NULL, &player1->entity.rect);
        renderViewport(renderer, viewport);

        renderMinimap(renderer, minimap);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    freeTileset(tileset);
    freeGrid(level);
    freePlayer(player1);
    freeCamera(camera);
    freeViewport(viewport);
    freeMinimap(minimap);

    SDL_Quit();
    return EXIT_SUCCESS;
}