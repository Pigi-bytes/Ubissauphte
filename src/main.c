#include "debug.h"
#include "engine/core/camera.h"
#include "engine/core/frame.h"
#include "engine/entities/enemy.h"
#include "engine/entities/player.h"
#include "engine/entities/systems/physicsSystem.h"
#include "engine/entities/tiles.h"
#include "engine/world/generationSalle/geneRoom.h"
#include "io/audioManager.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "ui/button.h"
#include "ui/fpsDisplay.h"
#include "ui/minimap.h"
#include "ui/slider.h"
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

typedef struct {
    t_button* button;
    SDL_Scancode* scancode;
    SDL_bool flagCommande;
} t_touche;

typedef struct {
    SDL_bool PleinEcran;
    SDL_bool FlagCommande;
} t_option;

void renderTouche(SDL_Renderer* renderer, t_touche* touche) {
    renderButton(renderer, touche->button);
}

void handleInputTouche(t_input* input, t_touche* touche, SDL_Renderer* renderer) {
    int newWidth = touche->button->rectDefault.w * SCALE_FACTOR;
    int newHeight = touche->button->rectDefault.h * SCALE_FACTOR;
    int deltaX = (newWidth - touche->button->rectDefault.w) / 2;
    int deltaY = (newHeight - touche->button->rectDefault.h) / 2;

    if (isMouseInsideRect(input->mouseX, input->mouseY, &touche->button->rect)) {
        touche->button->rect.w = newWidth;
        touche->button->rect.h = newHeight;
        touche->button->rect.x = touche->button->rectDefault.x - deltaX;
        touche->button->rect.y = touche->button->rectDefault.y - deltaY;

        if (input->mouseButtons[SDL_BUTTON_LEFT]) {
            if (!touche->button->isClicked) {
                touche->button->isClicked = SDL_TRUE;
                touche->button->color = touche->button->colorOnClick;
                if (touche->button->OnClick) {
                    callFonction(touche->button->OnClick);
                }
            }
        } else if (touche->button->isClicked) {
            touche->button->isClicked = SDL_FALSE;
            touche->button->color = touche->button->colorDefault;
        }
    } else {
        touche->button->rect = touche->button->rectDefault;
        if (touche->flagCommande) {
            touche->flagCommande = SDL_FALSE;
            char newtouche[50];
            sprintf(newtouche, "Commande : %s", SDL_GetKeyName(SDL_GetKeyFromScancode(*touche->scancode)));
            printf("coucou");
            updateTextOutline(&(touche->button->label), renderer, newtouche, BLACK, WHITE, 2);
        }
        if (touche->button->isClicked) {
            touche->button->isClicked = SDL_FALSE;
            touche->button->color = touche->button->colorDefault;
        }
    }
    if (touche->flagCommande && indiceToucheCliquer(input) != -1) {
        char newtouche[50];
        sprintf(newtouche, "Commande : %s", SDL_GetKeyName(SDL_GetKeyFromScancode(indiceToucheCliquer(input))));
        updateTextOutline(&(touche->button->label), renderer, newtouche, BLACK, WHITE, 2);
        *touche->scancode = ((SDL_Scancode)indiceToucheCliquer(input));
        printf("%s", SDL_GetKeyName(SDL_GetKeyFromScancode(*touche->scancode)));
        touche->flagCommande = SDL_FALSE;
    }
}

GENERATE_WRAPPER_2(renderText, SDL_Renderer*, t_text*)
GENERATE_WRAPPER_2(renderButton, SDL_Renderer*, t_button*)
GENERATE_WRAPPER_2(renderFPSDisplay, SDL_Renderer*, t_fpsDisplay*)
GENERATE_WRAPPER_3(renderGrid, SDL_Renderer*, t_grid*, t_camera*)
GENERATE_WRAPPER_2(renderMinimap, SDL_Renderer*, t_minimap*)
GENERATE_WRAPPER_2(renderViewport, SDL_Renderer*, t_viewPort*)
GENERATE_WRAPPER_3(renderPlayer, SDL_Renderer*, t_joueur*, t_camera*)
GENERATE_WRAPPER_3(renderEnemy, SDL_Renderer*, t_ennemi*, t_camera*)

GENERATE_WRAPPER_3(updateFPSDisplay, t_fpsDisplay*, t_frameData*, SDL_Renderer*)

GENERATE_WRAPPER_3(updateMinimap, t_minimap*, t_camera*, SDL_Renderer*)
GENERATE_WRAPPER_2(cameraHandleZoom, t_viewPort*, int*)

GENERATE_WRAPPER_2(handleInputButton, t_input*, t_button*)
GENERATE_WRAPPER_4(handleInputPlayer, t_input*, t_joueur*, t_grid*, float*)

GENERATE_WRAPPER_2(setRenderTarget, SDL_Renderer*, t_viewPort*)
GENERATE_WRAPPER_3(centerCameraOn, t_camera*, int*, int*)

GENERATE_WRAPPER_3(resizeViewport, t_viewPort*, int*, int*)
GENERATE_WRAPPER_4(resizeMinimap, SDL_Renderer*, t_minimap*, int*, int*)

