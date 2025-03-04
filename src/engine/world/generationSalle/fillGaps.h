#ifndef FILLGAPS_H
#define FILLGAPS_H

#include <SDL2/SDL.h>
#include "grille.h"

typedef struct s_element {
    SDL_Point* point;
    struct s_element* precedent;
} t_element;



//fonction pour les piles
void initPile();
int PileVide();
void empiler(SDL_Point* p);
SDL_Point* depiler();
void viderPile();

// fonction pour combler les trous
float **createMatriceFloat(int nbLigne, int nbColonne);
int **matriceFloat2Int(float** mat,int nbLigne,int nbColonne);
void freeMatriceFloat(float **mat, int nbLigne, int nbColonne);
void freeMatriceInt(int **mat, int nbLigne, int nbColonne);
SDL_bool matriceSans0(int **mat, int nbLigne, int nbColonne);
int nombreElementsInBlock(int ** mat ,int nbLigne, int nbColonne, int num_Block);
void rendreMatriceContinue(int ** mat ,int nbLigne, int nbColonne);
void comblerTrou(int ** mat ,int nbLigne, int nbColonne, int ** copie, int num_Block);

#endif