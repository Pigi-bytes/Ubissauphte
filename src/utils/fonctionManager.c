#include "fonctionManager.h"

t_fonctionParam* creerFonction(char* name, void (*f)(t_fonctionParam*), ...) {
    va_list args;
    void* arg;
    t_fonctionParam* funct = malloc(sizeof(t_fonctionParam));
    funct->param = malloc(sizeof(void*));
    funct->fonction = f;
    funct->nb_param = 0;
    strncpy(funct->nom, name, MAX_LENGTH - 1);
    va_start(args, f);

    while ((arg = va_arg(args, void*)) != NULL) {
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
    free((*fonctParam)->param);
    (*fonctParam)->param = NULL;
    free((*fonctParam));
    (*fonctParam) = NULL;
}