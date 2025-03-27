
#include "geneRoom.h"

t_block *getPlafond(t_listeBlock **listAllBlock) {
    t_listeBlock *listBlock = listeByType(listAllBlock, PLAFOND_TYPE);
    return randomBlocByType(listBlock);
}

t_block *getSol(t_listeBlock **listAllBlock, t_case *c) {
    t_listeBlock *listBlock = listeByType(listAllBlock, SOL_TYPE);
    if (existe(c->tabVoisin[VOISIN_HAUT]) && c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, FRONTALE_CORP_PILONNE))
        return blockByName(listBlock, SOL_BAS_PILONNE);
    if (existe(c->tabVoisin[VOISIN_HAUT]) && c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, FRONTALE_FONTAINE_PAS_EAUX)) {
        c->val = ELTAJOUTE;
        if (rand() % 2 && c->tabVoisin[VOISIN_BAS]->val == SOL && c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS] == SOL && c->tabVoisin[VOISIN_DIAG_DROIT_BAS] == SOL)
            return blockByName(listBlock, SOL_BAS_FONTAINE_SANS_EAUX_PROFOND);
        return blockByName(listBlock, SOL_BAS_FONTAINE_SANS_EAUX_GRILLE);
    }
    if (existe(c->tabVoisin[VOISIN_HAUT]) && c->tabVoisin[VOISIN_HAUT]->tiles != NULL && (blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, DECO_CROIX_TOMBE) || blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, DECO_PIERRE_TOMBALE))) {
        c->val = ELTAJOUTE;
        return blockByName(listBlock, SOL_TOMBE);
    }
    if (existe(c->tabVoisin[VOISIN_HAUT]) && c->tabVoisin[VOISIN_HAUT]->tiles != NULL && (blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, DECO_HAUT_BOITE))) {
        c->val = ELTAJOUTE;
        return blockByName(listBlock, SOL_BAS_BOITE);
    }
    if (existe(c->tabVoisin[VOISIN_HAUT]) && c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, FRONTALE_FONTAINE_EAUX)) {
        c->val = ELTAJOUTE;
        if (rand() % 2 && c->tabVoisin[VOISIN_BAS]->val == SOL && c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS] == SOL && c->tabVoisin[VOISIN_DIAG_DROIT_BAS] == SOL)

            return blockByName(listBlock, SOL_BAS_FONTAINE_EAUX_PROFOND);
        return blockByName(listBlock, SOL_BAS_FONTAINE_EAUX_GRILLE);
    }

    if (EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_GAUCHE]) && (c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE)) {
        c->val = ELTAJOUTE;
        return blockByName(listBlock, SOL_OMBRE_ARRONDI_GAUCHE);
    }
    if ((EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT]) && c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && ((EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]) && c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val == OBSTACLE) || (EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val == OBSTACLE)))) {
        c->val = ELTAJOUTE;
        if (rand() % 2)
            return blockByName(listBlock, SOL_OMBRE_MUR2);
        return blockByName(listBlock, SOL_OMBRE_MUR);
    }
    if (EXISTE_VOISIN(c->tabVoisin[VOISIN_GAUCHE]) && c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) {
        c->val = ELTAJOUTE;
        if (rand() % 2)
            return blockByName(listBlock, SOL_OMBRE_MUR_GAUCHE2);
        return blockByName(listBlock, SOL_OMBRE_MUR_GAUCHE);
    }
    if (EXISTE_VOISIN(c->tabVoisin[VOISIN_GAUCHE], c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]) && c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIsOmbre(c->tabVoisin[VOISIN_HAUT]->tiles) && c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val == OBSTACLE && blockIsOmbre(c->tabVoisin[VOISIN_GAUCHE]->tiles)) {
        c->val = ELTAJOUTE;
        return blockByName(listBlock, SOL_OMBRE_ANGLE_GAUCHE);
    }
    return randomBlocByType(listBlock);
}

t_block *getDeco(t_listeBlock **listAllBlock) {
    t_listeBlock *listBlock = listeByType(listAllBlock, DECO_TYPE);
    return randomBlocByType(listBlock);
}

