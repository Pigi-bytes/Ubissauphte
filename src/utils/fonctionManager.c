#include "fonctionManager.h"

t_fonctionParam* creerFonction(void (*f)(t_fonctionParam*), ...) {
    va_list args;  // structure qui contiendra tout les paramètres
    void* arg;

    t_fonctionParam* funct = malloc(sizeof(t_fonctionParam));
    funct->param = malloc(sizeof(void*));
    funct->fonction = f;
    funct->nb_param = 0;

    va_start(args, f);
    while ((arg = va_arg(args, void*)) != NULL) {  // parcourir tout les paramètres jusqu'à atteindre la sentinelle (NULL)
        funct->param[funct->nb_param++] = arg;
        funct->param = realloc(funct->param, (funct->nb_param + 1) * sizeof(void*));
    }
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