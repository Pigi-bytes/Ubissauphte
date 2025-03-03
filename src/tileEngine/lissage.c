#include "lissage.h"

SDL_bool caseSansVoisins(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return caseIsSol(i + 1, j, mat,nbLigne,nbColonne) && caseIsSol(i - 1, j, mat,nbLigne,nbColonne) && caseIsSol(i, j + 1, mat,nbLigne,nbColonne) && caseIsSol(i, j - 1, mat,nbLigne,nbColonne);
}

SDL_bool caseIsSol(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return inMatrice(i, j,nbLigne,nbColonne) && !mat[i][j];
}
SDL_bool caseIsPlafond(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return inMatrice(i, j,nbLigne,nbColonne) && mat[i][j];
}
SDL_bool ajoutArriere(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return caseIsPlafond(i + 1, j, mat,nbLigne,nbColonne) && caseIsSol(i + 2, j, mat,nbLigne,nbColonne) && caseIsSol(i - 1, j, mat, nbLigne,nbColonne);
}

SDL_bool blockDiagHaut(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return caseIsSol(i + 1, j - 1, mat,nbLigne,nbColonne) && caseIsSol(i - 1, j + 1, mat,nbLigne,nbColonne) && (caseIsPlafond(i, j + 1, mat,nbLigne,nbColonne) || caseIsPlafond(i, j - 1, mat,nbLigne,nbColonne));
}

SDL_bool blockDiagCoin(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return caseIsSol(i - 1, j + 1, mat,nbLigne,nbColonne) && caseIsSol(i + 2, j - 1, mat,nbLigne,nbColonne) && (caseIsPlafond(i, j + 1, mat,nbLigne,nbColonne) || caseIsPlafond(i, j - 1, mat,nbLigne,nbColonne));
}

SDL_bool blockDiagBas(int i, int j, int** mat, int nbLigne, int nbColonne) {
    return caseIsSol(i - 1, j - 1, mat,nbLigne,nbColonne) && caseIsSol(i + 2, j + 1, mat,nbLigne,nbColonne) && (caseIsPlafond(i, j + 1, mat,nbLigne,nbColonne) && caseIsPlafond(i, j - 1, mat,nbLigne,nbColonne));
}

void lissage(int ** mat ,int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (mat[i][j] == 1 && !(caseSansVoisins(i, j, mat,nbLigne,nbColonne))) {
                if (((caseIsSol(i + 1, j, mat,nbLigne,nbColonne) && caseIsSol(i - 1, j, mat,nbLigne,nbColonne)) || (caseIsSol(i, j + 1, mat,nbLigne,nbColonne) && caseIsSol(i, j - 1, mat,nbLigne,nbColonne)))) {
                    mat[i][j] = 0;
                } else if (ajoutArriere(i, j, mat,nbLigne,nbColonne)) {
                    mat[i][j] = 0;
                    mat[i + 1][j] = 0;
                } else if (blockDiagHaut(i, j, mat,nbLigne,nbColonne)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMatrice(i + 2, j - 1,nbLigne,nbColonne)) mat[i + 2][j - 1] = 1;
                } else if (blockDiagCoin(i, j, mat,nbLigne,nbColonne)) {
                    mat[i + 1][j - 1] = 1;
                    if (inMatrice(i + 2, j - 1,nbLigne,nbColonne)) mat[i + 2][j - 1] = 1;
                } else if (blockDiagBas(i, j, mat,nbLigne,nbColonne)) {
                    if (inMatrice(i + 2, j +1,nbLigne,nbColonne)) mat[i + 2][j + 1] = 1;
                }
            }
        }
    }
}
