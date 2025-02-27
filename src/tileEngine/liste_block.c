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

void ajouterBlock(t_listeBlock** lab, t_blocktype type, char* name, char* tailes, char* rotation, double proba) {
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

    lab[type]->listeBlock = realloc(lab[type]->listeBlock, sizeof(t_block) * (lab[type]->nbElem + 1));
    lab[type]->listeBlock[lab[type]->nbElem++] = block;
}

t_listeBlock* listeByType(t_listeBlock** lab, t_blocktype type) {
    return lab[type];
}

t_block* blockByName(t_listeBlock* lb, char* name) {
    for (int i = 0; i < lb->nbElem; i++) {
        if (!(strcmp(lb->listeBlock[i]->name, name))) {
            return lb->listeBlock[i];
        }
    }
    return NULL;
}

void afficherBlock(t_block* b) {
    printf("type : %d\n", b->type);
    printf("tails : %s:%s\n", b->tiles, b->rotation);
    printf("nom : %s\n", b->name);
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

    ajouterBlock(lab, MUR_TYPE, "angleDroit", "6", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "angleGauche", "5", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "arrondiInfGauche", "28", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "arrondiInfDroit", "26", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "arrondiSupGauche", "4", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "arrondiSuDroit", "2", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "angleContinueGauche", "17", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "angleContinueDroit", "18", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "bordureMurAvant", "3", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "bordureMurArriere", "27", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "bordureMurGauche", "14", "0", 1);
    ajouterBlock(lab, MUR_TYPE, "bordureMurDroit", "16", "0", 1);

    ajouterBlock(lab, SOL_TYPE, "solSimple", "49", "0", (0.33));
    ajouterBlock(lab, SOL_TYPE, "solFracture", "43", "0", (0.33));
    ajouterBlock(lab, SOL_TYPE, "solPoint", "50", "0", 0.33);

    ajouterBlock(lab, FRONTAL_TYPE, "briqueSimple", "41", "0", (((double)2) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, "briqueFenetre", "29", "0", (((double)2) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, "drapeau", "30", "0", (((double)2) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, "fontaineAll", "21", "0", (((double)1) / ((double)8)));
    ajouterBlock(lab, FRONTAL_TYPE, "fontainePasAll", "20", "0", (((double)1) / ((double)8)));

    ajouterBlock(lab, PLAFOND_TYPE, "plafondSimple", "1", "0", (((double)2) / ((double)4)));
    ajouterBlock(lab, PLAFOND_TYPE, "plafondFracture", "13", "0", (((double)2) / ((double)4)));
    ajouterBlock(lab, PLAFOND_TYPE, "plafondPoint", "25", "0", (((double)2) / ((double)4)));

    ajouterBlock(lab, DECO_TYPE, "hautBoite", "69", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, "croixTombe", "65", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, "pierreTombale", "66", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, "table", "73", "0", (((double)1) / ((double)5)));
    ajouterBlock(lab, DECO_TYPE, "enclume", "66", "0", (((double)1) / ((double)5)));

    ajouterBlock(lab, COMPDECO_TYPE, "basBoite", "76", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, "basBoite", "76", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, "tombe", "66", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, "basFontaineEaux", "33", "0", 1);
    ajouterBlock(lab, COMPDECO_TYPE, "basFontaineSansEaux", "32", "0", 1);

    return lab;
}
