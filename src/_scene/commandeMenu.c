#include "commandeMenu.h"

t_scene* createCommandeMenu(SDL_Renderer* renderer, t_input* input, TTF_Font* font, SDL_Window* window, t_option* option, t_control* control, t_sceneController* sceneController) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t TEXTE_TYPE = registerType(registre, freeText, "text");
    const uint8_t COMMANDE_TYPE = registerType(registre, freeTouche, "commande");
    const uint8_t BUTTON_TYPE = registerType(registre, freeButton, "button");

    t_scene* scene = createScene(initObjectManager(registre), "commande");

    int nb = 2;

    char newTouche[50];
    sprintf(newTouche, "Commande Up : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->up))));
    t_text* textCommande = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande = creerFonction(miseAjourCommande, NULL);
    t_touche* touche = createTouche(textCommande, GREEN, WHITE, creerRect(0.2f, 0.30f, 0.3f, 0.1f), fonctionCommande, &control->up, "Up");
    addPamaretre(fonctionCommande, FONCTION_PARAMS(touche, renderer));

    sprintf(newTouche, "Commande Down : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->down))));
    t_text* textCommande2 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande2 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche2 = createTouche(textCommande2, GREEN, WHITE, creerRect(0.2f, 0.44f, 0.3f, 0.1f), fonctionCommande2, &control->down, "Down");
    addPamaretre(fonctionCommande2, FONCTION_PARAMS(touche2, renderer));

    sprintf(newTouche, "Commande Right : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->right))));
    t_text* textCommande3 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande3 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche3 = createTouche(textCommande3, GREEN, WHITE, creerRect(0.2f, 0.58f, 0.3f, 0.1f), fonctionCommande3, &control->right, "Right");
    addPamaretre(fonctionCommande3, FONCTION_PARAMS(touche3, renderer));

    sprintf(newTouche, "Commande Left : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->left))));
    t_text* textCommande4 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande4 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche4 = createTouche(textCommande4, GREEN, WHITE, creerRect(0.2, 0.72f, 0.3f, 0.1f), fonctionCommande4, &control->left, "Left");
    addPamaretre(fonctionCommande4, FONCTION_PARAMS(touche4, renderer));

    sprintf(newTouche, "Commande Dash : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->dash))));
    t_text* textCommande5 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande5 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche5 = createTouche(textCommande5, GREEN, WHITE, creerRect(0.52f, 0.44f, 0.3f, 0.1f), fonctionCommande5, &control->dash, "Dash");
    addPamaretre(fonctionCommande5, FONCTION_PARAMS(touche5, renderer));

    sprintf(newTouche, "Commande Escape : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->escape))));
    t_text* textCommande6 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande6 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche6 = createTouche(textCommande6, GREEN, WHITE, creerRect(0.52f, 0.58f, 0.3f, 0.1f), fonctionCommande6, &control->escape, "Escape");
    addPamaretre(fonctionCommande6, FONCTION_PARAMS(touche6, renderer));

    t_text* text = createText(renderer, "Commandes", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, touche, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche2, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche3, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche4, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche5, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche6, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Retour", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(sceneController, "option"))), BUTTON_TYPE);
    sceneRegisterFunction(scene, COMMANDE_TYPE, HANDLE_INPUT, handleInputToucheWrapper, 1, FONCTION_PARAMS(input, renderer));
    sceneRegisterFunction(scene, COMMANDE_TYPE, RENDER_UI, renderToucheWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    return scene;
}