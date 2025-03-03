
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

int blocSeul(t_case *c) {
    return (
        existVois(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_BAS], c->tabVoisin[VOISIN_GAUCHE], c->tabVoisin[VOISIN_DROIT]) 
        && (!(c->tabVoisin[VOISIN_HAUT]->val))
        && (!(c->tabVoisin[VOISIN_BAS]->val))
        && (!(c->tabVoisin[VOISIN_GAUCHE]->val))
        && (!(c->tabVoisin[VOISIN_DROIT])->val)
    );
}

int mur_avant(t_case *c) {
    return existVois(c->tabVoisin[VOISIN_BAS]) && (!(c->tabVoisin[VOISIN_BAS]->val));
}
int angle_droit(t_case *c) {
    return (
        existVois(c->tabVoisin[VOISIN_HAUT], c->tabVoisin[VOISIN_DROIT])
        && (!(c->tabVoisin[VOISIN_HAUT]->val)) 
        && (
            (!c->tabVoisin[VOISIN_DROIT]->val) 
            || (
                (c->tabVoisin[VOISIN_DROIT]->val) 
                && existVois(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])
                &&(!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))
            )
        )
    );
}
int angle_gauche(t_case * c) {
    return (
        existVois(c->tabVoisin[VOISIN_HAUT],c->tabVoisin[VOISIN_GAUCHE])         
        && (!(c->tabVoisin[VOISIN_HAUT]->val))                                         
        && (
            (!(c->tabVoisin[VOISIN_GAUCHE]->val)) 
            || (
                (c->tabVoisin[VOISIN_GAUCHE]->val)
                && existVois(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])
                && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))
            )
        )
    );
}

int arrondi_inf_droit(t_case * c) {
    return (
        existVois(c->tabVoisin[VOISIN_HAUT],c->tabVoisin[VOISIN_DIAG_DROIT_HAUT],c->tabVoisin[VOISIN_DROIT])
        && (c->tabVoisin[VOISIN_HAUT]->val) 
        && (c->tabVoisin[VOISIN_DROIT]->val) 
        && (!(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val))
    );
}

int arrondi_inf_gauche(t_case * c) {
    return (
        existVois(c->tabVoisin[VOISIN_HAUT],c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT],c->tabVoisin[VOISIN_GAUCHE]) 
        && (c->tabVoisin[VOISIN_HAUT]->val) 
        && (c->tabVoisin[VOISIN_GAUCHE]->val) 
        && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val))
    );
}

int arrondi_sup_droit(t_case *c) {
    return (
        (existVois(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_DIAG_DROIT_BAS],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT],c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) 
            && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val) 
            && (c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]->val)
            && (c->tabVoisin[VOISIN_BAS]->val) 
            && (c->tabVoisin[VOISIN_BAS2]->val) 
            && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val))
        )||(existVois(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_DIAG_DROIT_BAS],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]) 
            && (c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val) 
            && (!existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_HAUT]))
            && (c->tabVoisin[VOISIN_BAS]->val) 
            && (c->tabVoisin[VOISIN_BAS2]->val) 
            && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_DROIT]->val))
        )
    );
}

int arrondi_sup_gauche(t_case *c) {
    return(
        (existVois(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS],c->tabVoisin[VOISIN_DIAG_DROIT_HAUT],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE])
            && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val) 
            && (c->tabVoisin[VOISIN_BAS]->val) 
            && (c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]->val)
            && (c->tabVoisin[VOISIN_BAS2]->val) 
            && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val))
        )||(existVois(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS],c->tabVoisin[VOISIN_BAS2],c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]) 
            && (c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val) 
            && (!existe(c->tabVoisin[VOISIN_DIAG_DROIT_HAUT]))
            && (c->tabVoisin[VOISIN_BAS]->val) 
            && (c->tabVoisin[VOISIN_BAS2]->val) 
            && (!(c->tabVoisin[VOISIN_CENTRE_BAS2_GAUCHE]->val))
        )
    );
}

int mur_arriere(t_case *c) {
    return existVois(c->tabVoisin[VOISIN_HAUT]) && (!(c->tabVoisin[VOISIN_HAUT]->val));
}

int angle_continue_gauche(t_case *c) {
    return (
        existVois(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_BAS2]) 
        && ((c->tabVoisin[VOISIN_BAS]->val)) 
        && (!(c->tabVoisin[VOISIN_BAS2]->val)) 
        && (
            (existe(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])&&(!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))) 
            ||(existe(c->tabVoisin[VOISIN_GAUCHE])&& (!(c->tabVoisin[VOISIN_GAUCHE]->val)))
        )
    );
}

int angle_continue_droit(t_case *c) {
    return( 
        existVois(c->tabVoisin[VOISIN_BAS],c->tabVoisin[VOISIN_BAS2])  
        && ((c->tabVoisin[VOISIN_BAS]->val)) 
        && (!(c->tabVoisin[VOISIN_BAS2]->val)) 
        && (
            (existe(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])&&(!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))) 
            ||(existe(c->tabVoisin[VOISIN_DROIT])&& (!(c->tabVoisin[VOISIN_DROIT]->val)))
        )
    );
}

