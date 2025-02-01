#include "fonctionManager.h"

int countfonct = 0;

// fonction qui initialise un fonction manager
t_tabFonct* initFonction() {
    t_tabFonct* fonct = (t_tabFonct*)malloc(sizeof(t_tabFonct));
    countfonct++;
    fonct->tab_fonct = malloc(sizeof(t_fonctionParam*));
    countfonct++;
    if (!(fonct->tab_fonct)) {
        printf("erreur lors de l'allocation de fonct->affiche");
        exit(EXIT_FAILURE);
    }
    fonct->nbFonct = 0;

    return fonct;
}

// fonction qui libére un fonction manager
void freeFonction(t_tabFonct** fonct) {
    for (int i = 0; i < (*fonct)->nbFonct; i++) {
        free((*fonct)->tab_fonct[i]->param);
        countfonct--;
        (*fonct)->tab_fonct[i]->param = NULL;
        free((*fonct)->tab_fonct[i]);
        countfonct--;
        (*fonct)->tab_fonct[i] = NULL;
    }
    if ((*fonct)->tab_fonct) {
        free((*fonct)->tab_fonct);
        countfonct--;
        (*fonct)->tab_fonct = NULL;
    }

    free(*fonct);
    countfonct--;
    *fonct = NULL;
}

// fonction qui ajoute une fonction dans le fonction manager
void addFunction(t_tabFonct* fonct, char* name, void (*f)(t_fonctionParam*), ...) {
    va_list list_param;
    void* arg;
    t_fonctionParam* funct = malloc(sizeof(t_fonctionParam));
    countfonct++;
    funct->param = malloc(sizeof(void*));
    countfonct++;
    funct->nb_param = 0;
    va_start(list_param, f);
    arg = va_arg(list_param, void*);
    while (arg != NULL) {
        funct->param[funct->nb_param] = arg;
        funct->nb_param++;
        funct->param = realloc(funct->param, (funct->nb_param + 1) * sizeof(void*));
        arg = va_arg(list_param, void*);
    }
    funct->fonction = f;
    strcpy(funct->nom, name);
    fonct->tab_fonct = realloc(fonct->tab_fonct, (fonct->nbFonct + 1) * sizeof(t_fonctionParam*));
    fonct->tab_fonct[fonct->nbFonct] = funct;
    fonct->nbFonct++;
    va_end(list_param);
}

// fonction qui appelle une fonction de nomFonction présent dans contenu
void call(t_fonctionManager* contenue, const char* nomFonction) {
    int call = SDL_FALSE;
    for (int i = 0; i < contenue->fonction->nbFonct; i++) {
        if (!(strcmp(contenue->fonction->tab_fonct[i]->nom, nomFonction))) {
            contenue->fonction->tab_fonct[i]->fonction(contenue->fonction->tab_fonct[i]);
            call = SDL_TRUE;
        }
    }
    if (!(call)) {
        printf("fonction non trouver \n");
    }
}
