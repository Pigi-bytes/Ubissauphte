#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "objectManager.h"

#ifndef HEADER_H
#define HEADER_H
#define MAX_LENGTH 100

#define HANDLEBUTTONCALL "handleAllButtonInputCall"
#define RENDERBUTTONCALL "renderAllButtonCall"
#define DRAWTALLEXTCALL "drawAllTextCall"

extern int countfonct;

typedef struct fonction {
    void (*fonction)(struct fonction*);
    char nom[MAX_LENGTH];
    void** param;
    int nb_param;
} t_fonctionParam;

typedef struct {
    t_fonctionParam** tab_fonct;
    int nbFonct;
} t_tabFonct;

typedef struct {
    t_objectManager* manage;
    t_tabFonct* fonction;
} t_fonctionManager;

t_tabFonct* initFonction(void);
void freeFonction(t_tabFonct** fonct);
void addFunction(t_tabFonct* fonct, char* name, void (*f)(t_fonctionParam*), ...);
void call(t_fonctionManager* contenue, const char* nomFonction);
#endif