GENERATE_WRAPPER_2(renderBarreVolumme, SDL_Renderer*, t_barreVolumme*)
GENERATE_WRAPPER_2(handleInputButtonVolumme, t_input*, t_barreVolumme*)

GENERATE_WRAPPER_2(renderTouche, SDL_Renderer*, t_touche*)
GENERATE_WRAPPER_3(handleInputTouche, t_input*, t_touche*, SDL_Renderer*)

GENERATE_WRAPPER_4(updatePlayer, t_joueur*, float*, t_grid*, t_objectManager*)
GENERATE_WRAPPER_4(updateEnemy, t_ennemi*, float*, t_grid*, t_objectManager*)

t_option* creeOption() {
    t_option* option = malloc(sizeof(t_option));
    option->PleinEcran = SDL_FALSE;
    option->FlagCommande = SDL_FALSE;
    return option;
}

void freeOption(void* elt) {
    free((t_option*)elt);
}

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

void changementAffichage(t_fonctionParam* fonction) {
    SDL_Window* window = GET_PTR(fonction, 0, SDL_Window*);
    t_option* option = GET_PTR(fonction, 1, t_option*);
    t_text** text = GET_PTR(fonction, 2, t_text**);
    SDL_Renderer* renderer = GET_PTR(fonction, 3, SDL_Renderer*);
    int sizeOutline = 2;
    if (option->PleinEcran == SDL_FALSE) {
        option->PleinEcran = SDL_TRUE;
        updateTextOutline(text, renderer, "Fullscreen", BLACK, WHITE, sizeOutline);
    } else {
        option->PleinEcran = SDL_FALSE;
        updateTextOutline(text, renderer, "windowed", BLACK, WHITE, sizeOutline);
    }
    Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
}

t_touche* createTouche(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick, SDL_Scancode* scancode) {
    t_touche* touche = malloc(sizeof(t_touche));
    touche->button = createButton(text, color, colorOnClick, rect, OnClick);
    touche->scancode = scancode;
    touche->flagCommande = SDL_FALSE;
    return touche;
}

void freeTouche(void* elt) {
    t_touche* touche = (t_touche*)elt;
    freeButton(touche->button);
    free(touche);
}

void miseAjourCommande(t_fonctionParam* fonction) {
    t_touche* touche = GET_PTR(fonction, 0, t_touche*);
    SDL_Renderer* renderer = GET_PTR(fonction, 1, SDL_Renderer*);

    int sizeOutline = 2;
    updateTextOutline(&(touche->button->label), renderer, "Commande : _", BLACK, WHITE, sizeOutline);
    touche->flagCommande = SDL_TRUE;
}

