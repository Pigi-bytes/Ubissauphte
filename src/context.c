#include "context.h"

t_option* creeOption() {
    t_option* option = malloc(sizeof(t_option));
    option->PleinEcran = SDL_FALSE;
    option->FlagCommande = SDL_FALSE;

    return option;
}

void freeOption(void* elt) {
    free((t_option*)elt);
}