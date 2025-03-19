#ifndef GRILLE_H
#define GRILLE_H

#include <stdarg.h>

#include "listeBlock.h"

#define VOISIN_GAUCHE 0
#define VOISIN_DROIT 1
#define VOISIN_BAS 2
#define VOISIN_HAUT 3
#define VOISIN_DIAG_GAUCHE_HAUT 4
#define VOISIN_DIAG_DROIT_HAUT 5
#define VOISIN_DIAG_GAUCHE_BAS 6
#define VOISIN_DIAG_DROIT_BAS 7

#define VOISIN_GAUCHE2 8
#define VOISIN_DROIT2 9
#define VOISIN_BAS2 10
#define VOISIN_HAUT2 11
#define VOISIN_DIAG_GAUCHE_HAUT2 12
#define VOISIN_DIAG_DROIT_HAUT2 13
#define VOISIN_DIAG_GAUCHE_BAS2 14
#define VOISIN_DIAG_DROIT_BAS2 15

#define VOISIN_CENTRE_BAS2_DROIT 16
#define VOISIN_CENTRE_BAS2_GAUCHE 17

#define OBSTACLE 1
#define SOL 0
#define ELTAJOUTE 2

typedef struct s_case {
    int i, j;
    t_block* tiles;
    int val;
    struct s_case* tabVoisin[18];
} t_case;

typedef struct s_grille {
    int nbLigne;
    int nbColonne;
    t_case*** grille;
    char nom[100];
} t_grille;

#ifndef CASE_NULL
#define CASE_NULL (&case_null)
#endif

#define EXISTE_VOISIN(c, ...) existeVoisin(c, ##__VA_ARGS__, NULL)

int inMatrice(int x, int y, int maxx, int maxy);
t_case* initCase(int i, int j, int val);
t_grille* initGrille(int nbLigne, int nbColonne);
void ajouterCase(t_grille* g, int i, int j, int val);
void freeGrille(t_grille* g);
void stockerEtatVoisin(t_grille* g, int i, int j, int nbLigne, int nbColonne);
t_grille* intToGrilleNiveau(int** entier, int nbLigne, int nbColonne);
int existe(const t_case* c);
int existeVoisin(t_case* c, ...);

#endif