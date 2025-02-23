#include "lissage.h"

int case_solo(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verif_voisin_sol(i + 1, j, mat) && verif_voisin_sol(i - 1, j, mat) && verif_voisin_sol(i, j + 1, mat) && verif_voisin_sol(i, j - 1, mat);
}

int verif_voisin_sol(int i, int j, int mat[HEIGHT][WIDTH]) {
    return inMat(i, j) && !mat[i][j];
}
int verif_voisin_plafond(int i, int j, int mat[HEIGHT][WIDTH]) {
    return inMat(i, j) && mat[i][j];
}
int ajout_arriere(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verif_voisin_plafond(i + 1, j, mat) && verif_voisin_sol(i + 2, j, mat) && verif_voisin_sol(i - 1, j, mat);
}

int probleme_diag_bas_vers_haut(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verif_voisin_sol(i + 1, j - 1, mat) && verif_voisin_sol(i - 1, j + 1, mat) && (verif_voisin_plafond(i, j + 1, mat) || verif_voisin_plafond(i, j - 1, mat));
}

int probleme_diag_coin(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verif_voisin_sol(i - 1, j + 1, mat) && verif_voisin_sol(i + 2, j - 1, mat) && (verif_voisin_plafond(i, j + 1, mat) || verif_voisin_plafond(i, j - 1, mat));
}

int probleme_diag_haut_vers_bas(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verif_voisin_sol(i - 1, j - 1, mat) && verif_voisin_sol(i + 2, j + 1, mat) && (verif_voisin_plafond(i, j + 1, mat) && verif_voisin_plafond(i, j - 1, mat));
}

void lissage(int mat[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] == 1 && !(case_solo(i, j, mat))) {
                if (((verif_voisin_sol(i + 1, j, mat) && verif_voisin_sol(i - 1, j, mat)) || (verif_voisin_sol(i, j + 1, mat) && verif_voisin_sol(i, j - 1, mat)))) {
                    mat[i][j] = 0;
                } else if (ajout_arriere(i, j, mat)) {
                    mat[i][j] = 0;
                    mat[i + 1][j] = 0;
                } else if (probleme_diag_bas_vers_haut(i, j, mat)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMat(i + 2, j - 1)) mat[i + 2][j - 1] = 1;
                } else if (probleme_diag_coin(i, j, mat)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMat(i + 2, j - 1)) mat[i + 2][j - 1] = 1;
                } else if (probleme_diag_haut_vers_bas(i, j, mat)) {
                    if (inMat(i + 2, j - +1)) mat[i + 2][j + 1] = 1;
                }
            }
        }
    }
}
