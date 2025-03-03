//#include "../debug.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../io/input.h"
#include "fillGaps.h"
#include "grille.h"
#include "lissage.h"
#include "liste_block.h"
#include "perlinNoise.h"

t_block *getPlafond(t_listeBlock **listAllBlock);
t_block *getSol(t_listeBlock **listAllBlock);
t_block *getDeco(t_listeBlock **listAllBlock);
t_block *getFrontale(t_listeBlock **listAllBlock);

//regles pour le choix du tiles trié par priorité
SDL_bool murAvant(t_case *c);
SDL_bool angleDroit(t_case *c);
SDL_bool angleGauche(t_case *c);
SDL_bool arrondiInferieurDroit(t_case *c);
SDL_bool arrondiInferieurGauche(t_case *c);
SDL_bool arrondiSuperieurDroit(t_case *c);
SDL_bool arrondiSuperieurGauche(t_case *c);
SDL_bool murArriere(t_case *c);
SDL_bool angleContinueGauche(t_case *c);
SDL_bool angleContinueDroit(t_case *c);
SDL_bool bordureMurAvant(t_case *c);
SDL_bool bordureMurGauche(t_case *c);
SDL_bool bordureMurDroit(t_case *c);

//fonctions de genération de la map
void choixTiles(t_listeBlock **listAllBlock, t_grille *g);
void saveMap(t_grille *g);