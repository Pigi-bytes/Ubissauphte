#include "win.h"

t_scene* CreateWin(t_context* context) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t LOAD_TYPE = registerType(registre, NULL, "load");

    char* phrases[] = {
        "!aaa",
        "aaaa",
        "a",
        "aa"};
    int nombrePhrases = sizeof(phrases) / sizeof(phrases[0]);

    srand((unsigned int)time(NULL));
    int indexAleatoire = rand() % nombrePhrases;

    t_text* text = createText(context->renderer, "You win", context->font, GREEN);
    text->rect = creerRect(0.1f, 0.25f, 0.8f, 0.2f);

    t_text* text2 = createText(context->renderer, phrases[indexAleatoire], context->font, GREEN);

    text2->rect = creerRect(0.3f, 0.25f, 0.075f, 0.075f);

    int centerX = 600;  // abscisse du point cible
    int centerY = 500;  // ordonnée du point cible

    // Par exemple, pour un objet t_text nommé "texte":
    text2->rect.x = centerX - text2->rect.w / 2;
    text2->rect.y = centerY - text2->rect.h / 2;

    t_scene* scene = createScene(initObjectManager(registre), "win");
    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, text2, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "quitter", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.35f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(context->input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, LOAD_TYPE);

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));

    return scene;
}