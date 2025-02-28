#include "../debug.h"
#include "../io/input.h"
#include "fill_gaps.h"
#include "general.h"
#include "grille.h"
#include "lissage.h"
#include "liste_block.h"
#include "perlinNoise.h"

t_block *plafond(t_listeBlock **lab);
t_block *sol(t_listeBlock **lab);
t_block *deco(t_listeBlock **lab);
t_block *frontale(t_listeBlock **lab);

//regles pour le choix du tiles trié par priorité
int mur_avant(t_case *c);
int angle_droit(t_case *c);
int angle_gauche(t_case *c);
int arrondi_inf_droit(t_case *c);
int arrondi_inf_gauche(t_case *c);
int arrondi_sup_droit(t_case *c);
int arrondi_sup_gauche(t_case *c);
int mur_arriere(t_case *c);
int angle_continue_gauche(t_case *c);
int angle_continue_droit(t_case *c);
int bordure_mur_avant(t_case *c);
int bordure_mur_gauche(t_case *c);
int bordure_mur_droit(t_case *c);

//fonctions de genération de la map
void choixTiles(t_listeBlock **lab, t_grille *g);
void load(t_grille *g);