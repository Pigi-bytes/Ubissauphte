#include "optionMenu.h"

t_scene* createOptionMenu(t_context* context) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t VOLUME_TYPE = registerType(registre, freeBv, "volumme");
    const uint8_t COMMANDE_TYPE = registerType(registre, freeTouche, "commande");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "option");

    int nb = 2;

    t_text* textEcran = createTextOutline(context->renderer, "windowed", context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionEcran = creerFonction(changementAffichage, NULL);
    t_button* fpsButtonEcran = createButton(textEcran, GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), fonctionEcran);
    addPamaretre(fonctionEcran, FONCTION_PARAMS(context->window, context->option, &(fpsButtonEcran->label), context->renderer, context->input));

    t_text* text = createText(context->renderer, "Option", context->font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Commandes", context->font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.3f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "commande"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Menu Fps", context->font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.44f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "fpsMenu"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, CreerBarreVolume(creerRect(0.35f, (0.58f + 0.05f), 0.3f, 0.01f), creerRect((0.35f + 0.14f), (0.58f + 0.032f), 0.02f, 0.04f), GREEN, WHITE, creerFonction(bouttonClickQuit, FONCTION_PARAMS(context->input))), VOLUME_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsButtonEcran, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Menu Principal", context->font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(getPrevuisMenuWrapper, FONCTION_PARAMS(context->sceneController))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, VOLUME_TYPE, HANDLE_INPUT, handleInputButtonVolummeWrapper, 1, FONCTION_PARAMS(context->input, context));
    sceneRegisterFunction(scene, COMMANDE_TYPE, HANDLE_INPUT, handleInputToucheWrapper, 1, FONCTION_PARAMS(context->input, context->renderer));

    sceneRegisterFunction(scene, VOLUME_TYPE, RENDER_UI, renderBarreVolummeWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, COMMANDE_TYPE, RENDER_UI, renderToucheWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, VOLUME_TYPE, RENDER_UI, renderBarreVolummeWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));

    return scene;
}