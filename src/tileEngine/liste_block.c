#include "liste_block.h"

t_listeBlock** InitAllBlock() {
    t_listeBlock** lab = malloc(sizeof(t_listeBlock*) * NBTYPE);
    if (!lab) {
        printf("erreur lors de l'allocation de mémoire de lb\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NBTYPE; i++) {
        lab[i] = malloc(sizeof(t_listeBlock));
        if (!lab[i]) {
            printf("erreur lors de l'allocation de mémoire de lb[i]\n");
            exit(EXIT_FAILURE);
        }
        lab[i]->listeBlock = malloc(sizeof(t_block));
        if (!lab[i]->listeBlock) {
            printf("erreur lors de l'allocation de mémoire de la liste de block\n");
            exit(EXIT_FAILURE);
        }
        lab[i]->nbElem = 0;
    }
    return lab;
}

void freeListeBlock(t_listeBlock** lab) {
    for (int i = 0; i < NBTYPE; i++) {
        for (int j = 0; j < lab[i]->nbElem; j++) {
            free(lab[i]->listeBlock[j]);
        }
        free(lab[i]->listeBlock);
    }
    free(lab);
}

void ajouterBlock(t_listeBlock** lab, t_blocktype type, nom_block name,
                  char* tailes, char* rotation, double proba) {
    t_block* block = malloc(sizeof(t_block));
    if (!block) {
        printf("erreur lors de l'allocation de mémoire de lb\n");
        exit(EXIT_FAILURE);
    }
    block->tiles = tailes;
    block->rotation = (type == MUR_TYPE) ? "0" : rotation;
    block->proba = (double)proba;
    block->type = type;
    block->name = name;

    lab[type]->listeBlock =
        realloc(lab[type]->listeBlock, sizeof(t_block) * (lab[type]->nbElem + 1));
    lab[type]->listeBlock[lab[type]->nbElem++] = block;
}

t_listeBlock* listeByType(t_listeBlock** lab, t_blocktype type) {
    return lab[type];
}

t_block* blockByName(t_listeBlock* lb, nom_block nom_block) {
    for (int i = 0; i < lb->nbElem; i++) {
        if (lb->listeBlock[i]->name == nom_block) {
            return lb->listeBlock[i];
        }
    }
    return NULL;
}

void afficherBlock(t_block* b) {
    printf("type : %d\n", b->type);
    printf("tails : %s:%s\n", b->tiles, b->rotation);
    printf("nom : %d\n", b->name);
    printf("proba : %lf\n", b->proba);
}

void afficher(t_listeBlock** lab) {
    for (int i = 0; i < NBTYPE; i++) {
        for (int j = 0; j < lab[i]->nbElem; j++) {
            afficherBlock(lab[i]->listeBlock[j]);
        }
        printf("\n");
    }
}

t_block* randomBlocByType(t_listeBlock* lb) {
    double nb = (rand() + 0.5) / ((double)RAND_MAX + 1);
    printf("%lf", nb);
    double somme = 0;
    int i;
    for (i = 0; somme < nb && i < lb->nbElem; i++) {
        somme += lb->listeBlock[i]->proba;
        if (somme >= nb) {
            return lb->listeBlock[i];
        }
    }

    return lb->listeBlock[lb->nbElem - 1];
}

t_listeBlock** createListAllBlock() {
    t_listeBlock** lab = InitAllBlock();

    ajouterBlock(lab, MUR_TYPE, MUR_ANGLE_DROIT, "6", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ANGLE_GAUCHE, "5", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ARRONDI_INF_GAUCHE, "28", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ARRONDI_INF_DROIT, "26", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ARRONDI_SUP_GAUCHE, "4", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ARRONDI_SUP_DROIT, "2", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ANGLE_CONTINUE_GAUCHE, "17", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_ANGLE_CONTINUE_DROIT, "18", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_BORDURE_MUR_AVANT, "3", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_BORDURE_MUR_ARRIERE, "27", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_BORDURE_MUR_GAUCHE, "14", "0", 1);
    ajouterBlock(lab, MUR_TYPE, MUR_BORDURE_MUR_DROIT, "16", "0", 1);

    ajouterBlock(lab, SOL_TYPE, SOL_SIMPLE, "49", "0", (0.33));
    ajouterBlock(lab, SOL_TYPE, SOL_FRACTURE, "43", "0", (0.33));
    ajouterBlock(lab, SOL_TYPE, SOL_POINT, "50", "0", 0.33);

    ajouterBlock(lab, FRONTAL_TYPE, FRONTALE_BRIQUE_SIMPLE, "41", "0", (((double)2) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, FRONTALE_BRIQUE_FENETRE, "29", "0", (((double)2) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, FRONTALE_DRAPEAU, "30", "0", (((double)2) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, FRONTALE_FONTAINE_EAUX, "21", "0", (((double)1) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, FRONTALE_FONTAINE_PAS_EAUX, "20", "0", (((double)1) / ((double)8)));

    ajouterBlock(lab, PLAFOND_TYPE, PLAFOND_SIMPLE, "1", "0", (((double)2) / ((double)4)));
    ajouterBlock(lab, PLAFOND_TYPE, PLAFOND_FRACTURE, "13", "0", (((double)2) / ((double)4)));
    ajouterBlock(lab, PLAFOND_TYPE, PLAFOND_POINT, "25", "0", (((double)2) / ((double)4)));

    ajouterBlock(lab, DECO_TYPE, DECO_HAUT_BOITE, "69", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, DECO_CROIX_TOMBE, "65", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, DECO_PIERRE_TOMBALE, "66", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, DECO_TABLE, "73", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, DECO_ENCLUME, "66", "0", (((double)0.25f) / ((double)5)));

    ajouterBlock(lab, COMPDECO_TYPE, COMPDECO_BAS_BOITE, "76", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, COMPDECO_CHAISE, "65", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, COMPDECO_TOMBE, "66", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, COMPDECO_BAS_FONTAINE_EAUX, "33", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, COMPDECO_BAS_FONTAINE_SANS_EAUX, "32", "0", 1);

    return lab;
}
