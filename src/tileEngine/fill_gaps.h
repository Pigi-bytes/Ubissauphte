
#include "general.h"
#include "pileRoom.h"

void float_to_int(float mat[HEIGHT][WIDTH], int entier[HEIGHT][WIDTH]);
void afficheMat(int mat[HEIGHT][WIDTH]);
int min(int mat[HEIGHT][WIDTH]);
int max(int mat[HEIGHT][WIDTH]);
int count(int mat[HEIGHT][WIDTH], int nb);
void propagadtion(int mat[HEIGHT][WIDTH]);
void miseA0(int mat[HEIGHT][WIDTH]);
void fillGaps(int mat[HEIGHT][WIDTH]);
