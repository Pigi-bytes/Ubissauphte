#include <time.h>

#include "fps.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "tileEngine/camera.h"
#include "tileEngine/movement.h"
#include "tileEngine/tilesManager.h"
#include "ui/minimap.h"
#include "ui/text.h"
#include "utils/objectManager.h"

#define window_width 1280
#define window_height 960

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));
    initTextEngine();

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 24);

    t_tileset* tileset = initTileset(renderer, 192, 176, 16, "assets/imgs/tileMapDungeon.bmp");
    t_grid* level = loadMap("assets/map/map01.txt", tileset);
    int levelWidth = level->width * tileset->tileSize;
    int levelHeight = level->height * tileset->tileSize;

    t_joueur* player1 = createplayer(&(t_control){SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT}, (SDL_Texture*)getObject(tileset->textureTiles, 97), (SDL_Rect){0, 0, 16, 16});
    t_camera* camera = createCamera(levelWidth, levelHeight, levelWidth / 2, levelHeight / 2);
    t_viewPort* viewport = createViewport(renderer, camera, window_width, window_height);
    t_minimap* minimap = createMinimap(renderer, window_width, window_height);
    t_frameData* frameData = initFrameData(renderer, font, 0);

    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        startFrame(frameData);

        updateInput(&input);
        handleInputPlayer(player1, level, &input);

        if (input.resized) {
            resizeViewport(viewport, input.windowWidth, input.windowHeight);
            resizeMinimap(renderer, minimap, input.windowWidth, input.windowHeight);
        }

        if (input.mouseYWheel != 0) {
            cameraHandleZoom(viewport, input.mouseYWheel);
            input.mouseYWheel = 0;
        }

        updateMinimap(renderer, minimap, camera, 196);
        updateFPS(frameData, renderer);

        centerCameraOn(camera, player1->entity.rect.x, player1->entity.rect.y);

        setRenderTarget(renderer, viewport);
        renderGrid(renderer, level, camera);
        SDL_RenderCopy(renderer, player1->entity.texture, NULL, &player1->entity.rect);
        renderViewport(renderer, viewport);

        renderMinimap(renderer, minimap);

        renderFPS(frameData, renderer);
        SDL_RenderPresent(renderer);
        capFrameRate(frameData);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    freeTileset(tileset);
    freeGrid(level);
    freePlayer(player1);
    freeCamera(camera);
    freeViewport(viewport);
    freeMinimap(minimap);
    freeFrameData(frameData);

    SDL_Quit();
    return EXIT_SUCCESS;
}