t_scene* createCommandeMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, SDL_Window* window, t_option* option, t_control* control) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t COMMANDE_TYPE = registerType(registre, freeTouche, "commande");

    t_scene* scene = createScene(initObjectManager(registre), "scene2");

    int nb = 2;

    char newTouche[50];
    sprintf(newTouche, "Commande : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->up))));
    t_text* textCommande = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande = creerFonction(miseAjourCommande, NULL);
    t_touche* touche = createTouche(textCommande, GREEN, WHITE, creerRect(0.35f, 0.30f, 0.3f, 0.1f), fonctionCommande, &control->up);
    addPamaretre(fonctionCommande, FONCTION_PARAMS(touche, renderer));

    sprintf(newTouche, "Commande : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->down))));
    t_text* textCommande2 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande2 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche2 = createTouche(textCommande2, GREEN, WHITE, creerRect(0.35f, 0.5f, 0.3f, 0.1f), fonctionCommande2, &control->down);
    addPamaretre(fonctionCommande2, FONCTION_PARAMS(touche2, renderer));

    sprintf(newTouche, "Commande : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->right))));
    t_text* textCommande3 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande3 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche3 = createTouche(textCommande3, GREEN, WHITE, creerRect(0.35f, 0.65f, 0.3f, 0.1f), fonctionCommande3, &control->right);
    addPamaretre(fonctionCommande3, FONCTION_PARAMS(touche3, renderer));

    t_text* text = createText(renderer, "Commande", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, touche, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche2, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche3, COMMANDE_TYPE);
    sceneRegisterFunction(scene, COMMANDE_TYPE, HANDLE_INPUT, handleInputToucheWrapper, 1, FONCTION_PARAMS(input, renderer));
    sceneRegisterFunction(scene, COMMANDE_TYPE, RENDER_UI, renderToucheWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}

t_scene* createOptionMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, SDL_Window* window, t_option* option, t_control* control) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t VOLUME_TYPE = registerType(registre, freeBv, "volumme");
    const uint8_t COMMANDE_TYPE = registerType(registre, freeTouche, "commande");

    t_scene* scene = createScene(initObjectManager(registre), "scene2");
    t_fpsDisplay* fpsDisplay = initFPSDisplay(renderer, font);

    int nb = 2;

    t_text* textFps = createTextOutline(renderer, "FPS/ON", font, BLACK, WHITE, nb);
    t_fonctionParam* fp = creerFonction(afficherFps, NULL);
    t_button* fpsButton = createButton(textFps, GREEN, WHITE, creerRect(0.35f, 0.44f, 0.3f, 0.1f), fp);
    addPamaretre(fp, FONCTION_PARAMS(fpsDisplay, &(fpsButton->label), renderer));

    t_text* textEcran = createTextOutline(renderer, "windowed", font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionEcran = creerFonction(changementAffichage, NULL);
    t_button* fpsButtonEcran = createButton(textEcran, GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), fonctionEcran);
    addPamaretre(fonctionEcran, FONCTION_PARAMS(window, option, &(fpsButtonEcran->label), renderer));

    char newTouche[50];
    sprintf(newTouche, "Commande : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->up))));
    t_text* textCommande = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande = creerFonction(miseAjourCommande, NULL);
    t_touche* touche = createTouche(textCommande, GREEN, WHITE, creerRect(0.35f, 0.30f, 0.3f, 0.1f), fonctionCommande, &control->up);
    addPamaretre(fonctionCommande, FONCTION_PARAMS(touche, renderer));

    t_text* text = createText(renderer, "Option", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, touche, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsButton, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, CreerBarreVolume(creerRect(0.35f, (0.58f + 0.05f), 0.3f, 0.01f), creerRect((0.35f + 0.14f), (0.58f + 0.032f), 0.02f, 0.04f), GREEN, WHITE, creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), VOLUME_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsButtonEcran, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Menu Principal", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsDisplay, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));
    sceneRegisterFunction(scene, VOLUME_TYPE, HANDLE_INPUT, handleInputButtonVolummeWrapper, 1, FONCTION_PARAMS(input));
    sceneRegisterFunction(scene, COMMANDE_TYPE, HANDLE_INPUT, handleInputToucheWrapper, 1, FONCTION_PARAMS(input, renderer));

    sceneRegisterFunction(scene, VOLUME_TYPE, RENDER_UI, renderBarreVolummeWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, COMMANDE_TYPE, RENDER_UI, renderToucheWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, VOLUME_TYPE, RENDER_UI, renderBarreVolummeWrapper, 1, FONCTION_PARAMS(renderer));

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
    const uint8_t ENEMY_TYPE = registerType(registre, freeEnemy, "enemy");
    t_scene* scene = createScene(initObjectManager(registre), "scene1");

    t_tileset* tileset = initTileset(renderer, 192, 176, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* playerIdle = initTileset(renderer, 32, 32, 16, "assets/imgs/chevaliervisiereouverteidle12run34.bmp");
    // t_tileset* playerCours = initTileset(renderer, 16, 32, 16, "assets/imgs/cours.bmp");

    // t_tileset* fantom = initTileset(renderer, 48, 16, 16, "assets/imgs/fantomidle23run1232.bmp");

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

    t_objectManager* entities = initObjectManager(createTypeRegistry());
    const uint8_t ENTITY = registerType(entities->registry, NULL, "ENTITY");

    t_joueur* joueur = createPlayer(contr, (SDL_Texture*)getObject(tileset->textureTiles, 98), (SDL_Rect){60, 60, 16, 16}, playerIdle);
    addObject(entities, &joueur->entity, ENTITY);
    placeOnRandomTile(level, &joueur->entity, entities);

    t_ennemi* enemy;
    for (int i = 0; i < 5; i++) {
        enemy = createEnemy((SDL_Texture*)getObject(tileset->textureTiles, 122), (SDL_Rect){100, 100, 16, 16}, playerIdle);
        addObject(entities, &enemy->entity, ENTITY);
        placeOnRandomTile(level, &enemy->entity, entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

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

    sceneRegisterFunction(scene, PLAYER_TYPE, UPDATE, updatePlayerWrapper, 0, FONCTION_PARAMS(&frameData->deltaTime, level, entities));
    sceneRegisterFunction(scene, ENEMY_TYPE, UPDATE, updateEnemyWrapper, 0, FONCTION_PARAMS(&frameData->deltaTime, level, entities));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));
    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, renderer));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.displayRect.x, &joueur->entity.displayRect.y));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&input->windowWidth, &input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(renderer, &input->windowWidth, &input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(renderer, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, 1, FONCTION_PARAMS(renderer, camera));
    sceneRegisterFunction(scene, ENEMY_TYPE, RENDER_GAME, renderEnemyWrapper, 1, FONCTION_PARAMS(renderer, camera));

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
    t_option* option = creeOption();

    t_control* contr = malloc(sizeof(t_control));
    contr->up = SDL_SCANCODE_UP;
    contr->down = SDL_SCANCODE_DOWN;
    contr->left = SDL_SCANCODE_LEFT;
    contr->right = SDL_SCANCODE_RIGHT;

    // t_scene* scene = createOptionMenu(renderer, input, font, frameData, window, option, contr);
    t_scene* scene = createMainWord(renderer, input, font, frameData);
    // t_scene* scene = createCommandeMenu(renderer, input, font, window, option, contr);

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

    freeOption(option);
    freeFrameData(frameData);
    freeScene(scene);
    free(contr);

    TTF_CloseFont(font);
    freeInput(input);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}