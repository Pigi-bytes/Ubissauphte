#include "fonctionManager.h"

void addPamaretre(t_fonctionParam* funct, ...) {
    va_list args;  // structure qui contiendra tout les paramètres
    va_start(args, funct);
    ajoutParametreInFonction(funct, args);
    va_end(args);
}

void ajoutParametreInFonction(t_fonctionParam* funct, va_list list) {
    void* arg;
    while ((arg = va_arg(list, void*)) != NULL) {  // parcourir tout les paramètres jusqu'à atteindre la sentinelle (NULL)
        funct->param[funct->nb_param++] = arg;
        funct->param = realloc(funct->param, (funct->nb_param + 1) * sizeof(void*));
    }
}

t_fonctionParam* creerFonction(void (*f)(t_fonctionParam*), ...) {
    va_list args;  // structure qui contiendra tout les paramètres
    va_start(args, f);

    t_fonctionParam* funct = malloc(sizeof(t_fonctionParam));
    funct->param = malloc(sizeof(void*));
    funct->fonction = f;
    funct->nb_param = 0;

    ajoutParametreInFonction(funct, args);
    va_end(args);

    return funct;
}

void callFonction(t_fonctionParam* funct) {
    funct->fonction(funct);
}

void freeFonction(t_fonctionParam** fonctParam) {
    // utilisation d'un t_fonctionParam** pour sécuriser les free
    free((*fonctParam)->param);
    (*fonctParam)->param = NULL;
    free((*fonctParam));
    (*fonctParam) = NULL;
}