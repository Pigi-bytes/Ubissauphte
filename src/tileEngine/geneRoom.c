
#include "../debug.h"
#include "../io/input.h"
#include "fill_gaps.h"
#include "general.h"
#include "lissage.h"
#include "perlinNoise.h"

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

void frontale(FILE *fichier) {
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
}

int blocSeul(int i, int j, int mat[HEIGHT][WIDTH]) {
    return ((i + 1) < HEIGHT) && ((i - 1) >= 0) && ((j + 1) < WIDTH) && ((j - 1) >= 0) && (!(mat[i - 1][j])) && (!(mat[i + 1][j])) && (!(mat[i][j + 1])) && (!(mat[i][j - 1]));
}

int mur_avant(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i + 1 < HEIGHT) && (!mat[i + 1][j]);
}
int angle_droit(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (i + 1 < HEIGHT) && (j + 1 < WIDTH) && (!mat[i - 1][j]) && ((!mat[i][j + 1]) || ((mat[i][j + 1]) && (!(mat[i + 1][j + 1]))));
}
int angle_gauche(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (i + 1 < HEIGHT) && (j - 1 >= 0) && (!mat[i - 1][j]) && ((!mat[i][j - 1]) || ((mat[i][j - 1]) && (!(mat[i + 1][j - 1]))));
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

int mur_arriere(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i - 1 >= 0) && (!mat[i - 1][j]);
}

int angle_continue_gauche(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i + 2 < HEIGHT) && (j - 1 >= 0) && ((mat[i + 1][j])) && (!mat[i + 2][j]) && ((!mat[i + 1][j - 1]) || (!mat[i][j - 1]));
}

int angle_continue_droit(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i + 2 < HEIGHT) && (j + 1 < WIDTH) && ((mat[i + 1][j])) && (!mat[i + 2][j]) && ((!mat[i + 1][j + 1]) || (!mat[i][j + 1]));
}

int bordure_mur_avant(int i, int j, int mat[HEIGHT][WIDTH]) {
    return (i + 2 < HEIGHT) && (!mat[i + 2][j]);
}

int bordure_mur_gauche(int i, int j, int mat[HEIGHT][WIDTH]) {
    return j + 1 < WIDTH && i + 1 < HEIGHT && ((!mat[i][j + 1]) || ((mat[i][j + 1]) && (!mat[i + 1][j + 1])));
}

int bordure_mur_droit(int i, int j, int mat[HEIGHT][WIDTH]) {
    return j - 1 >= 0 && i + 1 < HEIGHT && (((!mat[i][j - 1]) || ((mat[i][j - 1]) && (!mat[i + 1][j - 1]))));
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
            } else if (mur_avant(i, j, mat)) {
                frontale(fichier);
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
            } else if (mur_arriere(i, j, mat)) {
                fprintf(fichier, "27:0 ");
            } else if (angle_continue_gauche(i, j, mat)) {
                fprintf(fichier, "17:0 ");
            } else if (angle_continue_droit(i, j, mat)) {
                fprintf(fichier, "18:0 ");
            } else if (bordure_mur_avant(i, j, mat)) {
                fprintf(fichier, "3:0 ");
            } else if (bordure_mur_gauche(i, j, mat)) {
                fprintf(fichier, "14:0 ");
            } else if (bordure_mur_droit(i, j, mat)) {
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
            SDL_FPoint st = {(float)i / 5, (float)j / 5};  // plus le dénominateur est petit plus il y à de murs
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
    fillGaps(entier);
    lissage(entier);

    load(entier);
    printf("1\n");
    SDL_Quit();
    return EXIT_SUCCESS;
}
