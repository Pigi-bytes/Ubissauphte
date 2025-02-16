#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 50
#define M 50

#define carac_ind 2
#define carac_vivant 1
#define carac_mort 0

#define DENSITE 0.3

int in_Room(int x, int y);
void affiche_Matriceint(int mat[N][M]);
void init_Matrice(int mat[N][M]);
void complete(int mat[N][M]);
void load(int mat[N][N]);
void creeMap();