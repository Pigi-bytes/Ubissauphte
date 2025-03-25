
#include "touche.h"
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