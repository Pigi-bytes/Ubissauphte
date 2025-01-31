#include "mapview.h"
#include <SDL2/SDL.h>

void printMap(int *tabCoordsX, int *tabCoordsY, t_salle **listeRoom, int numberRoom, SDL_Renderer *pRendu){


    SDL_Rect rectRoom; /////// A MODIFIER TOUT

    //printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    for (int i = 0; i < numberRoom; i++){
        //printf("%d\n", i);
        int taille = 50;
        int espace = 5;
        int coordX = tabCoordsX[i]*taille+500;
        int coordY = tabCoordsY[i]*taille+500;
        rectRoom.x = coordX;
        rectRoom.y = coordY;
        rectRoom.h = taille-espace;
        rectRoom.w = taille-espace;
        SDL_SetRenderDrawColor(pRendu,255,0,0,255);
        SDL_RenderDrawRect(pRendu, &rectRoom);

        if (listeRoom[i]->droite != NULL){
            SDL_SetRenderDrawColor(pRendu, 255, 255, 0, 255);
            SDL_RenderDrawLine(pRendu, coordX+taille/2, coordY+taille/2, coordX+taille/2+(taille/2+espace), coordY+taille/2);
        }if (listeRoom[i]->gauche != NULL){
            SDL_SetRenderDrawColor(pRendu, 0, 255, 0, 255);
            SDL_RenderDrawLine(pRendu, coordX+taille/2, coordY+taille/2, coordX+taille/2-(taille/2+espace), coordY+taille/2);
        }if (listeRoom[i]->haut != NULL){
            SDL_SetRenderDrawColor(pRendu, 0, 255, 255, 255);
            SDL_RenderDrawLine(pRendu, coordX+taille/2, coordY+taille/2, coordX+taille/2, coordY+taille/2+(taille/2+espace));
        }if (listeRoom[i]->bas != NULL){
            SDL_SetRenderDrawColor(pRendu, 0, 0, 255, 255);
            SDL_RenderDrawLine(pRendu, coordX+taille/2, coordY+taille/2, coordX+taille/2, coordY+taille/2-(taille/2+espace));
        }
        printf("tabCoordX [%d] : %d X et %d Y\n", i, tabCoordsX[i], tabCoordsY[i]);
    }
}

/*
int *genMapCoords(t_salle **tabRoom, int numberRoom, SDL_Renderer *pRendu){
    
    for (int i = 1; i < numberRoom; i++){ // pour chaque struct de la liste elm
        
        for (int j = 0; j < i; j++){ // pour tt les tabCoords qui ont plus d'ancienneté que l'actuel
            if ((tabCoordsX[j] == tabCoordsX[i]) && (tabCoordsY[j] == tabCoordsY[i])){ // Si les coords du nouvelle elm == une coord présente
                printf("i = %d, tabCoordsX[i] = %d, tabCoordsY[i] = %d\n", i, tabCoordsX[i], tabCoordsY[i]);
                printf("j = %d, tabCoordsX[j] = %d, tabCoordsY[j] = %d\n", j, tabCoordsX[j], tabCoordsY[j]);
                for (int h = 0; h < i; h++){ // pour tt les tabCoords qui ont plus d'ancienneté que l'actuel ET qui sont à décaler
                    if (direction == 1 && (tabCoordsY[i] >= tabCoordsY[h])){
                        tabCoordsY[h] -= 1;
                    }else if (direction == 2 && (tabCoordsX[i] >= tabCoordsX[h])){
                        tabCoordsX[h] -= 1;
                    }else if (direction == 3 && (tabCoordsY[i] >= tabCoordsY[h])){
                        tabCoordsY[h] += 1;
                    }else if (direction == 4 && (tabCoordsX[i] >= tabCoordsX[h])){
                        tabCoordsX[h] += 1;
                    }
                }
                printf("i = %d, tabCoordsX[i] = %d, tabCoordsY[i] = %d\n", i, tabCoordsX[i], tabCoordsY[i]);
                printf("j = %d, tabCoordsX[j] = %d, tabCoordsY[j] = %d\n", j, tabCoordsX[j], tabCoordsY[j]);
            }
        }
    }

    /*for (int i = 0; i < numberRoom; i ++){
        printf("Element i = %d :\n", i/2);
        printf("X = %d et Y = %d\n\n", tabCoordsX[i],tabCoordsY[i]);
    }



    int *tabCoords = malloc(sizeof(int)*numberRoom*2);
    for (int i = 0; i < numberRoom*2; i += 2){
        tabCoords[i] = tabCoordsX[i];
        tabCoords[i] = tabCoordsY[i+1];
    }

    return tabCoords;
}*/
