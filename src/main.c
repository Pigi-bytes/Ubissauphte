#include "debug.h"
#include "fps.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "tileEngine/camera.h"
#include "tileEngine/movement.h"
#include "tileEngine/tilesManager.h"
#include "ui/button.h"
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

// void bouttonClick(t_fonctionParam* fonction) {
//     t_sceneManager* sm = GET_PTR(fonction, 0, t_sceneManager*);
//     switchScene(sm);
// }

void bouttonClickQuit(t_fonctionParam* fonction) {
    t_input* input = GET_PTR(fonction, 0, t_input*);
    input->quit = SDL_TRUE;
}

void renderTextWrapper(t_fonctionParam* f) {
    renderText(GET_PTR(f, 1, SDL_Renderer*), GET_PTR(f, 0, t_text*));
}

void renderButtonWrapper(t_fonctionParam* f) {
    renderButton(GET_PTR(f, 1, SDL_Renderer*), GET_PTR(f, 0, t_button*));
}

void handleButtonClickWrapper(t_fonctionParam* f) {
    handleButtonClick(GET_PTR(f, 0, t_input*), GET_PTR(f, 1, t_button*));
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    initTextEngine();
    SDL_Window* window = SDL_CreateWindow("animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    t_input* input = initInput(WINDOW_WIDTH, WINDOW_HEIGHT);
    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 16);

    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");

    t_scene* scene1 = createScene(initObjectManager(registre), "scene1");

    // Scene du debut
    t_text text = createText(renderer, "Ubissauphte1", font, WHITE);
    text.rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);
    ADD_OBJECT_TO_SCENE(scene1, &text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene1, createButton(createTextOutline(renderer, "Jouer", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.35f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene1, createButton(createTextOutline(renderer, "Option", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.5f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene1, createButton(createTextOutline(renderer, "Quitter", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);

    sceneRegisterFunction(scene1, BUTTON_TYPE, RENDER, renderButtonWrapper, 0, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene1, BUTTON_TYPE, UPDATE, handleButtonClickWrapper, 1, FONCTION_PARAMS(input));
    sceneRegisterFunction(scene1, TEXTE_TYPE, RENDER, renderTextWrapper, 0, FONCTION_PARAMS(renderer));

    while (!input->quit) {
        updateInput(input);
        SDL_RenderClear(renderer);
        executeSceneFunctions(scene1, UPDATE);
        executeSceneFunctions(scene1, RENDER);
        SDL_RenderPresent(renderer);
    }

    freeScene(scene1);
    TTF_CloseFont(font);
    freeInput(input);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}