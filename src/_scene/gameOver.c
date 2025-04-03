#include "gameOver.h"

t_scene* CreateGameOver(t_context* context) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t LOAD_TYPE = registerType(registre, NULL, "load");

    t_text* text = createText(context->renderer, "GameOver", context->font, WHITE);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    t_scene* scene = createScene(initObjectManager(registre), "attente");
    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "quitter", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.35f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(context->input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, LOAD_TYPE);

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));

    return scene;
}