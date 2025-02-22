#include "debug.h"
#include "fps.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "tileEngine/camera.h"
#include "tileEngine/movement.h"
#include "tileEngine/tilesManager.h"
#include "ui/button.h"
#include "ui/fpsDisplay.h"
#include "ui/minimap.h"
#include "ui/text.h"
#include "utils/fonctionManager.h"
#include "utils/fscene.h"
#include "utils/objectManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

const SDL_Color WHITE = {255, 255, 255}, BLACK = {0, 0, 0}, MAGENTA = {255, 0, 255}, BLUE = {0, 0, 255};

SDL_Rect creerRect(float x_ratio, float y_ratio, float w_ratio, float h_ratio) {
    return (SDL_Rect){WINDOW_WIDTH * x_ratio, WINDOW_HEIGHT * y_ratio, WINDOW_WIDTH * w_ratio, WINDOW_HEIGHT * h_ratio};
}

void bouttonClickQuit(t_fonctionParam* fonction) {
    t_input* input = GET_PTR(fonction, 0, t_input*);
    input->quit = SDL_TRUE;
}

GENERATE_WRAPPER_2(renderText, SDL_Renderer*, t_text*)
GENERATE_WRAPPER_2(renderButton, SDL_Renderer*, t_button*)
GENERATE_WRAPPER_2(renderFPSDisplay, SDL_Renderer*, t_fpsDisplay*)
GENERATE_WRAPPER_3(renderGrid, SDL_Renderer*, t_grid*, t_camera*)
GENERATE_WRAPPER_2(renderMinimap, SDL_Renderer*, t_minimap*)
GENERATE_WRAPPER_2(renderViewport, SDL_Renderer*, t_viewPort*)
GENERATE_WRAPPER_2(renderPlayer, SDL_Renderer*, t_joueur*)

GENERATE_WRAPPER_3(updateFPSDisplay, t_fpsDisplay*, t_frameData*, SDL_Renderer*)
GENERATE_WRAPPER_3(updateMinimap, t_minimap*, t_camera*, SDL_Renderer*)

GENERATE_WRAPPER_2(handleInputButton, t_input*, t_button*)
GENERATE_WRAPPER_3(handleInputPlayer, t_input*, t_joueur*, t_grid*)

GENERATE_WRAPPER_2(setRenderTarget, SDL_Renderer*, t_viewPort*)
GENERATE_WRAPPER_3(centerCameraOn, t_camera*, int*, int*)

t_scene* createMainMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "scene1");

    t_text* text = createText(renderer, "Ubissauphte1", font, WHITE);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Jouer", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.35f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Option", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.5f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Quitter", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, initFPSDisplay(renderer, font), FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}

t_scene* createMainWord(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t GRID_TYPE = registerType(registre, freeGrid, "Grid");
    const uint8_t PLAYER_TYPE = registerType(registre, freePlayer, "player");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t CAMERA_TYPE = registerType(registre, freeCamera, "camera");
    const uint8_t VIEWPORT_TYPE = registerType(registre, freeViewport, "viewport");
    const uint8_t MINIMAP_TYPE = registerType(registre, freeMinimap, "minimap");

    t_scene* scene = createScene(initObjectManager(registre), "scene1");

    t_tileset* tileset = initTileset(renderer, 192, 176, 16, "assets/imgs/tileMapDungeon.bmp");
    t_grid* level = loadMap("assets/map/map02.txt", tileset);

    int levelWidth = level->width * tileset->tileSize;
    int levelHeight = level->height * tileset->tileSize;

    t_control* contr = malloc(sizeof(t_control));
    contr->up = SDL_SCANCODE_UP;
    contr->down = SDL_SCANCODE_DOWN;
    contr->left = SDL_SCANCODE_LEFT;
    contr->right = SDL_SCANCODE_RIGHT;
    t_joueur* joueur = createplayer(contr, (SDL_Texture*)getObject(tileset->textureTiles, 97), (SDL_Rect){0, 0, 16, 16});

    t_camera* camera = createCamera(levelWidth, levelHeight, levelWidth / 2, levelHeight / 2);

    t_viewPort* viewport = createViewport(renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    ADD_OBJECT_TO_SCENE(scene, initFPSDisplay(renderer, font), FRAME_DISPLAY_TYPE);
    ADD_OBJECT_TO_SCENE(scene, joueur, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, 1, FONCTION_PARAMS(input, level));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));
    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, renderer));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.rect.x, &joueur->entity.rect.y));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(renderer, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    initTextEngine();
    SDL_Window* window = SDL_CreateWindow("animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    t_input* input = initInput(WINDOW_WIDTH, WINDOW_HEIGHT);
    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 24);
    t_frameData* frameData = initFrameData(0);

    t_scene* scene1 = createMainMenu(renderer, input, font, frameData);
    t_scene* scene2 = createMainWord(renderer, input, font, frameData);
    t_scene* scene = scene2;

    while (!input->quit) {
        startFrame(frameData);

        updateInput(input);
        executeSceneFunctions(scene, HANDLE_INPUT);

        updateFPS(frameData);
        executeSceneFunctions(scene, UPDATE);

        SDL_RenderClear(renderer);
        executeSceneFunctions(scene, SET_BUFFER);
        executeSceneFunctions(scene, RENDER_GAME);
        executeSceneFunctions(scene, RENDER_BUFFER);
        executeSceneFunctions(scene, RENDER_UI);

        SDL_RenderPresent(renderer);

        capFrameRate(frameData);
    }

    freeFrameData(frameData);
    freeScene(scene);
    TTF_CloseFont(font);
    freeInput(input);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}