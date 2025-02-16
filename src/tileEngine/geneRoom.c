#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../io/input.h"
#include "./pileRoom.h"

#define HEIGHT 50
#define WIDTH 40

#define window_width 1280
#define window_height 960

SDL_FPoint point_sin(SDL_FPoint a) {
    a.x = sin(a.x);
    a.y = sin(a.y);
    return a;
}

float dot(SDL_FPoint a, SDL_FPoint b) {
    return a.x * b.x + a.y * b.y;
}

float float_fract(float val) {
    return val - floor(val);
}

float random_float(SDL_FPoint st) {
    SDL_FPoint point = {12.9898, 78.233};
    return float_fract(sin(dot(st, point)) * 43758.5453123);
}

SDL_FPoint point_floor(SDL_FPoint st) {
    st.x = floor(st.x);
    st.y = floor(st.y);
    return st;
}

SDL_FPoint point_fract(SDL_FPoint st) {
    st.x = st.x - floor(st.x);
    st.y = st.y - floor(st.y);
    return st;
}

SDL_FPoint smooth_step(SDL_FPoint st) {
    st.x = st.x * st.x * (3.0 - 2.0 * st.x);
    st.y = st.y * st.y * (3.0 - 2.0 * st.y);
    return st;
}

float interpo(float a, float b, SDL_FPoint u) {
    return (a * (1 - u.x) + b * u.x);
}

float noise(SDL_FPoint st) {
    SDL_FPoint i = point_floor(st);
    SDL_FPoint f = point_fract(st);

    SDL_FPoint g = {i.x + 1.0, i.y};
    SDL_FPoint h = {i.x, i.y + 1.0};
    SDL_FPoint l = {i.x + 1, i.y + 1.0};

    float a = random_float(i);
    float b = random_float(g);
    float c = random_float(h);
    float d = random_float(l);

    SDL_FPoint u = smooth_step(f);

    return (interpo(a, b, u) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y);
}

void render_noise(SDL_Renderer *renderer, float values[HEIGHT][WIDTH]) {
    float cell_width = (float)window_width / WIDTH;
    float cell_height = (float)window_height / HEIGHT;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            float n = values[y][x];
            int color = (n * 255.0);  // si n est supérieur à 0.3 la couleur est noir sinon blanche plus c'est petit moins il y à de noir

            SDL_SetRenderDrawColor(renderer, color, color, color, 255);

            SDL_Rect rect = {
                (int)(x * cell_width),
                (int)(y * cell_height),
                (int)cell_width,
                (int)cell_height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

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

void completTrou(int mat[HEIGHT][WIDTH]) {
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
                if (!(not_enfermer)) copie[px][py] = -1;
            }
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

void plafond(FILE *fichier) {
    int alea = rand() % 4;
    switch (alea) {
        case 0:
            fprintf(fichier, "25:0 ");
            break;
        case 1:
            fprintf(fichier, "13:0 ");
            break;
        default:
            fprintf(fichier, "1:0 ");
            break;
    }
}

void sol(FILE *fichier) {
    int alea = rand() % 3;
    switch (alea) {
        case 0:
            fprintf(fichier, "49:0 ");
            break;
        case 1:
            fprintf(fichier, "50:0 ");
            break;
        case 2:
            fprintf(fichier, "43:0 ");
            break;
    }
}

void deco(FILE *fichier) {
    int alea = rand() % 6;
    switch (alea) {
        case 0:
            fprintf(fichier, "64:49 ");
            break;
        case 1:
            fprintf(fichier, "65:49 ");
            break;
        case 2:
            fprintf(fichier, "66:49 ");
            break;
        case 3:
            fprintf(fichier, "73:49 ");
            break;
        case 4:
            fprintf(fichier, "75:49 ");
            break;
        case 5:
            fprintf(fichier, "76:49 ");
            break;
    }
}

int blocSeul(int i, int j, int mat[HEIGHT][WIDTH]) {
    return ((i + 1) < HEIGHT) && ((i - 1) >= 0) && ((j + 1) < WIDTH) && ((j - 1) >= 0) && (!(mat[i - 1][j])) && (!(mat[i + 1][j])) && (!(mat[i][j + 1])) && (!(mat[i][j - 1]));
}
int angle_droit(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (j + 1 < WIDTH) && (mat[i][j]) && (!mat[i - 1][j]) && (!mat[i][j + 1]);
}
int angle_gauche(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (j - 1 >= 0) && (mat[i][j]) && (!mat[i - 1][j]) && (!mat[i][j - 1]);
}

int arrondi_inf_droit(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (j + 1 >= 0) && (mat[i - 1][j]) && (mat[i][j + 1]) && (!mat[i - 1][j + 1]);
}

int arrondi_inf_gauche(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (j - 1 >= 0) && (mat[i - 1][j]) && (mat[i][j - 1]) && (!mat[i - 1][j - 1]);
}

int arrondi_sup_droit(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i + 2 < HEIGHT) && (j + 1 >= 0) && (mat[i + 1][j + 1]) && (mat[i + 1][j]) && (mat[i + 1][j + 1]) && (mat[i + 2][j]) && (!mat[i + 2][j + 1]);
}

int arrondi_sup_gauche(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i + 2 < HEIGHT) && (j - 1 >= 0) && (mat[i + 1][j - 1]) && (mat[i + 1][j]) && (mat[i - 1][j + 1]) && (mat[i + 2][j]) && (!mat[i + 2][j - 1]);
}

