
#include "geneRoom.h"

t_block * plafond(t_listeBlock **lab) {
    t_listeBlock *lb = listeByType(lab, PLAFOND_TYPE);
    return randomBlocByType(lb);
}

t_block *sol(t_listeBlock **lab) {
    t_listeBlock *lb = listeByType(lab, SOL_TYPE);
    return randomBlocByType(lb);
}

t_block * deco(t_listeBlock **lab) {
    t_listeBlock *lb = listeByType(lab, DECO_TYPE);
    return randomBlocByType(lb);
}

t_block * frontale(t_listeBlock **lab) {
    t_listeBlock *lb = listeByType(lab, FRONTAL_TYPE);
    return randomBlocByType(lb);
}

int blocSeul(int i, int j, t_case *c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_GAUCHE], c->tabVoisin[VOISIN_DROIT]) 
        && (!(c->tabVoisin[VOISIN_HAUT]->val))
        && (!(c->tabVoisin[VOISIN_BAS]->val))
        && (!(c->tabVoisin[VOISIN_GAUCHE]->val))
        && (!(c->tabVoisin[VOISIN_DROIT])->val)
    );
}

int mur_avant(int i, int j, t_case *c) {
    return existeVoisin(c->tabVoisin[VOISIN_BAS]) && (!(c->tabVoisin[VOISIN_BAS]->val));
}
int angle_droit(int i, int j, t_case *c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DROIT])
        && (!(c->tabVoisin[VOISIN_HAUT]->val)) 
        && (
            (!c->tabVoisin[VOISIN_DROIT]->val) 
            || (
                (c->tabVoisin[VOISIN_DROIT]->val) 
                && existeVoisin(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])
                &&(!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))
            )
        )
    );
}
int angle_gauche(int i, int j, t_case * c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_HAUT],c->tabVoisin[VOISIN_GAUCHE])         
        && (c->tabVoisin[VOISIN_HAUT]->val)                                         
        && (
            (!(c->tabVoisin[VOISIN_GAUCHE]->val)) 
            || (
                (c->tabVoisin[VOISIN_GAUCHE]->val)
                && existeVoisin(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])
                && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))
            )
        )
    );
}

int arrondi_inf_droit(int i, int j, t_case * c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_HAUT],c->tabVoisin[VOISIN_DIAG_DROIT_HAUT],c->tabVoisin[VOISIN_DROIT])
        && (c->tabVoisin[VOISIN_HAUT]->val) 
        && (c->tabVoisin[VOISIN_DROIT]->val) 
        && (!(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val))
    );
}

int arrondi_inf_gauche(int i, int j, t_case * c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_HAUT],c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT],c->tabVoisin[VOISIN_GAUCHE]) 
        && (c->tabVoisin[VOISIN_HAUT]->val) 
        && (c->tabVoisin[VOISIN_GAUCHE]->val) 
        && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val))
    );
}

int arrondi_sup_droit(int i, int j, t_case *c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_DIAG_DROIT_BAS],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) 
        && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val) 
        && (c->tabVoisin[VOISIN_BAS]->val) 
        && (c->tabVoisin[VOISIN_BAS2]->val) 
        && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val))
    );
}

int arrondi_sup_gauche(int i, int j, t_case *c) {
    return(
        existeVoisin(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS],c->tabVoisin[VOISIN_DIAG_DROIT_HAUT],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE])
        && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val) 
        && (c->tabVoisin[VOISIN_BAS]->val) 
        && (c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val)
        && (c->tabVoisin[VOISIN_BAS2]->val) 
        && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val))
    );
}

int mur_arriere(int i, int j, t_case *c) {
    return existeVoisin(c->tabVoisin[VOISIN_HAUT]) && (!(c->tabVoisin[VOISIN_HAUT]->val));
}

