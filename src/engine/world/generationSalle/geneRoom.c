
#include "geneRoom.h"

t_block *getPlafond(t_listeBlock **listAllBlock) {
    t_listeBlock *listBlock = listeByType(listAllBlock, PLAFOND_TYPE);
    return randomBlocByType(listBlock);
}

t_block *getSol(t_listeBlock **listAllBlock, t_case *c) {
    t_listeBlock *listBlock = listeByType(listAllBlock, SOL_TYPE);
    if (c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, FRONTALE_CORP_PILONNE))
        return blockByName(listBlock, SOL_BAS_PILONNE);
    if (c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, FRONTALE_FONTAINE_PAS_EAUX)) {
        if (rand() % 2)
            return blockByName(listBlock, SOL_BAS_FONTAINE_SANS_EAUX_GRILLE);
        return blockByName(listBlock, SOL_BAS_FONTAINE_SANS_EAUX_PROFOND);
    }
    if (c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, FRONTALE_FONTAINE_EAUX)) {
        if (rand() % 2)
            return blockByName(listBlock, SOL_BAS_FONTAINE_EAUX_GRILLE);
        return blockByName(listBlock, SOL_BAS_FONTAINE_EAUX_PROFOND);
    }
    if ((c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE))
        return blockByName(listBlock, SOL_OMBRE_ARRONDI_DROIT);
    if ((c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE))
        return blockByName(listBlock, SOL_OMBRE_ARRONDI_GAUCHE);
    if (c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE && (c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val == OBSTACLE || (c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val == OBSTACLE))) {
        if (rand() % 2)
            return blockByName(listBlock, SOL_OMBRE_MUR_GAUCHE);
        return blockByName(listBlock, SOL_OMBRE_MUR);
    }
    if ((c->tabVoisin[VOISIN_HAUT2]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE))
        return blockByName(listBlock, SOL_OMBRE_ANGLE_DROIT);
    if ((c->tabVoisin[VOISIN_HAUT2]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE))
        return blockByName(listBlock, SOL_OMBRE_ANGLE_GAUCHE);

    return randomBlocByType(listBlock);
}

t_block *getDeco(t_listeBlock **listAllBlock) {
    t_listeBlock *listBlock = listeByType(listAllBlock, DECO_TYPE);
    return randomBlocByType(listBlock);
}

t_block *getFrontale(t_listeBlock **listAllBlock, t_case *c) {
    t_listeBlock *listBlock = listeByType(listAllBlock, FRONTAL_TYPE);
    if (c->tabVoisin[VOISIN_GAUCHE]->val == SOL)
        return blockByName(listBlock, FRONTALE_ANGLE_DROIT);
    if (c->tabVoisin[VOISIN_DROIT]->val == SOL)
        return blockByName(listBlock, FRONTALE_ANGLE_GAUCHE);
    if (c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, MUR_BORDURE_MUR_AVANT_PILONNE))
        return blockByName(listBlock, FRONTALE_CORP_PILONNE);
    return randomBlocByType(listBlock);
}

SDL_bool blocSeul(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_GAUCHE], c->tabVoisin[VOISIN_DROIT]) && (!(c->tabVoisin[VOISIN_HAUT]->val)) && (!(c->tabVoisin[VOISIN_BAS]->val)) && (!(c->tabVoisin[VOISIN_GAUCHE]->val)) && (!(c->tabVoisin[VOISIN_DROIT])->val));
}

SDL_bool murAvant(t_case *c) {
    return EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS]) && (!(c->tabVoisin[VOISIN_BAS]->val));
}

SDL_bool angleDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DROIT]) && (!(c->tabVoisin[VOISIN_HAUT]->val)) && ((!c->tabVoisin[VOISIN_DROIT]->val) || ((c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) && EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]) && (!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val)))));
}

SDL_bool angleGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_GAUCHE]) && (!(c->tabVoisin[VOISIN_HAUT]->val)) && ((!(c->tabVoisin[VOISIN_GAUCHE]->val)) || ((c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) && EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]) && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val)))));
}

