#include "grille.h"

t_case case_null = {-1, -1, NULL, -1, {NULL}};

int inMatrice(int x, int y, int maxx, int maxy) {
    return (x >= 0 && x < maxx && y >= 0 && y < maxy);
}

t_case* initCase(int i, int j, int val) {
    t_case* c = malloc(sizeof(t_case));
    c->i = i;
    c->j = j;
    c->val = val;
    c->tiles = NULL;
    return c;
}

t_grille* initGrille(int nbLigne, int nbColonne) {
    t_grille* g = malloc(sizeof(t_grille));
    if (!g) {
        printf("erreur lors de l'allocationde la grille");
        exit(EXIT_FAILURE);
    }
    g->grille = (t_case***)malloc(sizeof(t_case**) * nbLigne);

    if (!(g->grille)) {
        printf("erreur lors de l'allocation des ligne de la grille");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nbLigne; i++) {
        g->grille[i] = (t_case**)malloc(sizeof(t_case*) * nbColonne);
        if (!(g->grille[i])) {
            printf("erreur lors de l'allocation de la colonne de la grille");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            g->grille[i][j] = NULL;
        }
    }
    g->nbLigne = nbLigne;
    g->nbColonne = nbColonne;
    return g;
}

void ajouterCase(t_grille* g, int i, int j, int val) {
    if (g->grille[i][j] == NULL) {
        g->grille[i][j] = initCase(i, j, val);
    }
}

void freeGrille(t_grille* g) {
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            free(g->grille[i][j]);
        }
        free(g->grille[i]);
    }
    free(g->grille);
    free(g);
}

void stockerEtatVoisin(t_grille* g, int i, int j, int nbLigne, int nbColonne) {
    g->grille[i][j]->tabVoisin[VOISIN_HAUT] = (inMatrice(i - 1, j, nbLigne, nbColonne)) ? g->grille[i - 1][j] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_HAUT2] = (inMatrice(i - 2, j, nbLigne, nbColonne)) ? g->grille[i - 2][j] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_BAS] = (inMatrice(i + 1, j, nbLigne, nbColonne)) ? g->grille[i + 1][j] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_BAS2] = (inMatrice(i + 2, j, nbLigne, nbColonne)) ? g->grille[i + 2][j] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_GAUCHE] = (inMatrice(i, j - 1, nbLigne, nbColonne)) ? g->grille[i][j - 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_GAUCHE2] = (inMatrice(i, j - 2, nbLigne, nbColonne)) ? g->grille[i][j - 2] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DROIT] = (inMatrice(i, j + 1, nbLigne, nbColonne)) ? g->grille[i][j + 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DROIT2] = (inMatrice(i, j + 2, nbLigne, nbColonne)) ? g->grille[i][j + 2] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_DROIT_BAS] = (inMatrice(i + 1, j + 1, nbLigne, nbColonne)) ? g->grille[i + 1][j + 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_DROIT_BAS2] = (inMatrice(i + 2, j + 2, nbLigne, nbColonne)) ? g->grille[i + 2][j + 2] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_DROIT] = (inMatrice(i + 2, j + 1, nbLigne, nbColonne)) ? g->grille[i + 2][j + 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE] = (inMatrice(i + 2, j - 1, nbLigne, nbColonne)) ? g->grille[i + 2][j - 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_DROIT_HAUT] = (inMatrice(i - 1, j + 1, nbLigne, nbColonne)) ? g->grille[i - 1][j + 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_DROIT_HAUT2] = (inMatrice(i - 2, j + 2, nbLigne, nbColonne)) ? g->grille[i - 2][j + 2] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_BAS] = (inMatrice(i + 1, j - 1, nbLigne, nbColonne)) ? g->grille[i + 1][j - 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_DROIT_BAS2] = (inMatrice(i + 2, j - 2, nbLigne, nbColonne)) ? g->grille[i + 2][j - 2] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT] = (inMatrice(i - 1, j - 1, nbLigne, nbColonne)) ? g->grille[i - 1][j - 1] : CASE_NULL;
    g->grille[i][j]->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT2] = (inMatrice(i - 2, j - 2, nbLigne, nbColonne)) ? g->grille[i - 2][j - 2] : CASE_NULL;
}

t_grille* intToGrilleNiveau(int** entier, int nbLigne, int nbColonne) {
    t_grille* g = initGrille(nbLigne, nbColonne);
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            g->grille[i][j] = initCase(i, j, entier[i][j]);
        }
    }

    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            stockerEtatVoisin(g, i, j, nbLigne, nbColonne);
        }
    }
    return g;
}

int existe(const t_case* c) {
    return ((c->i != -1) && (c->j !=-1) && (c->val != -1));
}

int existeVoisin(t_case* c, ...) {
    if (!existe(c)) {
        return 0;
    }
    va_list l;
    va_start(l, c);
    t_case* arg;
    while (arg = va_arg(l, t_case*)) {
        //printf("Argument reçu: %p\n", (void*)arg);
        if (!existe(arg)) {
            va_end(l);
            return 0;
        }
    }
    va_end(l);
    return 1;
}