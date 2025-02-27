#ifndef LISTB_H
#define LISTB_H

#include <stdio.h>
#include <stdlib.h>

#include "general.h"

#define NBTYPE 6

typedef enum {
    MUR_TYPE,
    PLAFOND_TYPE,
    FRONTAL_TYPE,
    SOL_TYPE,
    DECO_TYPE,
    COMPDECO_TYPE
} t_blocktype;

typedef struct {
    char* tiles;
    char* rotation;
    t_blocktype type;
    double proba;
    char* name;
} t_block;

typedef struct {
    int nbElem;
    t_block** listeBlock;
} t_listeBlock;

t_listeBlock** InitAllBlock();
void freeListeBlock(t_listeBlock** lab);
void ajouterBlock(t_listeBlock** lab, t_blocktype type, char* name, char* tailes, char* rotation, double proba);
t_listeBlock* listeByType(t_listeBlock** lab, t_blocktype type);
t_block* blockByName(t_listeBlock* lb, char* name);
void afficherBlock(t_block* b);
void afficher(t_listeBlock** lab);
t_block* randomBlocByType(t_listeBlock* lb);
t_listeBlock** createListAllBlock();

#endif