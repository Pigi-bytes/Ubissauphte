
#include "general.h"
#include "pileRoom.h"

#define TRUE 1
#define FALSE 0

void float_to_int(float mat[HEIGHT][WIDTH], int entier[HEIGHT][WIDTH]);
void afficheMat(int mat[HEIGHT][WIDTH]);
int sans_0(int mat[HEIGHT][WIDTH]);
int nb_elem_block(int mat[HEIGHT][WIDTH], int num_Block);
void fill_gaps(int mat[HEIGHT][WIDTH]);
void comblet(int mat[HEIGHT][WIDTH], int copie[HEIGHT][WIDTH], int num_Block);
