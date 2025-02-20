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
    t_button* btn = GET_PTR(f, 0, t_button*);
    printf("Rendering button: %s\n", btn->label.text);  // Vérifie le texte du bouton
    renderButton(GET_PTR(f, 1, SDL_Renderer*), btn);
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

    t_objectManager* manager1 = initObjectManager(registre);
    t_scene* scene1 = createScene("scene1", manager1);

    // Scene du debut
    t_text text = createText(renderer, "Ubissauphte1", font, WHITE);
    text.rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);
    addObject(manager1, &text, TEXTE_TYPE);

    sceneRegisterFunction(scene1, TEXTE_TYPE, RENDER, renderTextWrapper, 0, renderer, NULL);

    float btnW = 0.3f, btnH = 0.1f, btnX = (1 - btnW) / 2, spacing = 0.05f;
    float btnY1 = 0.1f + 0.2f + spacing;
    float btnY2 = btnY1 + btnH + spacing;
    float btnY3 = btnY2 + btnH + spacing;

    t_button *btn, *btn2, *btn3;
    btn = createButton(createTextOutline(renderer, "Jouer", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(btnX, btnY1, btnW, btnH), CREER_FONCTION(bouttonClickQuit, input));
    addObject(manager1, btn, BUTTON_TYPE);
    btn2 = createButton(createTextOutline(renderer, "Option", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(btnX, btnY2, btnW, btnH), CREER_FONCTION(bouttonClickQuit, input));
    addObject(manager1, btn2, BUTTON_TYPE);
    btn3 = createButton(createTextOutline(renderer, "Quitter", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(btnX, btnY3, btnW, btnH), CREER_FONCTION(bouttonClickQuit, input));
    addObject(manager1, btn3, BUTTON_TYPE);

    sceneRegisterFunction(scene1, BUTTON_TYPE, RENDER, renderButtonWrapper, 0, renderer, NULL);
    // sceneRegisterFunction(scene1, BUTTON_TYPE, UPDATE, handleButtonClickWrapper, 0, input, NULL);

    while (!input->quit) {
        updateInput(input);
        SDL_RenderClear(renderer);

        callFonction(scene1->fonctions[RENDER][0]);
        callFonction(scene1->fonctions[RENDER][1]);
        callFonction(scene1->fonctions[RENDER][2]);
        callFonction(scene1->fonctions[RENDER][3]);

        SDL_RenderPresent(renderer);
    }
    return EXIT_SUCCESS;
}