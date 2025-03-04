#include "debug.h"
#include "engine/core/camera.h"
#include "engine/core/frame.h"
#include "engine/entities/player.h"
#include "engine/gameplay/movementPlayer.h"
#include "engine/world/generationSalle/geneRoom.h"
#include "engine/world/tilesManager.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "ui/button.h"
#include "ui/fpsDisplay.h"
#include "ui/minimap.h"
#include "ui/text.h"
#include "utils/fonctionManager.h"
#include "utils/fscene.h"
#include "utils/objectManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

const SDL_Color WHITE = {255, 255, 255}, BLACK = {0, 0, 0}, MAGENTA = {255, 0, 255}, BLUE = {0, 0, 255}, GREEN = {0, 255, 0};

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
GENERATE_WRAPPER_3(renderPlayer, SDL_Renderer*, t_joueur*, t_camera*)

GENERATE_WRAPPER_3(updateFPSDisplay, t_fpsDisplay*, t_frameData*, SDL_Renderer*)
GENERATE_WRAPPER_3(updateMinimap, t_minimap*, t_camera*, SDL_Renderer*)
GENERATE_WRAPPER_2(cameraHandleZoom, t_viewPort*, int*)

GENERATE_WRAPPER_2(handleInputButton, t_input*, t_button*)
GENERATE_WRAPPER_4(handleInputPlayer, t_input*, t_joueur*, t_grid*, float*)

GENERATE_WRAPPER_2(setRenderTarget, SDL_Renderer*, t_viewPort*)
GENERATE_WRAPPER_3(centerCameraOn, t_camera*, int*, int*)

GENERATE_WRAPPER_3(resizeViewport, t_viewPort*, int*, int*)
GENERATE_WRAPPER_4(resizeMinimap, SDL_Renderer*, t_minimap*, int*, int*)

typedef struct {
    SDL_bool affihcher_fps;
} t_option;

void afficherFps(t_fonctionParam* fonction) {
    t_fpsDisplay* fps = GET_PTR(fonction, 0, t_fpsDisplay*);
    t_text** text = GET_PTR(fonction, 1, t_text**);
    SDL_Renderer* renderer = GET_PTR(fonction, 2, SDL_Renderer*);
    int sizeOutline = 2;
    if (fps->showFPS) {
        fps->showFPS = SDL_FALSE;
        updateTextOutline(text, renderer, "FPS/OFF", BLACK, WHITE, sizeOutline);
    } else {
        fps->showFPS = SDL_TRUE;
        updateTextOutline(text, renderer, "FPS/ON", BLACK, WHITE, sizeOutline);
    }
}

t_scene* createFPSMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "scene2");
    t_text* text = createText(renderer, "Option", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Commande", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.30f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);

    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Volume", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.58f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Affichage", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Menu Principal", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, frameData, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}

t_scene* createOptionMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "scene2");
    t_fpsDisplay* fpsDisplay = initFPSDisplay(renderer, font);

    int nb = 2;

    t_text* textFps = createTextOutline(renderer, "FPS/ON", font, BLACK, WHITE, nb);

    t_button* fpsButton;
    t_fonctionParam* fp = creerFonction(afficherFps, NULL);
    fpsButton = createButton(textFps, GREEN, WHITE, creerRect(0.35f, 0.44f, 0.3f, 0.1f), fp);
    addPamaretre(fp, FONCTION_PARAMS(fpsDisplay, &(fpsButton->label), renderer));

    t_text* text = createText(renderer, "Option", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Commande", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.30f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsButton, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Volume", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.58f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Affichage", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Menu Principal", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsDisplay, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}

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

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));

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
    t_tileset* playerIdle = initTileset(renderer, 16, 80, 16, "assets/imgs/chevalierAnimation.bmp");
    t_tileset* playerCours = initTileset(renderer, 16, 32, 16, "assets/imgs/cours.bmp");

    t_grille* grille = geneRoom();
    t_grid* level = loadMap(grille->nom, tileset);
    freeGrille(grille);

    int levelWidth = level->width * tileset->tileSize;
    int levelHeight = level->height * tileset->tileSize;

    t_control* contr = malloc(sizeof(t_control));
    contr->up = SDL_SCANCODE_UP;
    contr->down = SDL_SCANCODE_DOWN;
    contr->left = SDL_SCANCODE_LEFT;
    contr->right = SDL_SCANCODE_RIGHT;

    t_joueur* joueur = createPlayer(contr, (SDL_Texture*)getObject(tileset->textureTiles, 98), (SDL_Rect){0, 0, 16, 16}, playerIdle, playerCours);
    joueur->entity.rect.x = 60;
    joueur->entity.rect.y = 60;

    t_camera* camera = createCamera(levelWidth, levelHeight, 300, 300);

    t_viewPort* viewport = createViewport(renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    ADD_OBJECT_TO_SCENE(scene, initFPSDisplay(renderer, font), FRAME_DISPLAY_TYPE);
    ADD_OBJECT_TO_SCENE(scene, joueur, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, 1, FONCTION_PARAMS(input, level, &frameData->deltaTime));
    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_INPUT, cameraHandleZoomWrapper, 0, FONCTION_PARAMS(&input->mouseYWheel));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));
    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, renderer));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.rect.x, &joueur->entity.rect.y));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&input->windowWidth, &input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(renderer, &input->windowWidth, &input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(renderer, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, 1, FONCTION_PARAMS(renderer, camera));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}

int main(int argc, char* argv[]) {
    if (initAudioSystem() != 0) {
        return 1;  // Arrêter si l'initialisation échoue
    }

    SDL_Init(SDL_INIT_VIDEO);
    initTextEngine();
    SDL_Window* window = SDL_CreateWindow("animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    t_input* input = initInput(WINDOW_WIDTH, WINDOW_HEIGHT);
    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 24);
    t_frameData* frameData = initFrameData(0);

    t_scene* scene = createOptionMenu(renderer, input, font, frameData);
    // t_scene* scene = createMainWord(renderer, input, font, frameData);

    while (!input->quit) {
        startFrame(frameData);

        updateInput(input);
        executeSceneFunctions(scene, HANDLE_INPUT);

        updateFPS(frameData);
        executeSceneFunctions(scene, UPDATE);

        if (input->resized) {
            executeSceneFunctions(scene, HANDLE_RESIZE);
        }

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