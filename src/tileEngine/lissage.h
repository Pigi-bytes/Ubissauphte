#include "general.h"

int caseSolo(int i, int j, int mat[HEIGHT][WIDTH]);
int verifVoisinSol(int i, int j, int mat[HEIGHT][WIDTH]);
int verifVoisinPlafond(int i, int j, int mat[HEIGHT][WIDTH]);
int ajoutArriere(int i, int j, int mat[HEIGHT][WIDTH]);
int probleme_diag_bas_vers_haut(int i, int j, int mat[HEIGHT][WIDTH]);
int probleme_diag_coin(int i, int j, int mat[HEIGHT][WIDTH]);
int probleme_diag_haut_vers_bas(int i, int j, int mat[HEIGHT][WIDTH]);
void lissage(int mat[HEIGHT][WIDTH]);