#ifndef LISTB_H
#define LISTB_H

#include <stdio.h>
#include <stdlib.h>

#include "general.h"

#define NBTYPE 6

typedef enum {
    MUR_ANGLE_DROIT,
    MUR_ANGLE_GAUCHE,
    MUR_ARRONDI_INF_GAUCHE,
    MUR_ARRONDI_INF_DROIT,
    MUR_ARRONDI_SUP_GAUCHE,
    MUR_ARRONDI_SUP_DROIT,
    MUR_ANGLE_CONTINUE_GAUCHE,
    MUR_ANGLE_CONTINUE_DROIT,
    MUR_BORDURE_MUR_AVANT,
    MUR_BORDURE_MUR_ARRIERE,
    MUR_BORDURE_MUR_GAUCHE,
    MUR_BORDURE_MUR_DROIT,

    SOL_SIMPLE,
    SOL_FRACTURE,
    SOL_POINT,

    FRONTALE_BRIQUE_SIMPLE,
    FRONTALE_BRIQUE_FENETRE,
    FRONTALE_DRAPEAU,
    FRONTALE_FONTAINE_EAUX,
    FRONTALE_FONTAINE_PAS_EAUX,

    PLAFOND_SIMPLE,
    PLAFOND_FRACTURE,
    PLAFOND_POINT,

    DECO_HAUT_BOITE,
    DECO_CROIX_TOMBE,
    DECO_PIERRE_TOMBALE,
    DECO_TABLE,
    DECO_ENCLUME,

    COMPDECO_BAS_BOITE,
    COMPDECO_CHAISE,
    COMPDECO_TOMBE,
    COMPDECO_BAS_FONTAINE_EAUX,
    COMPDECO_BAS_FONTAINE_SANS_EAUX
} nom_block;

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
    nom_block name;
} t_block;

typedef struct {
    int nbElem;
    t_block** listeBlock;
} t_listeBlock;

t_listeBlock** InitAllBlock();
void freeListeBlock(t_listeBlock** lab);
void ajouterBlock(t_listeBlock** lab, t_blocktype type, nom_block name, char* tailes, char* rotation, double proba);
t_listeBlock* listeByType(t_listeBlock** lab, t_blocktype type);
t_block* blockByName(t_listeBlock* lb, nom_block name);
void afficher(t_listeBlock** lab);
t_block* randomBlocByType(t_listeBlock* lb);
t_listeBlock** createListAllBlock();

#endif