t_block *getFrontale(t_listeBlock **listAllBlock, t_case *c) {
    t_listeBlock *listBlock = listeByType(listAllBlock, FRONTAL_TYPE);
    if (c->tabVoisin[VOISIN_GAUCHE]->val != OBSTACLE)
        return blockByName(listBlock, FRONTALE_ANGLE_DROIT);
    if (c->tabVoisin[VOISIN_DROIT]->val != OBSTACLE)
        return blockByName(listBlock, FRONTALE_ANGLE_GAUCHE);
    if (blockIs(c->tabVoisin[VOISIN_GAUCHE]->tiles, FRONTALE_PORTE_SOLO)) {
        if ((((double)rand()) / RAND_MAX) < 0.2) {
            copierVal(blockByName(listBlock, FRONTALE_PORTE_DUO_GAUCHE), &c->tabVoisin[VOISIN_GAUCHE]->tiles);
            return blockByName(listBlock, FRONTALE_PORTE_DUO_DROITE);
        }
    }
    if (c->tabVoisin[VOISIN_HAUT]->tiles != NULL && blockIs(c->tabVoisin[VOISIN_HAUT]->tiles, MUR_BORDURE_MUR_AVANT_PILONNE))
        return blockByName(listBlock, FRONTALE_CORP_PILONNE);
    return randomBlocByType(listBlock);
}

SDL_bool blocSeul(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_GAUCHE], c->tabVoisin[VOISIN_DROIT]) && (c->tabVoisin[VOISIN_HAUT]->val != OBSTACLE) && ((c->tabVoisin[VOISIN_BAS]->val != OBSTACLE)) && ((c->tabVoisin[VOISIN_GAUCHE]->val != OBSTACLE)) && ((c->tabVoisin[VOISIN_DROIT])->val != OBSTACLE));
}

SDL_bool murAvant(t_case *c) {
    return EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS]) && ((c->tabVoisin[VOISIN_BAS]->val != OBSTACLE));
}

SDL_bool angleDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DROIT]) && ((c->tabVoisin[VOISIN_HAUT]->val != OBSTACLE)) && ((c->tabVoisin[VOISIN_DROIT]->val != OBSTACLE) || ((c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) && EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]) && ((c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val != OBSTACLE)))));
}

SDL_bool angleGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_GAUCHE]) && ((c->tabVoisin[VOISIN_HAUT]->val != OBSTACLE)) && (((c->tabVoisin[VOISIN_GAUCHE]->val != OBSTACLE)) || ((c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) && EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]) && ((c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val != OBSTACLE)))));
}

SDL_bool arrondiInferieurDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DIAG_DROIT_HAUT], c->tabVoisin[VOISIN_DROIT]) && (c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val != OBSTACLE)));
}

SDL_bool arrondiInferieurGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT], c->tabVoisin[VOISIN_GAUCHE]) && (c->tabVoisin[VOISIN_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val != OBSTACLE)));
}

SDL_bool arrondiSuperieurDroit(t_case *c) {
    return (
        (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_DROIT_BAS], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT], c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val != OBSTACLE))) || (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_DROIT_BAS], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val == OBSTACLE) && (!existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT])) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val != OBSTACLE))));
}

SDL_bool arrondiSuperieurGauche(t_case *c) {
    return (
        (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS], c->tabVoisin[VOISIN_DIAG_DROIT_HAUT], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val != OBSTACLE))) || (EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS], c->tabVoisin[VOISIN_BAS2], c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val == OBSTACLE) && (!existe(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT])) && (c->tabVoisin[VOISIN_BAS]->val == OBSTACLE) && (c->tabVoisin[VOISIN_BAS2]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val != OBSTACLE))));
}

SDL_bool murArriere(t_case *c) {
    return EXISTE_VOISIN(c->tabVoisin[VOISIN_HAUT]) && ((c->tabVoisin[VOISIN_HAUT]->val != OBSTACLE));
}

