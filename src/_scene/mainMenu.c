#include "mainMenu.h"

t_scene* createMainMenu(t_context* context) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "menuPrincipal");

    t_text* text = createText(context->renderer, "Lo Poti Donjon", context->font, WHITE);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Jouer", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.35f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "main"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Option", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.5f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "option"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Quitter", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(context->input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));

    return scene;
}