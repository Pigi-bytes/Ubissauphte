#include "fpsMenu.h"

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

void modifierFps(t_fonctionParam* fonction) {
    t_frameData* fps = GET_PTR(fonction, 0, t_frameData*);
    t_text** text = GET_PTR(fonction, 1, t_text**);
    SDL_Renderer* renderer = GET_PTR(fonction, 2, SDL_Renderer*);
    int sizeOutline = 2;
    switch (fps->targetFPS) {
        case 30: {
            fps->targetFPS = 60;
            updateTextOutline(text, renderer, "FPS : 60", BLACK, WHITE, sizeOutline);
            break;
        }
        case 60: {
            fps->targetFPS = 120;
            updateTextOutline(text, renderer, "FPS : 120", BLACK, WHITE, sizeOutline);
            break;
        }
        case 120: {
            fps->targetFPS = 0;
            updateTextOutline(text, renderer, "FPS : MAX", BLACK, WHITE, sizeOutline);
            break;
        }
        case 0: {
            fps->targetFPS = 30;
            updateTextOutline(text, renderer, "FPS : 30", BLACK, WHITE, sizeOutline);
            break;
        }
    }
}

t_scene* createFpsMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, t_fpsDisplay* fpsDisplay, SDL_Window* window, t_option* option, t_sceneController* sceneController) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "fpsMenu");

    int nb = 2;

    t_text* textFps = createTextOutline(renderer, "FPS/ON", font, BLACK, WHITE, nb);
    t_fonctionParam* fp = creerFonction(afficherFps, NULL);
    t_button* fpsButton = createButton(textFps, GREEN, WHITE, creerRect(0.35f, 0.44f, 0.3f, 0.1f), fp);
    addPamaretre(fp, FONCTION_PARAMS(fpsDisplay, &(fpsButton->label), renderer));

    char Newtext[50];
    sprintf(Newtext, "FPS : %d", frameData->targetFPS);
    t_text* textFpsMax = createTextOutline(renderer, Newtext, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctiontextFpsMax = creerFonction(modifierFps, NULL);
    t_button* fpsMaxButton = createButton(textFpsMax, GREEN, WHITE, creerRect(0.35f, 0.58f, 0.3f, 0.1f), fonctiontextFpsMax);
    addPamaretre(fonctiontextFpsMax, FONCTION_PARAMS(frameData, &(fpsMaxButton->label), renderer));

    t_text* text = createText(renderer, "Frame Menu", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, fpsMaxButton, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, fpsButton, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Retour", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(sceneController, "option"))), BUTTON_TYPE);

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, 0, FONCTION_PARAMS(frameData, renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}