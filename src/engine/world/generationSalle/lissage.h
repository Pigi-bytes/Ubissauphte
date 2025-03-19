#ifndef LISSAGE_H
#define LISSAGE_H

#include <SDL2/SDL.h>

#include "grille.h"

SDL_bool caseSansVoisins(t_case* c);
SDL_bool caseIsSol(t_case* c);
SDL_bool caseIsPlafond(t_case* c);
SDL_bool ajoutArriere(t_case* c);
SDL_bool blockDiagHaut(t_case* c);
SDL_bool blockDiagCoin(t_case* c);
SDL_bool blockDiagBas(t_case* c);
void lissage(t_grille* grille);

#endif