void load(int mat[HEIGHT][WIDTH]) {
    system("find ~ -name 'testmap*.txt'|wc -l > nb.txt");
    FILE *nbFichier = fopen("nb.txt", "r");
    if (!nbFichier) {
        perror("problème d'ouverture du fichie 1r\n");
        exit(EXIT_FAILURE);
    }
    char nb[100];
    int entier;
    fscanf(nbFichier, "%d", &entier);
    entier += 1;
    sprintf(nb, "%d", entier);
    char chaine[100] = "testmap";
    strcat(chaine, nb);
    strcat(chaine, ".txt");
    fclose(nbFichier);
    char ouverture[100] = "../../assets/map/";
    strcat(ouverture, chaine);
    FILE *fichier = fopen(ouverture, "a");
    if (!fichier) {
        perror("problème d'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fichier, "%dx%dx1\n", WIDTH, HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if ((!mat[i][j])) {
                sol(fichier);
            } else if (blocSeul(i, j, mat)) {
                deco(fichier);
            } else if ((i + 1 < HEIGHT) && (!mat[i + 1][j])) {
                int alea = rand() % 3;
                switch (alea) {
                    case 0:
                        fprintf(fichier, "41:0 ");
                        break;
                    case 1:
                        fprintf(fichier, "29:0 ");
                        break;
                    case 2:
                        fprintf(fichier, "30:0 ");
                        break;
                }
            } else if (angle_droit(i, j, mat)) {
                fprintf(fichier, "6:0 ");
            } else if (angle_gauche(i, j, mat)) {
                fprintf(fichier, "5:0 ");
            } else if (arrondi_inf_droit(i, j, mat)) {
                fprintf(fichier, "26:0 ");
            } else if (arrondi_inf_gauche(i, j, mat)) {
                fprintf(fichier, "28:0 ");
            } else if (arrondi_sup_droit(i, j, mat)) {
                fprintf(fichier, "2:0 ");
            } else if (arrondi_sup_gauche(i, j, mat)) {
                fprintf(fichier, "4:0 ");
            } else if (i - 1 >= 0 && (!mat[i - 1][j])) {
                fprintf(fichier, "27:0 ");
            } else if ((i + 2 < HEIGHT) && (j - 1 >= 0) && ((mat[i + 1][j])) && (!mat[i + 2][j]) && (!mat[i + 1][j - 1])) {
                fprintf(fichier, "17:0 ");
            } else if ((i + 2 < HEIGHT) && (j + 1 < WIDTH) && ((mat[i + 1][j])) && (!mat[i + 2][j]) && (!mat[i + 1][j + 1])) {
                fprintf(fichier, "18:0 ");
            } else if ((i + 2 < HEIGHT) && (!mat[i + 2][j])) {
                fprintf(fichier, "3:0 ");
            } else if (j + 1 < WIDTH && i + 1 < HEIGHT && ((!mat[i][j + 1]) || ((mat[i][j + 1]) && (!mat[i + 1][j + 1])))) {
                fprintf(fichier, "14:0 ");
            } else if (j - 1 >= 0 && i + 1 < HEIGHT && (((!mat[i][j - 1]) || ((mat[i][j - 1]) && (!mat[i + 1][j - 1]))))) {
                fprintf(fichier, "16:0 ");
            } else
                plafond(fichier);
        }
        fprintf(fichier, "\n");
    }
    fprintf(fichier, "\n");
    fclose(fichier);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Bruit de Perlin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    float values[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            SDL_FPoint st = {(float)i / 8, (float)j / 8};  // plus le dénominateur est petit plus il y à de murs
            SDL_FPoint st2 = {(float)i / 10, (float)j / 10};
            SDL_FPoint st3 = {(float)i / 15, (float)j / 15};
            values[i][j] = noise(st);
            // values[i][j] = (values[i][j] + noise(st2)) / 2.0f;
            // values[i][j] = (values[i][j] + noise(st3)) / 2.0f;
        }
    }

    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        updateInput(&input);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        render_noise(renderer, values);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    int entier[HEIGHT][WIDTH];
    float_to_int(values, entier);
    afficheMat(entier);
    // completTrou(entier);

    load(entier);
    SDL_Quit();
    return EXIT_SUCCESS;
}