int bordure_mur_avant(t_case *c) {
    return existVois(c->tabVoisin[VOISIN_BAS2]) && (!(c->tabVoisin[VOISIN_BAS2]->val));
}

int bordure_mur_gauche(t_case *c) {
    return (
        existVois(c->tabVoisin[VOISIN_DROIT])
        &&(
            ((!(c->tabVoisin[VOISIN_DROIT]->val))) 
            ||((existVois(c->tabVoisin[VOISIN_DIAG_DROIT_BAS])) &&((c->tabVoisin[VOISIN_DROIT]->val) && (!(c->tabVoisin[VOISIN_DIAG_DROIT_BAS]->val))))
        )
    );
}

int bordure_mur_droit(t_case *c) {
    return (
        existVois(c->tabVoisin[VOISIN_GAUCHE])
        &&( 
            ((!(c->tabVoisin[VOISIN_GAUCHE]->val))) 
            ||((existVois(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS])) &&((c->tabVoisin[VOISIN_GAUCHE]->val) && (!(c->tabVoisin[VOISIN_DIAG_GAUCHE_BAS]->val))))
        )
    );
}

void choixTiles(t_listeBlock **lab, t_grille *g) {
    t_listeBlock *lb=listeByType(lab,MUR_TYPE);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            if ((!g->grille[i][j]->val)) {
                g->grille[i][j]->tiles = sol(lab);
            } else if (blocSeul(g->grille[i][j])) {
                g->grille[i][j]->tiles = deco(lab);
            } else if (mur_avant(g->grille[i][j])) {
                g->grille[i][j]->tiles = frontale(lab);
            } else if (angle_droit(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_DROIT);
            } else if (angle_gauche(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_GAUCHE);
            } else if (arrondi_inf_droit(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_INF_DROIT);
            } else if (arrondi_inf_gauche(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_INF_GAUCHE);
            } else if (arrondi_sup_droit(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_SUP_DROIT);
            } else if (arrondi_sup_gauche(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ARRONDI_SUP_GAUCHE);
            } else if (mur_arriere(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_ARRIERE);
            } else if (angle_continue_gauche(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_CONTINUE_GAUCHE);
            } else if (angle_continue_droit(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_ANGLE_CONTINUE_DROIT);
            } else if (bordure_mur_avant(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_AVANT);
            } else if (bordure_mur_gauche(g->grille[i][j])) {
                g->grille[i][j]->tiles = blockByName(lb,MUR_BORDURE_MUR_GAUCHE);
            } else if (bordure_mur_droit(g->grille[i][j])) {
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
    fprintf(fichier, "%dx%dx1\n", g->nbColonne, g->nbLigne);
    for (int i = 0; i < g->nbLigne; i++) {
        for (int j = 0; j < g->nbColonne; j++) {
            fprintf(fichier,"%s:%s ",g->grille[i][j]->tiles->tiles,g->grille[i][j]->tiles->rotation);
        }
        fprintf(fichier, "\n");
    }
    fprintf(fichier, "\n");
    fclose(fichier);
}

t_grille * geneRomm(){
    srand(time(NULL));

    /*
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Bruit de Perlin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    */
    int nbLigne = (rand() % 60 + 20) & ~1;
    int nbColonne = (rand() % 2) == 0 ? (nbLigne + rand() % 11) & ~1 : (nbLigne - rand() % 11) & ~1;

    SDL_FPoint seed;
    seed.x = ((rand()) % 90 + 10) + (((float)(rand() + 1)) / RAND_MAX);
    seed.y = ((rand()) % 90 + 10) + (((float)(rand() + 1)) / RAND_MAX);

    printf("%f %f\n",seed.x,seed.y);


    t_listeBlock **lab = createListAllBlock();

    float** values = matFloat(nbLigne,nbColonne);

    for (int i = 0; i < nbLigne; i++) {
        for (int j = 0; j < nbColonne; j++) {
            SDL_FPoint st = {(float)i / 10, (float)j / 10};  // plus le dénominateur est petit plus il y à de murs
            SDL_FPoint st2 = {(float)i / 10, (float)j / 10};
            SDL_FPoint st3 = {(float)i / 15, (float)j / 15};
            values[i][j] = noise(st,seed);
            // values[i][j] = (values[i][j] + noise(st2)) / 2.0f;
            // values[i][j] = (values[i][j] + noise(st3)) / 2.0f;
        }
    }

    /*
    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        updateInput(&input);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        render_noise(renderer, values,nbLigne,nbColonne);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    */
    int **entier = float_to_int(values,nbLigne,nbColonne);
    // afficheMat(entier);
    fill_gaps(entier, nbLigne, nbColonne);
    lissage(entier, nbLigne, nbColonne);

    t_grille *grille = intToGrilleNiveau(entier, nbLigne, nbColonne);

    // printf("%d", existVois(grille->grille[50][50]->tabVoisin[VOISIN_BAS2]));
    choixTiles(lab, grille);
    load(grille);

    freeMatInt(entier, nbLigne, nbColonne);
    freeMatFloat(values, nbLigne, nbColonne);
    freeListeBlock(lab);

    //SDL_Quit();
    return grille;
}


int main(){
    t_grille * g = geneRomm();
    freeGrille(g);
}