int angle_continue_gauche(int i, int j, t_case *c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_BAS2]) 
        && ((c->tabVoisin[VOISIN_BAS]->val)) 
        && (!(c->tabVoisin[VOISIN_BAS2]->val)) 
        && (
            (existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])&&(!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))) 
            ||(existe(c->tabVoisin[VOISIN_GAUCHE])&& (!(c->tabVoisin[VOISIN_GAUCHE]->val)))
        )
    );
}

int angle_continue_droit(int i, int j, t_case *c) {
    return( 
        existeVoisin(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_BAS2])  
        && ((c->tabVoisin[VOISIN_BAS]->val)) 
        && (!(c->tabVoisin[VOISIN_BAS2]->val)) 
        && (
            (existe(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])&&(!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))) 
            ||(existe(c->tabVoisin[VOISIN_DROIT])&& (!(c->tabVoisin[VOISIN_DROIT]->val)))
        )
    );
}

int bordure_mur_avant(int i, int j, t_case *c) {
    return existeVoisin(c->tabVoisin[VOISIN_BAS2]) && (!(c->tabVoisin[VOISIN_BAS2]->val));
}

int bordure_mur_gauche(int i, int j, t_case *c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_DROIT])
        &&(
            ((!(c->tabVoisin[VOISIN_DROIT]->val))) 
            ||((existeVoisin(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])) &&((c->tabVoisin[VOISIN_DROIT]->val) && (!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))))
        )
    );
}

int bordure_mur_droit(int i, int j, t_case *c) {
    return (
        existeVoisin(c->tabVoisin[VOISIN_GAUCHE])
        &&( 
            ((!(c->tabVoisin[VOISIN_GAUCHE]->val))) 
            ||((existeVoisin(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])) &&((c->tabVoisin[VOISIN_GAUCHE]->val) && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))))
        )
    );
}

void choixTiles(t_listeBlock **lab, t_grille *g) {
    t_listeBlock *lb=listeByType(lab,MUR_TYPE);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            if ((!g->grille[i][j]->val)) {
                g->grille[i][j]->tiles = sol(lab);
            } else if (blocSeul(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = deco(lab);
            } else if (mur_avant(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = frontale(lab);
            } else if (angle_droit(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_DROIT);
            } else if (angle_gauche(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_GAUCHE);
            } else if (arrondi_inf_droit(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_INF_DROIT);
            } else if (arrondi_inf_gauche(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_INF_GAUCHE);
            } else if (arrondi_sup_droit(i, j,  g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_SUP_DROIT);
            } else if (arrondi_sup_gauche(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_SUP_GAUCHE);
            } else if (mur_arriere(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_ARRIERE);
            } else if (angle_continue_gauche(i, j,g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_CONTINUE_GAUCHE);
            } else if (angle_continue_droit(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_CONTINUE_DROIT);
            } else if (bordure_mur_avant(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_AVANT);
            } else if (bordure_mur_gauche(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_GAUCHE);
            } else if (bordure_mur_droit(i, j, g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_DROIT);
            } else
            g->grille[i][j]->tiles  = plafond(lab);
        }
    }
}

void load(t_grille * g) {
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
    fprintf(fichier, "%dx%dx1\n", g->nbLigne, g->nbColonne);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            fprintf(fichier,"%s:%s ",g->grille[i][j]->tiles->tiles,g->grille[i][j]->tiles->rotation);
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
    t_listeBlock **lab = createListAllBlock();

    float values[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            SDL_FPoint st = {(float)i / 10, (float)j / 10};  // plus le dénominateur est petit plus il y à de murs
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
    int **entier = float_to_int(values);
    //afficheMat(entier);
    fill_gaps(entier,HEIGHT,WIDTH);
    lissage(entier,HEIGHT,WIDTH);

    t_grille *grille = intToGrilleNiveau(entier, HEIGHT, WIDTH);

    choixTiles(lab,grille);
    load(grille);

    freeMatInt(entier, HEIGHT, WIDTH);
    freeGrille(grille);
    freeListeBlock(lab);

    SDL_Quit();
    return EXIT_SUCCESS;
}