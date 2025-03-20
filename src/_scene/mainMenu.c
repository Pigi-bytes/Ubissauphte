#include "mainMenu.h"

t_scene* createMainMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, t_sceneController* sceneController) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "menuPrincipal");

    t_text* text = createText(renderer, "Lo Poti Donjon", font, WHITE);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Jouer", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.35f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(sceneController, "main"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Option", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.5f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(sceneController, "option"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Quitter", font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(input))), BUTTON_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}