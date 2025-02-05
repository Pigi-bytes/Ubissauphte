#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "io/imageLoader.h"
#include "io/input.h"

#define window_width 1280
#define window_height 960
#define WORLD_WIDTH 2000
#define WORLD_HEIGHT 2000
#define NUM_OBSTACLES 15

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} t_control;

typedef struct {
    SDL_Rect rect;  // Position et taille dans le monde
    SDL_Texture* texture;
} t_entity;

typedef struct {
    t_entity entity;
    t_control* control;
} t_joueur;

typedef struct {
    t_entity entity;
} t_obstacle;

typedef struct {
    int x, y;         // position dans le monde
    int w, h;         // dimensions de l'écran
    int worldWidth;   // largeur du monde
    int worldHeight;  // hauteur du monde
} t_camera;

SDL_bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2);
void movePlayer(t_joueur* player, int dx, int dy, t_obstacle obstacles[], int num_obstacles);
void handleInputPlayer(t_joueur* player, t_obstacle obstacles[], int num_obstacles, t_camera* camera, t_input* input);
void renderRect(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color);
void renderPlayer(SDL_Renderer* renderer, t_joueur* object);
t_joueur* createplayer(t_control* control, SDL_Texture* texture, SDL_Rect rect);
void freePlayer(t_joueur* player);

SDL_Texture* creerTextureDepuisRect(SDL_Renderer* renderer, SDL_Color couleur, SDL_Rect rect);

void cameraCenterOnRect(t_camera* camera, SDL_Rect* target);
SDL_Rect cameraTransformWorld2Camera(t_camera* camera, SDL_Rect* world_rect);
void renderEntity(SDL_Renderer* renderer, t_camera* camera, t_entity* entity);
SDL_bool entityOnCamera(t_camera* camera, SDL_Rect* rectWordCoord);