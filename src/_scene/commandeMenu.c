#include "commandeMenu.h"

void renderTouche(SDL_Renderer* renderer, t_touche* touche) {
    renderButton(renderer, touche->button);
}

void handleInputTouche(t_input* input, t_touche* touche, SDL_Renderer* renderer) {
    int newWidth = touche->button->rectDefault.w * SCALE_FACTOR;
    int newHeight = touche->button->rectDefault.h * SCALE_FACTOR;
    int deltaX = (newWidth - touche->button->rectDefault.w) / 2;
    int deltaY = (newHeight - touche->button->rectDefault.h) / 2;

    if (isMouseInsideRect(input->mouseX, input->mouseY, &touche->button->rect)) {
        touche->button->rect.w = newWidth;
        touche->button->rect.h = newHeight;
        touche->button->rect.x = touche->button->rectDefault.x - deltaX;
        touche->button->rect.y = touche->button->rectDefault.y - deltaY;

        if (input->mouseButtons[SDL_BUTTON_LEFT]) {
            if (!touche->button->isClicked) {
                touche->button->isClicked = SDL_TRUE;
                touche->button->color = touche->button->colorOnClick;
            }
        } else if (touche->button->isClicked) {
            touche->button->isClicked = SDL_FALSE;
            touche->button->color = touche->button->colorDefault;
            if (touche->button->OnClick) {
                callFonction(touche->button->OnClick);
            }
        }
    } else {
        touche->button->rect = touche->button->rectDefault;
        if (touche->flagCommande) {
            touche->flagCommande = SDL_FALSE;
            char newtouche[50];
            sprintf(newtouche, "Commande %s : %s", touche->nom, SDL_GetKeyName(SDL_GetKeyFromScancode(*touche->scancode)));
            updateTextOutline(&(touche->button->label), renderer, newtouche, BLACK, WHITE, 2);
        }
        if (touche->button->isClicked) {
            touche->button->isClicked = SDL_FALSE;
            touche->button->color = touche->button->colorDefault;
        }
    }
    if (touche->flagCommande && indiceToucheCliquer(input) != -1) {
        char newtouche[50];
        sprintf(newtouche, "Commande %s : %s", touche->nom, SDL_GetKeyName(SDL_GetKeyFromScancode(indiceToucheCliquer(input))));
        updateTextOutline(&(touche->button->label), renderer, newtouche, BLACK, WHITE, 2);
        *touche->scancode = ((SDL_Scancode)indiceToucheCliquer(input));
        touche->flagCommande = SDL_FALSE;
    }
}

t_touche* createTouche(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick, SDL_Scancode* scancode, char* nom) {
    t_touche* touche = malloc(sizeof(t_touche));
    touche->button = createButton(text, color, colorOnClick, rect, OnClick);
    touche->scancode = scancode;
    touche->flagCommande = SDL_FALSE;
    strcpy(touche->nom, nom);
    return touche;
}

void miseAjourCommande(t_fonctionParam* fonction) {
    t_touche* touche = GET_PTR(fonction, 0, t_touche*);
    SDL_Renderer* renderer = GET_PTR(fonction, 1, SDL_Renderer*);

    int sizeOutline = 2;
    char commande[50];
    sprintf(commande, "Commande %s : _", touche->nom);
    updateTextOutline(&(touche->button->label), renderer, commande, BLACK, WHITE, sizeOutline);
    touche->flagCommande = SDL_TRUE;
}

void freeTouche(void* elt) {
    t_touche* touche = (t_touche*)elt;
    freeButton(touche->button);
    free(touche);
}

GENERATE_WRAPPER_2(renderTouche, SDL_Renderer*, t_touche*)
GENERATE_WRAPPER_3(handleInputTouche, t_input*, t_touche*, SDL_Renderer*)

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
    t_touche* touche = createTouche(textCommande, GREEN, WHITE, creerRect(0.35f, 0.30f, 0.3f, 0.1f), fonctionCommande, &control->up, "Up");
    addPamaretre(fonctionCommande, FONCTION_PARAMS(touche, renderer));

    sprintf(newTouche, "Commande Down : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->down))));
    t_text* textCommande2 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande2 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche2 = createTouche(textCommande2, GREEN, WHITE, creerRect(0.35f, 0.44f, 0.3f, 0.1f), fonctionCommande2, &control->down, "Down");
    addPamaretre(fonctionCommande2, FONCTION_PARAMS(touche2, renderer));

    sprintf(newTouche, "Commande Right : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->right))));
    t_text* textCommande3 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande3 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche3 = createTouche(textCommande3, GREEN, WHITE, creerRect(0.35f, 0.58f, 0.3f, 0.1f), fonctionCommande3, &control->right, "Right");
    addPamaretre(fonctionCommande3, FONCTION_PARAMS(touche3, renderer));

    sprintf(newTouche, "Commande Left : %s", SDL_GetKeyName(SDL_GetKeyFromScancode((control->left))));
    t_text* textCommande4 = createTextOutline(renderer, newTouche, font, BLACK, WHITE, nb);
    t_fonctionParam* fonctionCommande4 = creerFonction(miseAjourCommande, NULL);
    t_touche* touche4 = createTouche(textCommande4, GREEN, WHITE, creerRect(0.35f, 0.72f, 0.3f, 0.1f), fonctionCommande4, &control->left, "Left");
    addPamaretre(fonctionCommande4, FONCTION_PARAMS(touche4, renderer));

    t_text* text = createText(renderer, "Commande", font, GREEN);
    text->rect = creerRect((1 - 0.8f) / 2, 0.05f, 0.8f, 0.2f);

    ADD_OBJECT_TO_SCENE(scene, text, TEXTE_TYPE);

    ADD_OBJECT_TO_SCENE(scene, touche, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche2, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche3, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, touche4, COMMANDE_TYPE);
    ADD_OBJECT_TO_SCENE(scene, createButton(createTextOutline(renderer, "Retour", font, BLACK, WHITE, 2), GREEN, WHITE, creerRect(0.35f, 0.86f, 0.3f, 0.1f), creerFonction(setSceneWrapper, FONCTION_PARAMS(sceneController, "option"))), BUTTON_TYPE);
    sceneRegisterFunction(scene, COMMANDE_TYPE, HANDLE_INPUT, handleInputToucheWrapper, 1, FONCTION_PARAMS(input, renderer));
    sceneRegisterFunction(scene, COMMANDE_TYPE, RENDER_UI, renderToucheWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, TEXTE_TYPE, RENDER_UI, renderTextWrapper, 1, FONCTION_PARAMS(renderer));
    sceneRegisterFunction(scene, BUTTON_TYPE, RENDER_UI, renderButtonWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, BUTTON_TYPE, HANDLE_INPUT, handleInputButtonWrapper, 1, FONCTION_PARAMS(input));

    return scene;
}