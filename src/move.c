#include <stdio.h>
#include <stdlib.h>

#include "./move.h"



SDL_Texture* loadImage(char* filename, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(filename);
    if (!surface) {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

SDL_bool check_collision(SDL_Rect* rect1, SDL_Rect* rect2, t_input* input) {
    return (SDL_bool)((rect1->x < rect2->x + rect2->w &&
                       rect1->x + rect1->w > rect2->x &&
                       rect1->y < rect2->y + rect2->h &&
                       rect1->y + rect1->h > rect2->y) ||
                      (rect1->y >= input->windowHeight - rect1->h) ||
                      rect1->y <= 0);
}

void move_player(t_joueur* player, int dx, int dy, Obstacle obstacles[], int num_obstacles, t_input* input) {
    SDL_Rect temp = player->rect;
    temp.x += dx;
    temp.y += dy;

    for (int i = 0; i < num_obstacles; i++) {
        if (check_collision(&temp, &obstacles[i].rect, input)) {
            return;
        }
    }

    player->rect.x = temp.x;
    player->rect.y = temp.y;
}

void handle_input(t_joueur* player, Obstacle obstacles[], int num_obstacles, t_input* input) {
    float speed = 15;
    float dx = 0;
    float dy = 0;

    if (input->key[player->control->left]) dx = -speed;
    if (input->key[player->control->right]) dx = speed;
    if (input->key[player->control->up]) dy = -speed;
    if (input->key[player->control->down]) dy = speed;

    if (dx && dy) {
        dx *= (1.0 / sqrt(2));
        dy *= (1.0 / sqrt(2));
    }

    move_player(player, dx, dy, obstacles, num_obstacles, input);

    if (player->rect.x <= -player->rect.w)
        player->rect.x = input->windowWidth;

    else if (player->rect.x >= input->windowWidth)
        player->rect.x = -player->rect.w;
}

void render_rect(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, rect);
}

void drawObject(SDL_Renderer* renderer, t_joueur* object) {
    SDL_RenderCopy(renderer, object->texture, NULL, &object->rect);
}

t_joueur* createplayer(t_control* control, SDL_Texture* texture, SDL_Rect rect) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->texture = texture;
    joueur->rect = rect;

    return joueur;
}

void freePlayer(t_joueur* player) {
    SDL_DestroyTexture(player->texture);
    free(player);
}