#include "mainMenu.h"

void modifierDifficulty(t_fonctionParam* fonction) {
    t_context* context = GET_PTR(fonction, 0, t_context*);
    t_text** text = GET_PTR(fonction, 1, t_text**);
    int sizeOutline = 2;
    switch (context->difficulty) {
        case EASY: {
            context->difficulty = NORMAL;
            context->nbLevel = 17;
            updateTextOutline(text, context->renderer, "difficulté : normal", BLACK, WHITE, sizeOutline);
            break;
        }
        case NORMAL: {
            context->difficulty = HARD;
            context->nbLevel = 22;
            updateTextOutline(text, context->renderer, "difficulté : hard", BLACK, WHITE, sizeOutline);
            break;
        }
        case HARD: {
            context->difficulty = DEMONIC;
            context->nbLevel = 32;
            updateTextOutline(text, context->renderer, "difficulté : demonic", BLACK, WHITE, sizeOutline);
            break;
        }
        case DEMONIC: {
            context->difficulty = LEGEND;
            context->nbLevel = 102;
            updateTextOutline(text, context->renderer, "difficulté : legend", BLACK, WHITE, sizeOutline);
            break;
        }
        case LEGEND: {
            context->difficulty = EASY;
            context->nbLevel = 7;
            updateTextOutline(text, context->renderer, "difficulté : easy", BLACK, WHITE, sizeOutline);
            break;
        }
    }
}

t_scene* createMainMenu(t_context* context, t_joueur** player) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    char Newtext[50];
    sprintf(Newtext, "difficulté : %s", indiceFromDifficulty(context->difficulty));
    t_text* textDiff = createTextOutline(context->renderer, Newtext, context->font, BLACK, WHITE, 2);
    t_fonctionParam* fonctionDiff = creerFonction(modifierDifficulty, NULL);
    t_button* difficultyButton = createButton(textDiff, MAGENTA, BLUE, creerRect(0.55f, 0.35f, 0.3f, 0.1f), fonctionDiff);
    addPamaretre(fonctionDiff, FONCTION_PARAMS(context, &(difficultyButton->label)));
    t_scene* scene = createScene(initObjectManager(registre), "menuPrincipal");

    t_text* text = createText(context->renderer, "Lo Poti Donjon", context->font, WHITE);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Jouer", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.20f, 0.35f, 0.3f, 0.1f), creerFonction(CreateNiveauWrapper, FONCTION_PARAMS(context, &context->nbLevel, player))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, difficultyButton, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Option", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.20f, 0.5f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "option"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Quitter", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.20f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(context->input))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));

    return scene;
}

char* indiceFromDifficulty(difficulty d) {
    switch (d) {
        case EASY:
            return "Easy";
        case NORMAL:
            return "Normal";
        case HARD:
            return "Hard";
        case DEMONIC:
            return "Demonic";
        case LEGEND:
            return "Legend";
    }
    return "";
}