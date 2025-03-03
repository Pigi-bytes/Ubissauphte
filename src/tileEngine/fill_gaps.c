#include "fill_gaps.h"
float ** matFloat(int nbLigne,int nbColonne){
    float **mat = (float **)malloc(sizeof(float *) * nbLigne);
    for (int i = 0; i < nbLigne; i++) {
        mat[i] = malloc(sizeof(float) * nbColonne);
    }
    return mat;
}

int **float_to_int(float **mat, int nbLigne,int nbColonne) {
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

void freeMatFloat(float **mat, int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        free(mat[i]);
    }
    free(mat);
}

void freeMatInt(int **mat, int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        free(mat[i]);
    }
    free(mat);
}

void afficheMat(int ** mat ,int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            printf("%2d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int sans_0(int ** mat ,int nbLigne, int nbColonne) {
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (mat[i][j] == 0) return FALSE;
        }
    }
    return TRUE;
}

int nb_elem_block(int ** mat ,int nbLigne, int nbColonne, int num_Block) {
    int count = 0;
    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            if (mat[i][j] == num_Block) count++;
        }
    }
    return count;
}

void comblet(int ** mat ,int nbLigne, int nbColonne, int ** copie, int num_Block) {
    int nbElemMax = 0;
    int num_BlockMax = 1;
    for (int block = 1; block < num_Block; block++) {
        int nbElem = nb_elem_block(copie,nbLigne,nbColonne, block);
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

void fill_gaps(int ** mat ,int nbLigne, int nbColonne) {
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

    while (!sans_0(copie,nbLigne,nbColonne)) {
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
                if (inMat2(nx, ny,nbLigne,nbColonne) && mat[nx][ny] == 0 && copie[nx][ny] == 0) {
                    SDL_Point *np = malloc(sizeof(SDL_Point));
                    np->x = nx;
                    np->y = ny;
                    empiler(np);
                }
            }
        }
        numBlock++;
    }
    comblet(mat,nbLigne,nbColonne, copie, numBlock);
    freeMatInt(copie, nbLigne, nbColonne);
}
