#include "fillGaps.h"

void initPile() {
    Tete = NULL;
}

int PileVide() {
    return Tete == NULL;
}

void empiler(SDL_Point *p) {
    t_element *elt = malloc(sizeof(t_element));
    elt->precedent = Tete;
    elt->point = p;
    Tete = elt;
}

SDL_Point *depiler() {
    if (PileVide()) {
        return NULL;
    }
    t_element *sauv = Tete;
    SDL_Point *p = Tete->point;
    Tete = Tete->precedent;
    free(sauv);
    return p;
}

void viderPile() {
    while ((!PileVide())) {
        depiler();
    }
}

float ** createMatriceFloat(int nbLigne,int nbColonne){
    float **mat = (float **)malloc(sizeof(float *) * nbLigne);
    for (int i = 0; i < nbLigne; i++) {
        mat[i] = malloc(sizeof(float) * nbColonne);
    }
    return mat;
}

int **matriceFloat2Int(float **mat, int nbLigne,int nbColonne) {
    int **entier = (int **)malloc(sizeof(int *) * nbLigne);
    for (int i = 0; i < nbLigne; i++) {
        entier[i] = malloc(sizeof(int) * nbColonne);
    }
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (i == 0 || j == 0 || i == nbLigne - 1 || j == nbColonne - 1) {
                entier[i][j] = 1;
            } else
                entier[i][j] = mat[i][j] > 0.3 ? 0 : 1;
        }
    }
    return entier;
}

void freeMatriceFloat(float **mat, int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        free(mat[i]);
    }
    free(mat);
}

void freeMatriceInt(int **mat, int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        free(mat[i]);
    }
    free(mat);
}


SDL_bool matriceSans0(int ** mat ,int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (mat[i][j] == 0) return SDL_FALSE;
        }
    }
    return SDL_TRUE;
}

int nombreElementsInBlock(int ** mat ,int nbLigne, int nbColonne, int num_Block) {
    int count = 0;
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (mat[i][j] == num_Block) count++;
        }
    }
    return count;
}

void comblerTrou(int ** mat ,int nbLigne, int nbColonne, int ** copie, int num_Block) {
    int nbElemMax = 0;
    int num_BlockMax = 1;
    for (int block = 1; block < num_Block; block++) {
        int nbElem = nombreElementsInBlock(copie,nbLigne,nbColonne, block);
        if (nbElem > nbElemMax) {
            nbElemMax = nbElem;
            num_BlockMax = block;
        }
    }

    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (copie[i][j] != num_BlockMax)
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
        }
    }
}

void rendreMatriceContinue(int ** mat ,int nbLigne, int nbColonne) {
    initPile();

    int **copie = (int **)malloc(sizeof(int *)*nbLigne);
    for (int i = 0; i < nbLigne; i++) {
        copie[i] = malloc(sizeof(int) * nbColonne);
    }
    int numBlock = 1;
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            copie[i][j] = (mat[i][j] == 1) ? -1 : 0;
        }
    }

    while (!matriceSans0(copie,nbLigne,nbColonne)) {
        int x = -1, y = -1;
        for (int i = 0; i < nbLigne && x == -1; i++) {
            for (int j = 0; j < nbColonne && y == -1; j++) {
                if (copie[i][j] == 0) {
                    x = i;
                    y = j;
                }
            }
        }

        SDL_Point *start = malloc(sizeof(SDL_Point));
        start->x = x;
        start->y = y;
        empiler(start);

        while (!PileVide() && x!=-1 && y!=-1) {
            SDL_Point *elt = depiler();
            if (!elt) continue;

            int px = elt->x, py = elt->y;
            free(elt);

            if (copie[px][py] != 0) continue;
            copie[px][py] = numBlock;

            int dx[] = {1, -1, 0, 0};
            int dy[] = {0, 0, 1, -1};
            for (int i = 0; i < 4; i++) {
                int nx = px + dx[i], ny = py + dy[i];
                if (inMatrice(nx, ny,nbLigne,nbColonne) && mat[nx][ny] == 0 && copie[nx][ny] == 0) {
                    SDL_Point *np = malloc(sizeof(SDL_Point));
                    np->x = nx;
                    np->y = ny;
                    empiler(np);
                }
            }
        }
        numBlock++;
    }
    comblerTrou(mat,nbLigne,nbColonne, copie, numBlock);
    freeMatriceInt(copie, nbLigne, nbColonne);
}
