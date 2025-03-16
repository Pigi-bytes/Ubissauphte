#ifndef LISTB_H
#define LISTB_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    MUR_BORDURE_MUR_AVANT_PILONNE,
    MUR_BORDURE_MUR_ARRIERE,
    MUR_BORDURE_MUR_GAUCHE,
    MUR_BORDURE_MUR_DROIT,

    SOL_SIMPLE,
    SOL_FRACTURE,
    SOL_POINT,
    SOL_OMBRE_MUR,
    SOL_OMBRE_MUR_GAUCHE,
    SOL_OMBRE_ARRONDI_DROIT,
    SOL_OMBRE_ARRONDI_GAUCHE,
    SOL_OMBRE_ANGLE_GAUCHE,
    SOL_OMBRE_ANGLE_DROIT,
    SOL_BAS_PILONNE,
    SOL_BAS_FONTAINE_EAUX_PROFOND,
    SOL_BAS_FONTAINE_EAUX_GRILLE,
    SOL_BAS_FONTAINE_SANS_EAUX_PROFOND,
    SOL_BAS_FONTAINE_SANS_EAUX_GRILLE,
    SOL_BAS_BOITE,
    SOL_TOMBE,

    FRONTALE_BRIQUE_SIMPLE,
    FRONTALE_BRIQUE_FENETRE,
    FRONTALE_DRAPEAU,
    FRONTALE_FONTAINE_EAUX,
    FRONTALE_FONTAINE_PAS_EAUX,
    FRONTALE_ANGLE_GAUCHE,
    FRONTALE_ANGLE_DROIT,
    FRONTALE_CORP_PILONNE,

    PLAFOND_SIMPLE,
    PLAFOND_FRACTURE,
    PLAFOND_POINT,

    DECO_HAUT_BOITE,
    DECO_CROIX_TOMBE,
    DECO_PIERRE_TOMBALE,
    DECO_TABLE,
    DECO_ENCLUME,
    DECO_TONNEAU,

    COMPDECO_DEBUT_ESCALIER,
    COMPDECO_MILLIEU_ESCALIER,
    COMPDECO_FIN_ESCALIER,
    COMPDECO_SOLO_ESCALIER,
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
    int tiles;
    int rotation;
    SDL_bool collisions;
    t_blocktype type;
    double proba;
    nom_block name;
    SDL_bool rotationAutorise;
} t_block;

typedef struct {
    int nbElem;
    t_block** listeBlock;
} t_listeBlock;

t_listeBlock** InitAllBlock();
void freeListeBlock(t_listeBlock** listAllBlock);
void ajouterBlock(t_listeBlock** listAllBlock, t_blocktype type, nom_block name, int tailes, int rotation, double proba, SDL_bool collisions, SDL_bool rotationAutorise);
t_listeBlock* listeByType(t_listeBlock** listAllBlock, t_blocktype type);
void rotationAleatoire(t_block* block);
t_block* blockByName(t_listeBlock* listBlock, nom_block name);
SDL_bool blockIs(t_block* block, nom_block nom);
t_block* randomBlocByType(t_listeBlock* listBlock);
t_listeBlock** createListAllBlock();
void copierVal(t_block* src, t_block** det);
#endif