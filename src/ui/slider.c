#include "slider.h"

t_barreVolumme* CreerBarreVolume(SDL_Rect barre, SDL_Rect curseur, SDL_Color couleur, SDL_Color couleurCurseur, t_fonctionParam* OnClick) {
    t_barreVolumme* bv = malloc(sizeof(t_barreVolumme));
    bv->barre = barre;
    bv->curseur = curseur;
    bv->curseurDefault = curseur;
    bv->volumme = 50;
    bv->colorBarre = couleur;
    bv->colorCurseur = couleurCurseur;
    bv->isClicked = SDL_FALSE;
    bv->OnClick = OnClick;
    return bv;
}

void renderBarreVolumme(SDL_Renderer* renderer, t_barreVolumme* barre) {
    SDL_SetRenderDrawColor(renderer, barre->colorBarre.r, barre->colorBarre.g, barre->colorBarre.b, barre->colorBarre.a);
    SDL_RenderFillRect(renderer, &barre->barre);
    SDL_SetRenderDrawColor(renderer, barre->colorCurseur.r, barre->colorCurseur.g, barre->colorCurseur.b, barre->colorCurseur.a);

    SDL_RenderFillRect(renderer, &barre->curseur);

    Debug_PushRect(&barre->barre, 3);
    Debug_PushRect(&barre->curseur, 3);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void handleInputButtonVolumme(t_input* input, t_barreVolumme* barre) {
    if (barre->isClicked) {
        if (((input->mouseX - (barre->curseur.w / 2)) >= (barre->barre.x)) && (input->mouseX + (barre->curseur.w / 2)) <= (barre->barre.x + barre->barre.w)) {
            barre->curseur.x = input->mouseX - (barre->curseur.w / 2);
            barre->volumme = ((float)(barre->curseur.x - barre->barre.x) * 100.0f) / ((float)(barre->barre.w - barre->curseur.w));
            printf("%f\n", barre->volumme);
        }
    }

    if (isMouseInsideRect(input->mouseX, input->mouseY, &barre->curseur)) {
        if (input->mouseButtons[SDL_BUTTON_LEFT]) {
            if (!barre->isClicked) {
                barre->isClicked = SDL_TRUE;
            }
        } else if (barre->isClicked) {
            barre->isClicked = SDL_FALSE;
        }
    }
}

void freeBv(void* elt) {
    free((t_barreVolumme*)elt);
}