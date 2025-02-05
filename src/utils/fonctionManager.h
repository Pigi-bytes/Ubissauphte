#ifndef HEADER_H
#define HEADER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "objectManager.h"

#define MAX_LENGTH 100

#define RENDERALL "randerAll"
#define ADD_FUNCTION(fonct, name, f, ...) addFunction(fonct, name, f, __VA_ARGS__, NULL);
#define ADD_FUNCTION_PARAM(name, f, ...) addFonctionSolo(name, f, __VA_ARGS__, NULL);

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

void addFunction(t_tabFonct* fonct, char* name, void (*f)(t_fonctionParam*), ...);
t_fonctionParam* addFonctionParam(char* name, void (*f)(t_fonctionParam*), va_list list_param);
t_fonctionParam* addFonctionSolo(char* name, void (*f)(t_fonctionParam*), ...);

void call(t_fonctionManager* contenue, const char* nomFonction);
void callSolo(t_fonctionParam* funct);

void freeFonction(t_tabFonct** fonct);
void freeFonctionParam(t_fonctionParam** fonctParam);
#endif