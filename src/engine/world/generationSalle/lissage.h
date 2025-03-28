#ifndef LISSAGE_H
#define LISSAGE_H

#include <SDL2/SDL.h>

#include "grille.h"

SDL_bool caseSansVoisins(t_case* c);
SDL_bool caseIsSol(t_case* c);
SDL_bool caseIsPlafond(t_case* c);

// fonctions de régles
SDL_bool isIsolatedCase(t_case* c);
SDL_bool isHorizontalOrVerticalPath(t_case* c);
SDL_bool shouldAddBackWall(t_case* c);
SDL_bool isDiagonalTopPattern(t_case* c);
SDL_bool isDiagonalCornerPattern(t_case* c);
SDL_bool isDiagonalBottomPattern(t_case* c);

// fonction d'affectation de la case
void handleDiagonalTopPattern(t_grille* grille, int i, int j);
void handleDiagonalCornerPattern(t_grille* grille, int i, int j);
void handleDiagonalBottomPattern(t_grille* grille, int i, int j);

void lissage(t_grille* grille);

#endif