#include "general.h"

int case_solo(int i, int j, int mat[HEIGHT][WIDTH]);
int verif_voisin_sol(int i, int j, int mat[HEIGHT][WIDTH]);
int verif_voisin_plafond(int i, int j, int mat[HEIGHT][WIDTH]);
int ajout_arriere(int i, int j, int mat[HEIGHT][WIDTH]);
int probleme_diag_bas_vers_haut(int i, int j, int mat[HEIGHT][WIDTH]);
int probleme_diag_coin(int i, int j, int mat[HEIGHT][WIDTH]);
int probleme_diag_haut_vers_bas(int i, int j, int mat[HEIGHT][WIDTH]);
void lissage(int mat[HEIGHT][WIDTH]);