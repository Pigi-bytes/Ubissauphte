
#include "pileRoom.h"

void initPile() {
    Tete = NULL;
}

int PileVide() {
    return Tete == NULL;
}

void empiler(SDL_Point* p) {
    t_element* elt = malloc(sizeof(t_element));
    elt->precedent = Tete;
    elt->point = p;
    Tete = elt;
}

SDL_Point* depiler() {
    if (PileVide()) {
        return NULL;
    }
    t_element* sauv = Tete;
    SDL_Point* p = Tete->point;
    Tete = Tete->precedent;
    free(sauv);
    return p;
}

void viderPile() {
    while ((!PileVide())) {
        depiler();
    }
}
