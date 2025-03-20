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
SDL_bool ajoutArriere(t_case* c) {
    return caseIsPlafond(c->tabVoisin[VOISIN_BAS]) && caseIsSol(c->tabVoisin[VOISIN_BAS2]) && caseIsSol(c->tabVoisin[VOISIN_HAUT]);
}

SDL_bool blockDiagHaut(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]) && caseIsSol(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]) && (caseIsPlafond(c->tabVoisin[VOISIN_DROIT]) || caseIsPlafond(c->tabVoisin[VOISIN_GAUCHE]));
}

SDL_bool blockDiagCoin(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]) && caseIsSol(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) && (caseIsPlafond(c->tabVoisin[VOISIN_DROIT]) || caseIsPlafond(c->tabVoisin[VOISIN_GAUCHE]));
}

SDL_bool blockDiagBas(t_case* c) {
    return caseIsSol(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]) && caseIsSol(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) && (caseIsPlafond(c->tabVoisin[VOISIN_DROIT]) && caseIsPlafond(c->tabVoisin[VOISIN_GAUCHE]));
}

void lissage(t_grille* grille) {
    for (int i = 0; i < grille->nbLigne; i++) {
        for (int j = 0; j < grille->nbColonne; j++) {
            if (grille->grille[i][j]->val == OBSTACLE && !(caseSansVoisins(grille->grille[i][j]))) {
                if (((caseIsSol(grille->grille[i][j]->tabVoisin[VOISIN_DROIT]) && caseIsSol(grille->grille[i][j]->tabVoisin[VOISIN_GAUCHE])) || (caseIsSol(grille->grille[i][j]->tabVoisin[VOISIN_HAUT]) && caseIsSol(grille->grille[i][j]->tabVoisin[VOISIN_BAS])))) {
                    grille->grille[i][j]->val = SOL;
                } else if (ajoutArriere(grille->grille[i][j])) {
                    grille->grille[i][j]->val = SOL;
                    grille->grille[i][j]->tabVoisin[VOISIN_BAS]->val = SOL;
                } else if (blockDiagHaut(grille->grille[i][j])) {
                    grille->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val = OBSTACLE;
                    if (existe(grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]))
                        grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val = OBSTACLE;
                } else if (blockDiagCoin(grille->grille[i][j])) {
                    grille->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val = OBSTACLE;
                    if (existe(grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]))
                        grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val = OBSTACLE;
                } else if (blockDiagBas(grille->grille[i][j])) {
                    if (existe(grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]))
                        grille->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val = OBSTACLE;
                }
            }
        }
    }
}
