#include "commandeMenu.h"

#include "../commun.h"

t_scene* createCommandeMenu(t_context* context) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t COMMANDE_TYPE = registerType(registre, freeTouche, "commande");
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");

    t_scene* scene = createScene(initObjectManager(registre), "commande");

    int nb = 2;

    char newTouche[50];
    sprintf(newTouche, "Commande Up : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->up))));
    t_text* textCommande = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande = creerFonction(miseAjourCommande, NULL);
    t_touche* touche = createTouche(textCommande, GREEN, WHITE, creerRect(0.025f, 0.30f, 0.3f, 0.1f), fonctionCommande, &context->control->up, "Up");
    addPamaretre(fonctionCommande, FONCTION_PARAMS(touche, context->renderer));

    sprintf(newTouche, "Commande Down : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->down))));
    t_text* textCommande2 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande2 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche2 = createTouche(textCommande2, GREEN, WHITE, creerRect(0.025f, 0.44f, 0.3f, 0.1f), fonctionCommande2, &context->control->down, "Down");
    addPamaretre(fonctionCommande2, FONCTION_PARAMS(touche2, context->renderer));

    sprintf(newTouche, "Commande Right : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->right))));
    t_text* textCommande3 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande3 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche3 = createTouche(textCommande3, GREEN, WHITE, creerRect(0.025f, 0.58f, 0.3f, 0.1f), fonctionCommande3, &context->control->right, "Right");
    addPamaretre(fonctionCommande3, FONCTION_PARAMS(touche3, context->renderer));

    sprintf(newTouche, "Commande Left : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->left))));
    t_text* textCommande4 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande4 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche4 = createTouche(textCommande4, GREEN, WHITE, creerRect(0.025, 0.72f, 0.3f, 0.1f), fonctionCommande4, &context->control->left, "Left");
    addPamaretre(fonctionCommande4, FONCTION_PARAMS(touche4, context->renderer));

    sprintf(newTouche, "Commande Dash : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->dash))));
    t_text* textCommande5 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande5 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche5 = createTouche(textCommande5, GREEN, WHITE, creerRect(0.35f, 0.30f, 0.3f, 0.1f), fonctionCommande5, &context->control->dash, "Dash");
    addPamaretre(fonctionCommande5, FONCTION_PARAMS(touche5, context->renderer));

    sprintf(newTouche, "Commande Escape : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->escape))));
    t_text* textCommande6 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande6 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche6 = createTouche(textCommande6, GREEN, WHITE, creerRect(0.35f, 0.44f, 0.3f, 0.1f), fonctionCommande6, &context->control->escape, "Escape");
    addPamaretre(fonctionCommande6, FONCTION_PARAMS(touche6, context->renderer));

    sprintf(newTouche, "Commande Map : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->map))));
    t_text* textCommande7 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande7 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche7 = createTouche(textCommande7, GREEN, WHITE, creerRect(0.35f, 0.58f, 0.3f, 0.1f), fonctionCommande7, &context->control->map, "map");
    addPamaretre(fonctionCommande7, FONCTION_PARAMS(touche7, context->renderer));

    sprintf(newTouche, "Commande Interact : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->interact))));
    t_text* textCommande8 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande8 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche8 = createTouche(textCommande8, GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), fonctionCommande8, &context->control->interact, "Interact");
    addPamaretre(fonctionCommande8, FONCTION_PARAMS(touche8, context->renderer));

    sprintf(newTouche, "Commande Inventaire : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((context->control->inventaire))));
    t_text* textCommande9 = createTextOutline(context->renderer, newTouche, context->font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande9 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche9 = createTouche(textCommande9, GREEN, WHITE, creerRect(0.675f, 0.44f, 0.3f, 0.1f), fonctionCommande9, &context->control->inventaire, "Inventaire");
    addPamaretre(fonctionCommande9, FONCTION_PARAMS(touche9, context->renderer));

    t_text* text = createText(context->renderer, "Commandes", context->font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, touche, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche2, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche3, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche4, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche5, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche6, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche7, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche8, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche9, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(context->renderer, "Retour", context->font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(getPrevuisMenuWrapper, FONCTION_PARAMS(context->sceneController))), BUTTON_TYPE);

    sceneRegisterFunction(scene, COMMANDE_TYPE, HANDLE_INPUT, handleInputToucheWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, COMMANDE_TYPE, RENDER_UI, renderToucheWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(context));

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));

    return scene;
}