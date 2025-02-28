#include "lissage.h"

int case_solo(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return verif_voisin_sol(i + 1, j, mat,nbLigne,nbColonne) && verif_voisin_sol(i - 1, j, mat,nbLigne,nbColonne) && verif_voisin_sol(i, j + 1, mat,nbLigne,nbColonne) && verif_voisin_sol(i, j - 1, mat,nbLigne,nbColonne);
}

int verif_voisin_sol(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return inMat2(i, j,nbLigne,nbColonne) && !mat[i][j];
}
int verif_voisin_plafond(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return inMat2(i, j,nbLigne,nbColonne) && mat[i][j];
}
int ajout_arriere(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return verif_voisin_plafond(i + 1, j, mat,nbLigne,nbColonne) && verif_voisin_sol(i + 2, j, mat,nbLigne,nbColonne) && verif_voisin_sol(i - 1, j, mat, nbLigne,nbColonne);
}

int probleme_diag_bas_vers_haut(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return verif_voisin_sol(i + 1, j - 1, mat,nbLigne,nbColonne) && verif_voisin_sol(i - 1, j + 1, mat,nbLigne,nbColonne) && (verif_voisin_plafond(i, j + 1, mat,nbLigne,nbColonne) || verif_voisin_plafond(i, j - 1, mat,nbLigne,nbColonne));
}

int probleme_diag_coin(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return verif_voisin_sol(i - 1, j + 1, mat,nbLigne,nbColonne) && verif_voisin_sol(i + 2, j - 1, mat,nbLigne,nbColonne) && (verif_voisin_plafond(i, j + 1, mat,nbLigne,nbColonne) || verif_voisin_plafond(i, j - 1, mat,nbLigne,nbColonne));
}

int probleme_diag_haut_vers_bas(int i, int j, int ** mat ,int nbLigne, int nbColonne) {
    return verif_voisin_sol(i - 1, j - 1, mat,nbLigne,nbColonne) && verif_voisin_sol(i + 2, j + 1, mat,nbLigne,nbColonne) && (verif_voisin_plafond(i, j + 1, mat,nbLigne,nbColonne) && verif_voisin_plafond(i, j - 1, mat,nbLigne,nbColonne));
}

void lissage(int ** mat ,int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (mat[i][j] == 1 && !(case_solo(i, j, mat,nbLigne,nbColonne))) {
                if (((verif_voisin_sol(i + 1, j, mat,nbLigne,nbColonne) && verif_voisin_sol(i - 1, j, mat,nbLigne,nbColonne)) || (verif_voisin_sol(i, j + 1, mat,nbLigne,nbColonne) && verif_voisin_sol(i, j - 1, mat,nbLigne,nbColonne)))) {
                    mat[i][j] = 0;
                } else if (ajout_arriere(i, j, mat,nbLigne,nbColonne)) {
                    mat[i][j] = 0;
                    mat[i + 1][j] = 0;
                } else if (probleme_diag_bas_vers_haut(i, j, mat,nbLigne,nbColonne)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMat2(i + 2, j - 1,nbLigne,nbColonne)) mat[i + 2][j - 1] = 1;
                } else if (probleme_diag_coin(i, j, mat,nbLigne,nbColonne)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMat2(i + 2, j - 1,nbLigne,nbColonne)) mat[i + 2][j - 1] = 1;
                } else if (probleme_diag_haut_vers_bas(i, j, mat,nbLigne,nbColonne)) {
                    if (inMat2(i + 2, j +1,nbLigne,nbColonne)) mat[i + 2][j + 1] = 1;
                }
            }
        }
    }
}
