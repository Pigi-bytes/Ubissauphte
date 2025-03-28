#include "lissage.h"

SDL_bool caseSansVoisins(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_BAS]) && caseIsSol(c->tabVoisin[VOISIN_HAUT]) && caseIsSol(c->tabVoisin[VOISIN_DROIT]) && caseIsSol(c->tabVoisin[VOISIN_GAUCHE]);
}

SDL_bool caseIsSol(t_case* c) {
    return existe(c) && c->val != OBSTACLE;
}
SDL_bool caseIsPlafond(t_case* c) {
    return existe(c) && (c->val != SOL);
}

SDL_bool isIsolatedCase(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_BAS]) &&
           caseIsSol(c->tabVoisin[VOISIN_HAUT]) &&
           caseIsSol(c->tabVoisin[VOISIN_DROIT]) &&
           caseIsSol(c->tabVoisin[VOISIN_GAUCHE]);
}

SDL_bool isHorizontalOrVerticalPath(t_case* c) {
    return (caseIsSol(c->tabVoisin[VOISIN_DROIT]) && caseIsSol(c->tabVoisin[VOISIN_GAUCHE])) ||
           (caseIsSol(c->tabVoisin[VOISIN_HAUT]) && caseIsSol(c->tabVoisin[VOISIN_BAS]));
}

SDL_bool shouldAddBackWall(t_case* c) {
    return caseIsPlafond(c->tabVoisin[VOISIN_BAS]) &&
           caseIsSol(c->tabVoisin[VOISIN_BAS2]) &&
           caseIsSol(c->tabVoisin[VOISIN_HAUT]);
}

SDL_bool isDiagonalTopPattern(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]) &&
           caseIsSol(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]) &&
           (caseIsPlafond(c->tabVoisin[VOISIN_DROIT]) || caseIsPlafond(c->tabVoisin[VOISIN_GAUCHE]));
}

SDL_bool isDiagonalCornerPattern(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]) &&
           caseIsSol(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) &&
           (caseIsPlafond(c->tabVoisin[VOISIN_DROIT]) || caseIsPlafond(c->tabVoisin[VOISIN_GAUCHE]));
}

static SDL_bool isDiagonalBottomPattern(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]) &&
           caseIsSol(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) &&
           caseIsPlafond(c->tabVoisin[VOISIN_DROIT]) &&
           caseIsPlafond(c->tabVoisin[VOISIN_GAUCHE]);
}

static void handleDiagonalTopPattern(t_grille* grille, int i, int j) {
    grille->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val = OBSTACLE;
    if (existe(grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE])) {
        grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val = OBSTACLE;
    }
}

static void handleDiagonalCornerPattern(t_grille* grille, int i, int j) {
    grille->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val = OBSTACLE;
    if (existe(grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE])) {
        grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val = OBSTACLE;
    }
}

static void handleDiagonalBottomPattern(t_grille* grille, int i, int j) {
    if (existe(grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_DROIT])) {
        grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val = OBSTACLE;
    }
}

void lissage(t_grille* grille) {
    for (int i = 0; i < grille->nbLigne; i++) {
        for (int j = 0; j < grille->nbColonne; j++) {
            t_case* current = grille->grille[i][j];

            if (current->val != OBSTACLE || isIsolatedCase(current)) {
                continue;
            }

            if (isHorizontalOrVerticalPath(current)) {
                current->val = SOL;
            } else if (shouldAddBackWall(current)) {
                current->val = SOL;
                current->tabVoisin[VOISIN_BAS]->val = SOL;
            } else if (isDiagonalTopPattern(current)) {
                handleDiagonalTopPattern(grille, i, j);
            } else if (isDiagonalCornerPattern(current)) {
                handleDiagonalCornerPattern(grille, i, j);
            } else if (isDiagonalBottomPattern(current)) {
                handleDiagonalBottomPattern(grille, i, j);
            }
        }
    }
}
