#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_element {
    SDL_Point* point;
    struct s_element* precedent;
} t_element;

t_element* Tete;

void initPile();
int PileVide();
void empiler(SDL_Point* p);
SDL_Point* depiler();
void viderPile();