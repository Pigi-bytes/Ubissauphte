
#include "general.h"
#include "pileRoom.h"

#define TRUE 1
#define FALSE 0

float **matFloat(int nbLigne, int nbColonne);
int **float_to_int(float** mat,int nbLigne,int nbColonne);
void freeMatFloat(float **mat, int nbLigne, int nbColonne);
void freeMatInt(int **mat, int nbLigne, int nbColonne);
void afficheMat(int ** mat ,int nbLigne, int nbColonne);
int sans_0(int ** mat ,int nbLigne, int nbColonne);
int nb_elem_block(int ** mat ,int nbLigne, int nbColonne, int num_Block);
void fill_gaps(int ** mat ,int nbLigne, int nbColonne);
void comblet(int ** mat ,int nbLigne, int nbColonne, int ** copie, int num_Block);
