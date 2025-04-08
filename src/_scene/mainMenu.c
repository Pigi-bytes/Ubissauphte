#include "mainMenu.h"

void freeTexture(void* elt) {
    SDL_DestroyTexture((SDL_Texture*)elt);
}

void afficheImage(t_fonctionParam* fp) {
    t_context* context = GET_PTR(fp, 0, t_context*);
    SDL_Texture* texture = GET_PTR(fp, 1, SDL_Texture*);

    SDL_RenderClear(context->renderer);
    SDL_Rect destRect = {0, 0, window_width, window_height};
    Uint8 mod = (Uint8)(255 * 0.8);
    SDL_SetTextureColorMod(texture, mod, mod, mod);
    SDL_RenderCopy(context->renderer, texture, NULL, &destRect);
}

void renderPlayerMain(t_fonctionParam* fp) {
    t_context* context = GET_PTR(fp, 0, t_context*);
    t_joueur* player = GET_PTR(fp, 1, t_joueur*);
    SDL_Rect rect = (SDL_Rect){850, 600, 128, 128};

    SDL_RenderCopyEx(context->renderer, player->entity.texture, NULL, &rect, 0, NULL, player->entity.flip);
}

void incrementPlayerOffset(t_fonctionParam* fp) {
    t_context* context = GET_PTR(fp, 0, t_context*);
    t_joueur* player = GET_PTR(fp, 1, t_joueur*);

    player->indexOffset = (player->indexOffset + 4) % (context->playerSkin->textureTiles->count - (context->playerSkin->textureTiles->count % 4));

    reloadPlayerAnimations(player, context->playerSkin, player->indexOffset);
}

void decrementPlayerOffset(t_fonctionParam* fp) {
    t_context* context = GET_PTR(fp, 0, t_context*);
    t_joueur* player = GET_PTR(fp, 1, t_joueur*);

    player->indexOffset = (player->indexOffset - 4 + (context->playerSkin->textureTiles->count - (context->playerSkin->textureTiles->count % 4))) % (context->playerSkin->textureTiles->count - (context->playerSkin->textureTiles->count % 4));

    reloadPlayerAnimations(player, context->playerSkin, player->indexOffset);
}

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
    const uint8_t FOND_TYPE = registerType(registre, freeTexture, "fond");
    const uint8_t PLAYER_TYPE = registerType(registre, NULL, "player");

    SDL_Texture* texture = loadImage("./assets/imgs/fondEcran.bmp", context->renderer);

    char Newtext[50];
    sprintf(Newtext, "difficulté : %s", indiceFromDifficulty(context->difficulty));
    t_text* textDiff = createTextOutline(context->renderer, Newtext, context->font, BLACK, WHITE, 2);
    t_fonctionParam* fonctionDiff = creerFonction(modifierDifficulty, NULL);
    t_button* difficultyButton = createButton(textDiff, MAGENTA, BLUE, creerRect(0.55f, 0.35f, 0.3f, 0.1f), fonctionDiff);
    addPamaretre(fonctionDiff, FONCTION_PARAMS(context, &(difficultyButton->label)));
    t_scene* scene = createScene(initObjectManager(registre), "menuPrincipal");

    t_text* text = createTextOutline(context->renderer, "Lo Poti Donjon", context->font, BLACK, WHITE, 3);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, texture, FOND_TYPE);
    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Jouer", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.20f, 0.35f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "chargement"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, difficultyButton, BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Option", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.20f, 0.5f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "option"))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Quitter", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.20f, 0.65f, 0.3f, 0.1f), creerFonction(bouttonClickQuit, FONCTION_PARAMS(context->input))), BUTTON_TYPE);

    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "->", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.75f, 0.65f, 0.1f, 0.1f), creerFonction(incrementPlayerOffset, FONCTION_PARAMS(context, *player))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "<-", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.58f, 0.65f, 0.1f, 0.1f), creerFonction(decrementPlayerOffset, FONCTION_PARAMS(context, *player))), BUTTON_TYPE);

    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, PLAYER_TYPE);

    sceneRegisterFunction(scene, FOND_TYPE, RENDER_UI, afficheImage, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_UI, renderPlayerMain, -1, FONCTION_PARAMS(context, *player));

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

t_scene* chargement(t_context* context, t_joueur** player) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");

    t_scene* scene = createScene(initObjectManager(registre), "chargement");

    t_text* text = createTextOutline(context->renderer, "Chargement de la carte ...", context->font, BLACK, WHITE, 3);
    text->rect = creerRect((1 - 0.8f) / 2, 0.1f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "cliquer ici pour génerer la carte", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.225f, 0.4f, 0.5f, 0.1f), creerFonction(CreateNiveauWrapper, FONCTION_PARAMS(context, &context->nbLevel, player))), BUTTON_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Menu Principal", context->font, BLACK, WHITE, 2), MAGENTA, BLUE, creerRect(0.325f, 0.55f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(context->sceneController, "menuPrincipal"))), BUTTON_TYPE);

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));

    return scene;
}