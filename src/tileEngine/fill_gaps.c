#include "fill_gaps.h"

void float_to_int(float mat[HEIGHT][WIDTH], int entier[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1) {
                entier[i][j] = 1;
            } else
                entier[i][j] = mat[i][j] > 0.3 ? 0 : 1;
        }
    }
}

void afficheMat(int mat[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
int inMat(int x, int y) {
    return x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH;
}

int min(int mat[HEIGHT][WIDTH]) {
    int min = 100;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] < min && mat[i][j] != -1) min = mat[i][j];
        }
    }
    return min;
}

int max(int mat[HEIGHT][WIDTH]) {
    int max = 1;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] > max) max = mat[i][j];
        }
    }
    return max;
}

int count(int mat[HEIGHT][WIDTH], int nb) {
    int cmpt = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] == nb) cmpt++;
        }
    }
    return cmpt;
}

void applatir(int mat[HEIGHT][WIDTH]) {
    if (max(mat) != min(mat)) {
        int mini = min(mat);
        int maxi = max(mat);
        int Countmini = count(mat, min(mat));
        int Countmax = count(mat, max(mat));
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (Countmini > Countmax) {
                    if (mat[i][j] == maxi) mat[i][j] = -1;
                } else {
                    if (mat[i][j] == mini)
                        mat[i][j] = -1;
                }
            }
        }
    }
}

void propagadtion(int mat[HEIGHT][WIDTH]) {
    int maxi = max(mat);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] > 0) mat[i][j] = maxi;
        }
    }
}

void miseA0(int mat[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mat[i][j] != -1) mat[i][j] = 0;
        }
    }
}

void fillGaps(int mat[HEIGHT][WIDTH]) {
    initPile();
    int copie[HEIGHT][WIDTH];

    // Initialisation de copie avec -1 pour les murs et 0 pour le reste
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            copie[i][j] = (mat[i][j] == 1) ? -1 : 0;
        }
    }
    // Trouver un point de départ (premier 0 trouvé)

    while (min(copie) != max(copie)) {
        miseA0(copie);
        int x = -1, y = -1;
        for (int i = 0; i < HEIGHT && x == -1; i++) {
            for (int j = 0; j < WIDTH && y == -1; j++) {
                if (copie[i][j] == 0) {
                    x = i;
                    y = j;
                }
            }
        }

        // Si aucun 0 n'est trouvé, on arrête
        if (x == -1) return;

        // Démarrage du flood fill
        SDL_Point *start = malloc(sizeof(SDL_Point));
        start->x = x;
        start->y = y;
        empiler(start);

        int nb = 1;
        int not_enfermer = 0;

        while (!PileVide()) {
            SDL_Point *elt = depiler();
            if (!elt) continue;

            int px = elt->x, py = elt->y;
            free(elt);  // Libérer la mémoire de l'élément dépilé

            if (copie[px][py] != 0) continue;  // Déjà visité ou obstacle
            copie[px][py] = nb++;

            // Ajouter les voisins valides à la pile
            int dx[] = {1, -1, 0, 0};
            int dy[] = {0, 0, 1, -1};
            for (int i = 0; i < 4; i++) {
                int nx = px + dx[i], ny = py + dy[i];
                if (inMat(nx, ny) && mat[nx][ny] == 0 && copie[nx][ny] == 0) {
                    SDL_Point *np = malloc(sizeof(SDL_Point));
                    np->x = nx;
                    np->y = ny;
                    empiler(np);
                    not_enfermer = 1;
                }
            }
            if (!(not_enfermer)) copie[px][py] = -1;
        }
        if (not_enfermer) {
            propagadtion(copie);
            applatir(copie);
        }
    }

    // Copier les valeurs dans mat (remplace les 0 inaccessibles par 1)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (copie[i][j] == 0)
                mat[i][j] = 1;  // Zone inaccessible
            else if (copie[i][j] == -1)
                mat[i][j] = 1;
            else
                mat[i][j] = copie[i][j];  // Conserve le remplissage
        }
    }
}