SDL_bool arrondiInferieurDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DIAG_DROIT_HAUT], c->tabVoisin[VOISIN_DROIT]) && (c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val)));
}

SDL_bool arrondiInferieurGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT], c->tabVoisin[VOISIN_GAUCHE]) && (c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val)));
}

SDL_bool arrondiSuperieurDroit(t_case *c) {
    return (
        (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_DROIT_BAS], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT], c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val))) || (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_DROIT_BAS], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val == OBSTACLE) && (!existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT])) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val))));
}

SDL_bool arrondiSuperieurGauche(t_case *c) {
    return (
        (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS], c->tabVoisin[VOISIN_DIAG_DROIT_HAUT], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val))) || (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val == OBSTACLE) && (!existe(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT])) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val))));
}

SDL_bool murArriere(t_case *c) {
    return EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT]) && (!(c->tabVoisin[VOISIN_HAUT]->val));
}

SDL_bool angleContinueGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_BAS2]) && ((c->tabVoisin[VOISIN_BAS]->val == OBSTACLE)) && (!(c->tabVoisin[VOISIN_BAS2]->val)) && ((existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]) && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))) || (existe(c->tabVoisin[VOISIN_GAUCHE]) && (!(c->tabVoisin[VOISIN_GAUCHE]->val)))));
}

SDL_bool angleContinueDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_BAS2]) && ((c->tabVoisin[VOISIN_BAS]->val == OBSTACLE)) && (!(c->tabVoisin[VOISIN_BAS2]->val)) && ((existe(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]) && (!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))) || (existe(c->tabVoisin[VOISIN_DROIT]) && (!(c->tabVoisin[VOISIN_DROIT]->val)))));
}

SDL_bool bordureMurAvant(t_case *c) {
    return EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS2]) && (!(c->tabVoisin[VOISIN_BAS2]->val));
}

SDL_bool bordureMurGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_DROIT]) && (((!(c->tabVoisin[VOISIN_DROIT]->val))) || ((EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])) && ((c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))))));
}

SDL_bool bordureMurDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_GAUCHE]) && (((!(c->tabVoisin[VOISIN_GAUCHE]->val))) || ((EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])) && ((c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))))));
}

void choixTiles(t_listeBlock **listAllBlock, t_grille *g) {
    t_listeBlock *listBlock = listeByType(listAllBlock, MUR_TYPE);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            if ((!g->grille[i][j]->val)) {
                copierVal(getSol(listAllBlock, g->grille[i][j]), &(g->grille[i][j]->tiles));
                rotationAleatoire(g->grille[i][j]->tiles);
            } else if (blocSeul(g->grille[i][j])) {
                copierVal(getDeco(listAllBlock), &(g->grille[i][j]->tiles));
            } else if (murAvant(g->grille[i][j])) {
                copierVal(getFrontale(listAllBlock, g->grille[i][j]), &(g->grille[i][j]->tiles));
            } else if (angleDroit(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ANGLE_DROIT), &(g->grille[i][j]->tiles));
            } else if (angleGauche(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ANGLE_GAUCHE), &(g->grille[i][j]->tiles));
            } else if (arrondiInferieurDroit(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ARRONDI_INF_DROIT), &(g->grille[i][j]->tiles));
            } else if (arrondiInferieurGauche(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ARRONDI_INF_GAUCHE), &(g->grille[i][j]->tiles));
            } else if (arrondiSuperieurDroit(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ARRONDI_SUP_DROIT), &(g->grille[i][j]->tiles));
            } else if (arrondiSuperieurGauche(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ARRONDI_SUP_GAUCHE), &(g->grille[i][j]->tiles));
            } else if (murArriere(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_BORDURE_MUR_ARRIERE), &(g->grille[i][j]->tiles));
            } else if (angleContinueGauche(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ANGLE_CONTINUE_GAUCHE), &(g->grille[i][j]->tiles));
            } else if (angleContinueDroit(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_ANGLE_CONTINUE_DROIT), &(g->grille[i][j]->tiles));
            } else if (bordureMurAvant(g->grille[i][j])) {
                if (rand() % 5 < 2) {
                    copierVal(blockByName(listBlock, MUR_BORDURE_MUR_AVANT_PILONNE), &(g->grille[i][j]->tiles));
                } else {
                    copierVal(blockByName(listBlock, MUR_BORDURE_MUR_AVANT), &(g->grille[i][j]->tiles));
                }
            } else if (bordureMurGauche(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_BORDURE_MUR_GAUCHE), &(g->grille[i][j]->tiles));
            } else if (bordureMurDroit(g->grille[i][j])) {
                copierVal(blockByName(listBlock, MUR_BORDURE_MUR_DROIT), &(g->grille[i][j]->tiles));
            } else {
                copierVal(getPlafond(listAllBlock), &(g->grille[i][j]->tiles));

                rotationAleatoire(g->grille[i][j]->tiles);
            }
        }
    }
}