SDL_bool angleContinueGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_BAS2]) && ((c->tabVoisin[VOISIN_BAS]->val == OBSTACLE)) && ((c->tabVoisin[VOISIN_BAS2]->val != OBSTACLE)) && ((existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]) && ((c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val != OBSTACLE))) || (existe(c->tabVoisin[VOISIN_GAUCHE]) && ((c->tabVoisin[VOISIN_GAUCHE]->val != OBSTACLE)))));
}

SDL_bool angleContinueDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_BAS2]) && ((c->tabVoisin[VOISIN_BAS]->val == OBSTACLE)) && ((c->tabVoisin[VOISIN_BAS2]->val != OBSTACLE)) && ((existe(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]) && ((c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val != OBSTACLE))) || (existe(c->tabVoisin[VOISIN_DROIT]) && ((c->tabVoisin[VOISIN_DROIT]->val != OBSTACLE)))));
}

SDL_bool bordureMurAvant(t_case *c) {
    return EXISTE_VOISIN(c->tabVoisin[VOISIN_BAS2]) && ((c->tabVoisin[VOISIN_BAS2]->val != OBSTACLE));
}

SDL_bool bordureMurGauche(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_DROIT]) && ((((c->tabVoisin[VOISIN_DROIT]->val != OBSTACLE))) || ((EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])) && ((c->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val != OBSTACLE))))));
}

SDL_bool bordureMurDroit(t_case *c) {
    return (
        EXISTE_VOISIN(c->tabVoisin[VOISIN_GAUCHE]) && ((((c->tabVoisin[VOISIN_GAUCHE]->val != OBSTACLE))) || ((EXISTE_VOISIN(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])) && ((c->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) && ((c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val != OBSTACLE))))));
}

