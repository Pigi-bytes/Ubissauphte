#include <stdbool.h>

#include "./io/input.h"

typedef struct
{
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
    SDL_Rect rect;
} t_joueur;

int check_collision(t_joueur* joueur1, t_joueur* joueur2);
void move_player(t_joueur* joueur, int dx, int dy, t_joueur* joueur2);
void handle_input(t_joueur* joueur1, t_joueur* joueur2, t_input* input, SDL_Window* window);

void drawPlayer(t_joueur* joueur1);
void dessin_rect(SDL_Renderer* renderer, SDL_Rect* rect, int r, int v, int b, int o);