void saveMap(t_grille *g) {
    system("ls  ./assets/map/map*.txt|wc -l > ./src/nb.txt");
    FILE *nbFichier = fopen("./src/nb.txt", "r");
    if (!nbFichier) {
        perror("problème d'ouverture du fichie 1r\n");
        exit(EXIT_FAILURE);
    }
    char nb[100];
    int entier;
    fscanf(nbFichier, "%d", &entier);
    system("rm ./src/nb.txt");
    entier += 1;
    sprintf(nb, "%d", entier);
    char chaine[100] = "map";
    strcat(chaine, nb);
    strcat(chaine, ".txt");
    fclose(nbFichier);
    char ouverture[100] = "assets/map/";
    strcat(ouverture, chaine);
    FILE *fichier = fopen(ouverture, "a");
    if (!fichier) {
        perror("problème d'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
    sprintf(g->nom, "%s", ouverture);
    fprintf(fichier, "%dx%dx1\n", g->nbColonne, g->nbLigne);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            fprintf(fichier, "%d:%d:%d ", g->grille[i][j]->tiles->tiles, g->grille[i][j]->tiles->rotation, g->grille[i][j]->tiles->collisions);
        }
        fprintf(fichier, "\n");
    }
    fprintf(fichier, "\n");
    fclose(fichier);
}

t_grille *geneRoom() {
    srand(time(NULL));

    int nbLigne = (rand() % 60 + 20) & ~1;
    int nbColonne = (rand() % 2) == 0 ? (nbLigne + rand() % 11) & ~1 : (nbLigne - rand() % 11) & ~1;

    SDL_FPoint seed;
    seed.x = ((rand()) % 90 + 10) + (((float)(rand() + 1)) / RAND_MAX);
    seed.y = ((rand()) % 90 + 10) + (((float)(rand() + 1)) / RAND_MAX);

    int denominateur = rand() % 5 + 5;

    t_listeBlock **listAllBlock = createListAllBlock();

    float **values = createMatriceFloat(nbLigne, nbColonne);

    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            SDL_FPoint st = {(float)i / denominateur, (float)j / denominateur};  // plus le dénominateur est petit plus il y à de murs
            values[i][j] = noise(st, seed);
        }
    }

    int **entier = matriceFloat2Int(values, nbLigne, nbColonne);
    // afficheMat(entier);
    rendreMatriceContinue(entier, nbLigne, nbColonne);
    lissage(entier, nbLigne, nbColonne);

    t_grille *grille = intToGrilleNiveau(entier, nbLigne, nbColonne);

    choixTiles(listAllBlock, grille);
    saveMap(grille);

    freeMatriceInt(entier, nbLigne, nbColonne);
    freeMatriceFloat(values, nbLigne, nbColonne);
    freeListeBlock(listAllBlock);

    return grille;
}
