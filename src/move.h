#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "./io/input.h"

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} t_control;

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    t_control* control;
} t_joueur;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Obstacle;

SDL_Texture* loadImage(char* filename, SDL_Renderer* renderer);
SDL_bool check_collision(SDL_Rect* rect1, SDL_Rect* rect2, t_input* input);
void move_player(t_joueur* player, int dx, int dy, Obstacle obstacles[], int num_obstacles, t_input* input);
void handle_input(t_joueur* player, Obstacle obstacles[], int num_obstacles, t_input* input);
void render_rect(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color);
void drawObject(SDL_Renderer* renderer, t_joueur* object);
t_joueur* createplayer(t_control* control, SDL_Texture* texture, SDL_Rect rect);
void freePlayer(t_joueur* player);





