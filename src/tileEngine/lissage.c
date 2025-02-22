#include "lissage.h"

int caseSolo(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verifVoisinSol(i + 1, j, mat) && verifVoisinSol(i - 1, j, mat) && verifVoisinSol(i, j + 1, mat) && verifVoisinSol(i, j - 1, mat);
}

int verifVoisinSol(int i, int j, int mat[HEIGHT][WIDTH]) {
    return inMat(i, j) && !mat[i][j];
}
int verifVoisinPlafond(int i, int j, int mat[HEIGHT][WIDTH]) {
    return inMat(i, j) && mat[i][j];
}
int ajoutArriere(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verifVoisinPlafond(i + 1, j, mat) && verifVoisinSol(i + 2, j, mat) && verifVoisinSol(i - 1, j, mat);
}

int probleme_diag_bas_vers_haut(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verifVoisinSol(i + 1, j - 1, mat) && verifVoisinSol(i - 1, j + 1, mat) && (verifVoisinPlafond(i, j + 1, mat) || verifVoisinPlafond(i, j - 1, mat));
}

int supressionLateral2(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verifVoisinSol(i - 1, j + 1, mat) && verifVoisinSol(i + 2, j - 1, mat) && (verifVoisinPlafond(i, j + 1, mat) || verifVoisinPlafond(i, j - 1, mat));
}

int probleme_diag_haut_vers_bas(int i, int j, int mat[HEIGHT][WIDTH]) {
    return verifVoisinSol(i - 1, j - 1, mat) && verifVoisinSol(i + 2, j + 1, mat) && (verifVoisinPlafond(i, j + 1, mat) && verifVoisinPlafond(i, j - 1, mat));
}

void lissage(int mat[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] == 1 && !(caseSolo(i, j, mat))) {
                if (((verifVoisinSol(i + 1, j, mat) && verifVoisinSol(i - 1, j, mat)) || (verifVoisinSol(i, j + 1, mat) && verifVoisinSol(i, j - 1, mat)))) {
                    mat[i][j] = 0;
                } else if (ajoutArriere(i, j, mat)) {
                    mat[i][j] = 0;
                    mat[i + 1][j] = 0;
                } else if (probleme_diag_bas_vers_haut(i, j, mat)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMat(i + 2, j - 1)) mat[i + 2][j - 1] = 1;
                } else if (supressionLateral2(i, j, mat)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMat(i + 2, j - 1)) mat[i + 2][j - 1] = 1;
                } else if (probleme_diag_haut_vers_bas(i, j, mat)) {
                    if (inMat(i + 2, j - +1)) mat[i + 2][j + 1] = 1;
                }
            }
        }
    }
}
