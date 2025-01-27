#include "fonctionManager.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

t_fonct* initFonction() {
    t_fonct* fonct = malloc(sizeof(t_fonct));
    fonct->fonction = malloc(sizeof(t_fonct*));
    if (!(fonct->fonction)) {
        printf("erreur lors de l'allocation de fonct->affiche");
        exit(EXIT_FAILURE);
    }
    fonct->nbFonct = 0;

    return fonct;
}

void freeFonction(t_fonct** fonct) {
    for (int i = 0; i < (*fonct)->nbFonct; i++) {
        free((*fonct)->fonction[i]);
        (*fonct)->fonction[i] = NULL;
    }

    free((*fonct)->fonction);
    (*fonct)->fonction = NULL;

    free(*fonct);
    *fonct = NULL;
}

void addFunction(t_fonct* fonct, void (*f)(void* first_param, ...)) {
    fonct->fonction = realloc(fonct->fonction, sizeof(fonct->fonction) + 1);
    fonct->fonction[fonct->nbFonct] = f;
    fonct->nbFonct++;
}