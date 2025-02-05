#include "fonctionManager.h"
t_tabFonct* initFonction() {
    t_tabFonct* fonct = (t_tabFonct*)malloc(sizeof(t_tabFonct));
    fonct->tab_fonct = malloc(sizeof(t_fonctionParam*));
    if (!(fonct->tab_fonct)) {
        printf("erreur lors de l'allocation de fonct->affiche");
        exit(EXIT_FAILURE);
    }
    fonct->nbFonct = 0;
    return fonct;
}

void freeFonctionParam(t_fonctionParam** fonctParam) {
    free((*fonctParam)->param);
    (*fonctParam)->param = NULL;
    free((*fonctParam));
    (*fonctParam) = NULL;
}

void freeFonction(t_tabFonct** fonct) {
    for (int i = 0; i < (*fonct)->nbFonct; i++) {
        freeFonctionParam((&(*fonct)->tab_fonct[i]));
    }
    if ((*fonct)->tab_fonct) {
        free((*fonct)->tab_fonct);
        (*fonct)->tab_fonct = NULL;
    }
    free(*fonct);
    *fonct = NULL;
}

t_fonctionParam* addFonctionSolo(char* name, void (*f)(t_fonctionParam*), ...) {
    va_list list_param;
    va_start(list_param, f);
    t_fonctionParam* fonct = addFonctionParam(name, f, list_param);
    va_end(list_param);
    return fonct;
}

t_fonctionParam* addFonctionParam(char* name, void (*f)(t_fonctionParam*), va_list list_param) {
    void* arg;
    t_fonctionParam* funct = malloc(sizeof(t_fonctionParam));
    funct->param = malloc(sizeof(void*));
    funct->nb_param = 0;
    arg = va_arg(list_param, void*);
    while (arg != NULL) {
        funct->param[funct->nb_param] = arg;
        funct->nb_param++;
        funct->param = realloc(funct->param, (funct->nb_param + 1) * sizeof(void*));
        arg = va_arg(list_param, void*);
    }
    funct->fonction = f;
    strcpy(funct->nom, name);
    return funct;
}

void addFunction(t_tabFonct* fonct, char* name, void (*f)(t_fonctionParam*), ...) {
    va_list list_param;
    void* arg;
    va_start(list_param, f);

    t_fonctionParam* funct = addFonctionParam(name, f, list_param);

    fonct->tab_fonct = realloc(fonct->tab_fonct, (fonct->nbFonct + 1) * sizeof(t_fonctionParam*));
    fonct->tab_fonct[fonct->nbFonct] = funct;
    fonct->nbFonct++;
    va_end(list_param);
}

void callSolo(t_fonctionParam* funct) {
    funct->fonction(funct);
}

void call(t_fonctionManager* contenue, const char* nomFonction) {
    int call = SDL_FALSE;
    for (int i = 0; i < contenue->fonction->nbFonct; i++) {
        if (!(strcmp(contenue->fonction->tab_fonct[i]->nom, nomFonction))) {
            callSolo(contenue->fonction->tab_fonct[i]);
            call = SDL_TRUE;
        }
    }
    if (!(call)) {
        printf("fonction non trouver \n");
    }
}
