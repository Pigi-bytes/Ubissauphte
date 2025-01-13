#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "./io/input.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 1280
#define WORLD_WIDTH 2000
#define WORLD_HEIGHT 2000
#define NUM_OBSTACLES 20

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

typedef struct {
    int x, y, w, h;
} t_camera;

SDL_bool check_collision(SDL_Rect* rect1, SDL_Rect* rect2) {
    return (SDL_bool)((rect1->x < rect2->x + rect2->w &&
                       rect1->x + rect1->w > rect2->x &&
                       rect1->y < rect2->y + rect2->h &&
                       rect1->y + rect1->h > rect2->y));
}

void update_camera(t_camera* camera, SDL_Rect* target, int world_width, int world_height) {
    camera->x = target->x + target->w / 2 - camera->w / 2;
    camera->y = target->y + target->h / 2 - camera->h / 2;

    if (camera->x < 0) camera->x = 0;
    if (camera->y < 0) camera->y = 0;
    if (camera->x > world_width - camera->w) camera->x = world_width - camera->w;
    if (camera->y > world_height - camera->h) camera->y = world_height - camera->h;
}

void move_player(t_joueur* player, int dx, int dy, Obstacle obstacles[], int num_obstacles) {
    SDL_Rect temp = player->rect;
    temp.x += dx;
    temp.y += dy;

    for (int i = 0; i < num_obstacles; i++) {
        if (check_collision(&temp, &obstacles[i].rect)) {
            return;
        }
    }

    player->rect.x = temp.x;
    player->rect.y = temp.y;
}

void handle_input(t_joueur* player, Obstacle obstacles[], int num_obstacles, t_camera* camera, t_input* input) {
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

    move_player(player, dx, dy, obstacles, num_obstacles);

    update_camera(camera, &player->rect, WORLD_WIDTH, WORLD_HEIGHT);
}

SDL_bool is_visible(SDL_Rect* obj, t_camera* camera) {
    return (obj->x + obj->w > camera->x && obj->x < camera->x + camera->w &&
            obj->y + obj->h > camera->y && obj->y < camera->y + camera->h);
}

void render_with_camera(SDL_Renderer* renderer, t_camera* camera, SDL_Rect* obj, void (*custom_render)(SDL_Renderer*, SDL_Rect*, SDL_Color), SDL_Color color) {
    if (is_visible(obj, camera)) {
        SDL_Rect render_rect = {
            obj->x - camera->x,
            obj->y - camera->y,
            obj->w,
            obj->h};
        custom_render(renderer, &render_rect, color);
    }
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

    Obstacle obstacles[NUM_OBSTACLES];
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].rect = (SDL_Rect){rand() % (WORLD_WIDTH - 100), rand() % (WORLD_HEIGHT - 100), 50 + rand() % 50, 50 + rand() % 50};
        obstacles[i].color = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};
    }

    t_camera camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    t_input input;
    initInput(&input, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!input.quit) {
        updateInput(&input);

        handle_input(player1, &obstacles, NUM_OBSTACLES, &camera, &input);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render_with_camera(renderer, &camera, &player1->rect, render_rect, color_j1);

        for (int i = 0; i < NUM_OBSTACLES; i++) {
            render_with_camera(renderer, &camera, &obstacles[i].rect, render_rect, obstacles[i].color);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}