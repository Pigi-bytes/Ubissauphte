#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../io/imageLoader.h"
#include "../io/input.h"
#include "tilesManager.h"

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} t_control;

typedef struct {
    t_entity entity;
    t_control* control;
} t_joueur;

typedef struct {
    t_entity entity;
} t_obstacle;

SDL_bool checkCollision(SDL_Rect* rect1, SDL_Rect* rect2);
void movePlayer(t_joueur* player, int dx, int dy, t_grid* grid);
void handleInputPlayer(t_joueur* player, t_grid* grid, t_input* input);
t_joueur* createplayer(t_control* control, SDL_Texture* texture, SDL_Rect rect);
void freePlayer(t_joueur* player);

SDL_Texture* creerTextureDepuisRect(SDL_Renderer* renderer, SDL_Color couleur, SDL_Rect rect);

#endif