void choixTiles(t_listeBlock **listAllBlock, t_grille *g) {
    t_listeBlock *listBlock = listeByType(listAllBlock, MUR_TYPE);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            if (g->grille[i][j]->tiles == NULL) {
                if ((g->grille[i][j]->val != OBSTACLE)) {
                    copierVal(getSol(listAllBlock, g->grille[i][j]), &(g->grille[i][j]->tiles));
                    rotationAleatoire(g->grille[i][j]->tiles);
                } else if (blocSeul(g->grille[i][j])) {
                    copierVal(getDeco(listAllBlock),
                              &(g->grille[i][j]->tiles));
                    g->grille[i][j]->val = ELTAJOUTE;
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
}

void saveMap(t_grille *g) {
    DIR *dir;
    int count = 1;
    struct dirent *entry;
    char chaine[100];
    dir = opendir("./assets/map");
    if (!dir) {
        printf("Erreur lors de l'ouverture des fichiers");
    }

    while ((entry = readdir(dir)) != NULL) {
        if ((strncmp(entry->d_name, "map", strlen("map")) == 0) && (strstr(entry->d_name, ".txt") != NULL)) {
            count++;
        }
    }
    sprintf(chaine, "map%d.txt", count);
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

void addComplement(t_grille *g, t_listeBlock **listAllBlock) {
    int nbEscalier = rand() % 3;
    int ligne;
    int nbBlock = 0;
    int count;
    int positionDep[10];

    t_listeBlock *listBlock = listeByType(listAllBlock, COMPDECO_TYPE);

    for (int i = 0; i < nbEscalier; i++) {
        ligne = rand() % (g->nbLigne - 2) + 1;

        nbBlock = 0;
        for (int j = 0, flag = 0; j < g->nbColonne; j++) {
            if (g->grille[ligne][j]->val != OBSTACLE && !flag) {
                positionDep[nbBlock++] = j;
                flag = 1;
                count = 0;
            }
            if (g->grille[ligne][j]->val != OBSTACLE) {
                count += 1;
            } else if (g->grille[ligne][j]->val == OBSTACLE) {
                if (count > 10 && nbBlock > 0) {
                    nbBlock -= 1;
                }
                flag = 0;
            }
        }
        if (nbBlock <= 0) {
            continue;
        }
        for (int k = positionDep[rand() % nbBlock]; k < g->nbColonne && g->grille[ligne][k]->val != OBSTACLE; k++) {
            if (g->grille[ligne][k]->tabVoisin[VOISIN_HAUT]->val != OBSTACLE) {
                if (g->grille[ligne][k]->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE && g->grille[ligne][k]->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) {
                    copierVal(blockByName(listBlock, COMPDECO_SOLO_ESCALIER), &g->grille[ligne][k]->tiles);
                } else if (g->grille[ligne][k]->tabVoisin[VOISIN_GAUCHE]->val == OBSTACLE) {
                    copierVal(blockByName(listBlock, COMPDECO_DEBUT_ESCALIER), &g->grille[ligne][k]->tiles);
                } else if (g->grille[ligne][k]->tabVoisin[VOISIN_DROIT]->val == OBSTACLE) {
                    copierVal(blockByName(listBlock, COMPDECO_FIN_ESCALIER), &g->grille[ligne][k]->tiles);
                } else {
                    copierVal(blockByName(listBlock, COMPDECO_MILLIEU_ESCALIER), &g->grille[ligne][k]->tiles);
                }
                g->grille[ligne][k]->val = ELTAJOUTE;
            }
        }
    }

    int nbCoffre;
    int nbTonneau;
    long int val = (g->nbLigne * g->nbColonne);
    if (val < 1600) {
        nbCoffre = 1;
        nbTonneau = rand() % 5 + 5;
    } else if (val < 4900) {
        nbCoffre = 2;
        nbTonneau = rand() % 9 + 6;
    } else {
        nbCoffre = rand() % 2 + 3;
        nbTonneau = rand() % 10 + 10;
    }

    for (int i = 0; i < nbCoffre; i++) {
        int x, y;
        do {
            x = rand() % g->nbLigne;
            y = rand() % g->nbColonne;
        } while (g->grille[x][y]->val != SOL);
        copierVal(blockByName(listBlock, COMPDECO_COFFRE), &g->grille[x][y]->tiles);
        g->grille[x][y]->val = ELTAJOUTE;
    }

    for (int i = 0; i < nbTonneau; i++) {
        int x, y;
        do {
            x = rand() % g->nbLigne;
            y = rand() % g->nbColonne;
        } while (g->grille[x][y]->val != SOL);
        copierVal(blockByName(listBlock, COMPDECO_TONNEAU), &g->grille[x][y]->tiles);
        g->grille[x][y]->val = ELTAJOUTE;
    }
}

int estOuvert(t_case *c, int changementLigne, int changementColonne) {
    if (changementLigne == 0 && changementColonne == 1) {
        return ((existe(c->tabVoisin[VOISIN_DROIT]) && (c->tabVoisin[VOISIN_DROIT]->val == SOL)) || (existe(c->tabVoisin[VOISIN_BAS]) && (c->tabVoisin[VOISIN_BAS]->val == SOL)) || (existe(c->tabVoisin[VOISIN_HAUT]) && (c->tabVoisin[VOISIN_HAUT]->val == SOL)));
    } else if (changementLigne == 0 && changementColonne == -1) {
        return ((existe(c->tabVoisin[VOISIN_GAUCHE]) && (c->tabVoisin[VOISIN_GAUCHE]->val == SOL)) || (existe(c->tabVoisin[VOISIN_BAS]) && (c->tabVoisin[VOISIN_BAS]->val == SOL)) || (existe(c->tabVoisin[VOISIN_HAUT]) && (c->tabVoisin[VOISIN_HAUT]->val == SOL)));
    } else if (changementLigne == 1 && changementColonne == 0) {
        return ((existe(c->tabVoisin[VOISIN_GAUCHE]) && (c->tabVoisin[VOISIN_GAUCHE]->val == SOL)) || (existe(c->tabVoisin[VOISIN_DROIT]) && (c->tabVoisin[VOISIN_DROIT]->val == SOL)) || (existe(c->tabVoisin[VOISIN_BAS]) && (c->tabVoisin[VOISIN_BAS]->val == SOL)));
    } else {
        return ((existe(c->tabVoisin[VOISIN_GAUCHE]) && (c->tabVoisin[VOISIN_GAUCHE]->val == SOL)) || (existe(c->tabVoisin[VOISIN_DROIT]) && (c->tabVoisin[VOISIN_DROIT]->val == SOL)) || (existe(c->tabVoisin[VOISIN_HAUT]) && (c->tabVoisin[VOISIN_HAUT]->val == SOL)));
    }
}

SDL_bool trouerGrille(t_grille **grille, int xdebut, int ydebut, int changementLigne, int changementColonne, t_listeBlock *lb) {
    if (!grille) return SDL_FALSE;  // Vérification de la grille

    int i = xdebut, j = ydebut;
    if (changementLigne == 0 && changementColonne == 1) {
        copierVal(blockByName(lb, DIRECTION_GAUCHE), &(*grille)->grille[i][j]->tiles);
    } else if (changementLigne == 0 && changementColonne == -1) {
        copierVal(blockByName(lb, DIRECTION_DROITE), &(*grille)->grille[i][j]->tiles);
    } else if (changementLigne == 1 && changementColonne == 0) {
        copierVal(blockByName(lb, DIRECTION_BAS), &(*grille)->grille[i][j]->tiles);
    } else {
        copierVal(blockByName(lb, DIRECTION_HAUT), &(*grille)->grille[i][j]->tiles);
    }
    (*grille)->grille[i][j]->val = SOL;

    // Creuser jusqu'à une case ouverte ou jusqu'à sortir des limites
    while (i >= 0 && i < (*grille)->nbLigne && j >= 0 && j < (*grille)->nbColonne && !estOuvert((*grille)->grille[i][j], changementLigne, changementColonne)) {
        i += changementLigne;
        j += changementColonne;
        if (i >= 0 && i < (*grille)->nbLigne && j >= 0 && j < (*grille)->nbColonne) {
            (*grille)->grille[i][j]->val = SOL;
        }
    }

    if (i < 0 || i > (*grille)->nbLigne || j < 0 || j > (*grille)->nbColonne) {
        for (i = xdebut, j = ydebut; i >= 0 && i < (*grille)->nbLigne && j >= 0 && j < (*grille)->nbColonne; i += changementLigne, j += changementColonne) {
            (*grille)->grille[i][j]->val = OBSTACLE;
        }
        return SDL_FALSE;
    }
    if ((*grille)->grille[i + changementLigne][j + changementColonne]->val == OBSTACLE) {
        (*grille)->grille[i + changementLigne][j + changementColonne]->val = SOL;
    }

    return SDL_TRUE;
}

void placerSortie(t_grille **grille, t_salle *salle, t_listeBlock **lab) {
    t_listeBlock *lb = listeByType(lab, DIRECTION_TYPE);
    if (salle->droite != NULL) {
        while (!(trouerGrille(grille, (rand() % ((*grille)->nbLigne - 3) + 1), (*grille)->nbColonne - 1, 0, -1, lb)));
    }
    if (salle->gauche != NULL) {
        while (!(trouerGrille(grille, (rand() % ((*grille)->nbLigne - 3) + 1), 0, 0, 1, lb)));
    }
    if (salle->haut != NULL) {
        while (!(trouerGrille(grille, 0, (rand() % ((*grille)->nbColonne - 3) + 1), 1, 0, lb)));
    }
    if (salle->bas != NULL) {
        while (!(trouerGrille(grille, (*grille)->nbLigne - 1, (rand() % ((*grille)->nbColonne - 2) + 1), -1, 0, lb)));
    }
}
t_grille *geneRoom(t_salle *salle) {
    // srand(time(NULL));

    int nbLigne = (rand() % 70 + 30) & ~1;
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
    t_grille *grille = intToGrilleNiveau(entier, nbLigne, nbColonne);

    placerSortie(&grille, salle, listAllBlock);
    lissage(grille);

    choixTiles(listAllBlock, grille);

    addComplement(grille, listAllBlock);

    saveMap(grille);

    freeMatriceInt(entier, nbLigne, nbColonne);
    freeMatriceFloat(values, nbLigne, nbColonne);
    freeListeBlock(listAllBlock);

    return grille;
}
