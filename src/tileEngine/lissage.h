#include "grille.h"
#include <SDL2/SDL.h>

SDL_bool caseSansVoisins(int i, int j, int** mat, int nbLigne, int nbColonne);
SDL_bool caseIsSol(int i, int j, int ** mat ,int nbLigne, int nbColonne);
SDL_bool caseIsPlafond(int i, int j, int** mat, int nbLigne, int nbColonne);
SDL_bool ajoutArriere(int i, int j, int** mat, int nbLigne, int nbColonne);
SDL_bool blockDiagHaut(int i, int j, int** mat, int nbLigne, int nbColonne);
SDL_bool blockDiagCoin(int i, int j, int** mat, int nbLigne, int nbColonne);
SDL_bool blockDiagBas(int i, int j, int** mat, int nbLigne, int nbColonne);
void lissage(int ** mat ,int nbLigne, int nbColonne);