#ifndef GENERAL_H
#define GENERAL_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 50
#define WIDTH 40

#define window_width 1280
#define window_height 960

#define VOISIN_GAUCHE 0
#define VOISIN_DROIT 1
#define VOISIN_BAS 2
#define VOISIN_HAUT 3
#define VOISIN_DIAGGAUCHE 4
#define VOISIN_DIAGDROIT 5
#define VOISIN_DIAGBAS 6
#define VOISIN_DIAGHAUT 7

typedef struct {
    char* index;
    char* rota;
    float proba;
} t_tile;

typedef struct s_caseMatrice {
    int valeur;
    int x, y;
    struct s_caseMatrice* voisins[8];
} t_caseMatrice;

int inMat(int x, int y);

#endif