#include "listeBlock.h"

t_listeBlock** InitAllBlock() {
    t_listeBlock** listAllBlock = malloc(sizeof(t_listeBlock*) * NBTYPE);
    if (!listAllBlock) {
        printf("erreur lors de l'allocation de mémoire de listBlock\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NBTYPE; i++) {
        listAllBlock[i] = malloc(sizeof(t_listeBlock));
        if (!listAllBlock[i]) {
            printf("erreur lors de l'allocation de mémoire de listBlock[i]\n");
            exit(EXIT_FAILURE);
        }
        listAllBlock[i]->listeBlock = malloc(sizeof(t_block));
        if (!listAllBlock[i]->listeBlock) {
            printf("erreur lors de l'allocation de mémoire de la liste de block\n");
            exit(EXIT_FAILURE);
        }
        listAllBlock[i]->nbElem = 0;
    }
    return listAllBlock;
}

void freeListeBlock(t_listeBlock** listAllBlock) {
    for (int i = 0; i < NBTYPE; i++) {
        for (int j = 0; j < listAllBlock[i]->nbElem; j++) {
            free(listAllBlock[i]->listeBlock[j]);
        }
        free(listAllBlock[i]->listeBlock);
    }
    free(listAllBlock);
}

void ajouterBlock(t_listeBlock** listAllBlock, t_blocktype type, nom_block name, int tailes, int rotation, double proba, SDL_bool collisions, SDL_bool rotationAutorise) {
    t_block* block = malloc(sizeof(t_block));
    if (!block) {
        printf("erreur lors de l'allocation de mémoire de listBlock\n");
        exit(EXIT_FAILURE);
    }
    block->tiles = tailes;
    block->rotation = (type == MUR_TYPE) ? 0 : rotation;

    block->proba = (double)proba;
    block->type = type;
    block->name = name;
    block->collisions = collisions;
    block->rotationAutorise = rotationAutorise;

    listAllBlock[type]->listeBlock =
        realloc(listAllBlock[type]->listeBlock, sizeof(t_block) * (listAllBlock[type]->nbElem + 1));
    listAllBlock[type]->listeBlock[listAllBlock[type]->nbElem++] = block;
}

t_listeBlock* listeByType(t_listeBlock** listAllBlock, t_blocktype type) {
    return listAllBlock[type];
}

t_block* blockByName(t_listeBlock* listBlock, nom_block nom_block) {
    for (int i = 0; i < listBlock->nbElem; i++) {
        if (listBlock->listeBlock[i]->name == nom_block) {
            return listBlock->listeBlock[i];
        }
    }
    return NULL;
}

t_block* randomBlocByType(t_listeBlock* listBlock) {
    double nb = (rand() + 0.5) / ((double)RAND_MAX + 1);
    double somme = 0;
    int i;
    for (i = 0; somme < nb && i < listBlock->nbElem; i++) {
        somme += listBlock->listeBlock[i]->proba;
        if (somme >= nb) {
            return listBlock->listeBlock[i];
        }
    }

    return listBlock->listeBlock[0];
}

void rotationAleatoire(t_block* block) {
    if (block->rotationAutorise == SDL_TRUE) {
        block->rotation = rand() % 5;
    }
}

SDL_bool blockIs(t_block* block, nom_block nom) {
    return block->name == nom;
}

t_listeBlock**
createListAllBlock() {
    t_listeBlock** listAllBlock = InitAllBlock();

    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ANGLE_DROIT, 6, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ANGLE_GAUCHE, 5, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ARRONDI_INF_GAUCHE, 28, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ARRONDI_INF_DROIT, 26, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ARRONDI_SUP_GAUCHE, 4, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ARRONDI_SUP_DROIT, 2, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ANGLE_CONTINUE_GAUCHE, 17, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_ANGLE_CONTINUE_DROIT, 18, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_BORDURE_MUR_AVANT, 3, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_BORDURE_MUR_AVANT_PILONNE, 7, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_BORDURE_MUR_ARRIERE, 27, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_BORDURE_MUR_GAUCHE, 14, 0, 1, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, MUR_TYPE, MUR_BORDURE_MUR_DROIT, 16, 0, 1, SDL_TRUE, SDL_FALSE);

    ajouterBlock(listAllBlock, SOL_TYPE, SOL_SIMPLE, 49, 0, (0.33), SDL_FALSE, SDL_TRUE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_FRACTURE, 43, 0, (0.33), SDL_FALSE, SDL_TRUE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_POINT, 50, 0, 0.33, SDL_FALSE, SDL_TRUE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_OMBRE_MUR, 52, 0, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_OMBRE_MUR_GAUCHE, 51, 0, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_OMBRE_ARRONDI_DROIT, 53, 0, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_OMBRE_ARRONDI_GAUCHE, 53, 1, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_OMBRE_ANGLE_GAUCHE, 54, 0, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_OMBRE_ANGLE_DROIT, 54, 1, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_BAS_PILONNE, 31, 0, 0.0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_BAS_FONTAINE_EAUX_PROFOND, 33, 0, 0, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_BAS_FONTAINE_EAUX_GRILLE, 45, 0, 0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_BAS_FONTAINE_SANS_EAUX_PROFOND, 32, 0, 0, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_BAS_FONTAINE_SANS_EAUX_GRILLE, 44, 0, 0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_BAS_BOITE, 76, 0, 0, SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, SOL_TYPE, SOL_TOMBE, 67, 0, 0, SDL_FALSE, SDL_FALSE);

    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_BRIQUE_SIMPLE, 41, 0, (0.3), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_BRIQUE_FENETRE, 29, 0, (0.3), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_DRAPEAU, 30, 0, (0.3), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_FONTAINE_EAUX, 21, 0, (0.05), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_FONTAINE_PAS_EAUX, 20, 0, (0.05), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_ANGLE_DROIT, 58, 0, (0.0), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_ANGLE_GAUCHE, 60, 0, (0.0), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, FRONTAL_TYPE, FRONTALE_CORP_PILONNE, 19, 0, (0.0), SDL_TRUE, SDL_FALSE);

    ajouterBlock(listAllBlock, PLAFOND_TYPE, PLAFOND_SIMPLE, 1, 0, (((double)2) / ((double)4)), SDL_TRUE, SDL_TRUE);
    ajouterBlock(listAllBlock, PLAFOND_TYPE, PLAFOND_FRACTURE, 13, 0, (((double)2) / ((double)4)), SDL_TRUE, SDL_TRUE);
    ajouterBlock(listAllBlock, PLAFOND_TYPE, PLAFOND_POINT, 25, 0, (((double)2) / ((double)4)), SDL_TRUE, SDL_TRUE);

    ajouterBlock(listAllBlock, DECO_TYPE, DECO_HAUT_BOITE, 64, 0, (((double)1) / ((double)6)), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, DECO_TYPE, DECO_CROIX_TOMBE, 65, 0, (((double)1) / ((double)6)), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, DECO_TYPE, DECO_PIERRE_TOMBALE, 66, 0, (((double)1) / ((double)6)), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, DECO_TYPE, DECO_TABLE, 73, 0, (((double)1) / ((double)6)), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, DECO_TYPE, DECO_ENCLUME, 75, 0, (((double)1) / ((double)6)), SDL_TRUE, SDL_FALSE);
    ajouterBlock(listAllBlock, DECO_TYPE, DECO_TONNEAU, 83, 0, (((double)1) / ((double)6)), SDL_TRUE, SDL_FALSE);

    ajouterBlock(listAllBlock, COMPDECO_TYPE, COMPDECO_DEBUT_ESCALIER, 37, 0, 0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, COMPDECO_TYPE, COMPDECO_MILLIEU_ESCALIER, 38, 0, 0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, COMPDECO_TYPE, COMPDECO_FIN_ESCALIER, 39, 0, 0, SDL_FALSE, SDL_FALSE);
    ajouterBlock(listAllBlock, COMPDECO_TYPE, COMPDECO_FIN_ESCALIER, 40, 0, 0, SDL_FALSE, SDL_FALSE);

    return listAllBlock;
}

void copierVal(t_block* src, t_block** det) {
    (*det) = malloc(sizeof(t_block));
    (*det)->collisions = src->collisions;
    (*det)->tiles = src->tiles;
    (*det)->rotation = src->rotation;
    (*det)->tiles = src->tiles;
    (*det)->rotationAutorise = src->rotationAutorise;
    (*det)->name = src->name;
    (*det)->type = src->type;
}
