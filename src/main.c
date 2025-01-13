#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "./io/input.h"

#define window_width 1280
#define window_height 960
#define NUM_OBSTACLES 1

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} t_control;

typedef struct {
    SDL_Rect rect;
    t_control* control;
} t_joueur;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Obstacle;

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

t_joueur* createplayer(t_control* control, SDL_Rect rect) {
    t_joueur* joueur = (t_joueur*)malloc(sizeof(t_joueur));

    joueur->control = control;
    joueur->rect = rect;

    return joueur;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_control fleche = {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT};

    t_joueur* player1 = createplayer(&fleche, (SDL_Rect){400, 300, 100, 100});
    SDL_Color color_j1 = {0, 0, 255, 255};
    Obstacle obstacles = {
        .rect = {600, 600, 100, 100},
        .color = {255, 255, 255, 255}};

    t_input input;
    initInput(&input, window_height, window_width);

    while (!input.quit) {
        updateInput(&input);

        handle_input(player1, &obstacles, NUM_OBSTACLES, &input);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_rect(renderer, &player1->rect, color_j1);
        render_rect(renderer, &obstacles.rect, obstacles.color);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}