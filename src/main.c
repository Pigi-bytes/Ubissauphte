#include <SDL2/SDL.h>
#include <stdio.h>

#include "touches.h"

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

/*this fonction detect collisions*/
int check_collision(t_joueur* joueur1, t_joueur* joueur2) {
    if (joueur2->rect.y + joueur2->rect.h >= joueur1->rect.y && joueur2->rect.y - joueur2->rect.h <= joueur1->rect.y - joueur1->rect.h) {
        /*Test if there is an obstacle at left when the obstacle is bigger*/
        if (joueur1->rect.x == joueur2->rect.x + joueur2->rect.w && joueur2->rect.h >= joueur1->rect.h)
            return LEFT;
        /*Test if there is an obstacle at right when the obstacle is bigger*/
        else if (joueur1->rect.x + joueur1->rect.w == joueur2->rect.x && joueur2->rect.h >= joueur1->rect.h)
            return RIGHT;
    }
    if (joueur1->rect.y >= joueur2->rect.y - joueur1->rect.h && joueur1->rect.y <= joueur2->rect.y + joueur1->rect.h - joueur2->rect.h) {
        /*Test if there is an obstacle at left when the obstacle is smaller*/
        if (joueur1->rect.x == joueur2->rect.x + joueur2->rect.w)
            return LEFT;
        /*Test if there is an obstacle at right when the obstacle is smaller */
        else if (joueur1->rect.x + joueur1->rect.w == joueur2->rect.x)
            return RIGHT;
    }
    if (joueur2->rect.x + joueur2->rect.w >= joueur1->rect.x && joueur2->rect.x <= joueur1->rect.x + joueur1->rect.w) {
        /*Test if there is an obstacle down when the obstacle is bigger*/
        if (joueur1->rect.y + joueur1->rect.h == joueur2->rect.y && joueur2->rect.w >= joueur1->rect.w)
            return UP;
        /*Test if there is an obstacle up when the obstacle is bigger*/
        else if (joueur1->rect.y == joueur2->rect.y + joueur2->rect.h && joueur2->rect.w >= joueur1->rect.w)
            return DOWN;
    }
    if (joueur1->rect.x >= joueur2->rect.x - joueur1->rect.w && joueur1->rect.x <= joueur2->rect.x + joueur1->rect.w - joueur2->rect.w) {
        /*Test if there is an obstacle down when the obstacle is smaller*/
        if (joueur1->rect.y + joueur1->rect.h == joueur2->rect.y)
            return UP;
        /*Test if there is an obstacle up when the obstacle is smaller*/
        else if (joueur1->rect.y == joueur2->rect.y + joueur2->rect.h)
            return DOWN;
    }
    return 0;
}
/*this fonction place the player*/
void move_player(t_joueur* joueur1, int dx, int dy, t_joueur* joueur2) {
    joueur1->rect.x = dx;
    joueur1->rect.y = dy;

    joueur2->rect.x = dx + 100;
    joueur2->rect.y = dy + 100;
}
/*this fonction allow to control the player*/
void handle_input(t_joueur* joueur1, t_joueur* joueur2, t_input* input, SDL_Window* window) {
    int coll = check_collision(joueur1, joueur2);
    int h, w;

    if (input->key[joueur1->up]) {
        SDL_GetWindowSize(window, &w, &h);
        if (input->key[joueur1->up] && input->key[joueur1->right]) {
            if (!(coll == DOWN))
                joueur1->rect.y -= 2;
            if (!(coll == RIGHT))
                joueur1->rect.x += 2;
            if (joueur1->rect.x > w)
                joueur1->rect.x = -joueur1->rect.w;
            keyPressOnce(input, joueur1->right);
            keyPressOnce(input, joueur1->up);
        } else if (input->key[joueur1->up] && input->key[joueur1->left]) {
            if (!(coll == DOWN))
                joueur1->rect.y -= 2;
            if (!(coll == LEFT))
                joueur1->rect.x -= 2;
            if (joueur1->rect.x <= -joueur1->rect.w)
                joueur1->rect.x = w;
            keyPressOnce(input, joueur1->left);
            keyPressOnce(input, joueur1->up);
        } else if (!input->key[joueur1->right] && !input->key[joueur1->left]) {
            if (!(coll == DOWN))
                joueur1->rect.y -= 2;
            keyPressOnce(input, joueur1->up);
        }

    } else if (input->key[joueur1->down]) {
        SDL_GetWindowSize(window, &w, &h);
        if (input->key[joueur1->down] && input->key[joueur1->right]) {
            if (joueur1->rect.y != h - joueur1->rect.h && !(coll == UP))
                joueur1->rect.y += 2;
            if (!(coll == RIGHT))
                joueur1->rect.x += 2;
            if (joueur1->rect.x >= w)
                joueur1->rect.x = -joueur1->rect.w;
            keyPressOnce(input, joueur1->down);
            keyPressOnce(input, joueur1->right);
        } else if (input->key[joueur1->down] && input->key[joueur1->left]) {
            if (joueur1->rect.y != h - joueur1->rect.h && !(coll == UP))
                joueur1->rect.y += 2;
            if (!(coll == LEFT))
                joueur1->rect.x -= 2;
            if (joueur1->rect.x <= -joueur1->rect.w)
                joueur1->rect.x = w;
            keyPressOnce(input, joueur1->down);
            keyPressOnce(input, joueur1->left);

        } else if (!input->key[joueur1->right] && !input->key[joueur1->left] && joueur1->rect.y != h - joueur1->rect.h) {
            if (!(coll == UP))
                joueur1->rect.y += 2;
            keyPressOnce(input, joueur1->down);
        }

    } else if (input->key[joueur1->right] && !input->key[joueur1->up] && !input->key[joueur1->down]) {
        SDL_GetWindowSize(window, &w, &h);

        if (!(coll == RIGHT))
            joueur1->rect.x += 2;
        if (joueur1->rect.x >= w)
            joueur1->rect.x = -joueur1->rect.w;
        keyPressOnce(input, joueur1->right);

    } else if (input->key[joueur1->left] && !input->key[joueur1->down] && !input->key[joueur1->up]) {
        SDL_GetWindowSize(window, &w, &h);

        if (!(coll == LEFT))
            joueur1->rect.x -= 2;
        if (joueur1->rect.x <= -joueur1->rect.w)
            joueur1->rect.x = w;
        keyPressOnce(input, joueur1->left);
    }
}

/*this fonction give a size for the player*/
void drawPlayer(t_joueur* joueur1) {
    joueur1->rect.h = 100;
    joueur1->rect.w = 100;
}

/*this fonction draw the player*/

void dessin_rect(SDL_Renderer* renderer, SDL_Rect* rect, int r, int v, int b, int o) {
    SDL_SetRenderDrawColor(renderer, r, v, b, o);
    SDL_RenderDrawRect(renderer, rect);
    SDL_RenderFillRect(renderer, rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    t_joueur joueur1;
    t_joueur joueur2;
    t_joueur joueur3;
    t_joueur joueur4;

    joueur1.up = SDL_SCANCODE_W;
    joueur1.down = SDL_SCANCODE_S;
    joueur1.left = SDL_SCANCODE_A;
    joueur1.right = SDL_SCANCODE_D;

    joueur2.up = SDL_SCANCODE_UP;
    joueur2.down = SDL_SCANCODE_DOWN;
    joueur2.left = SDL_SCANCODE_LEFT;
    joueur2.right = SDL_SCANCODE_RIGHT;

    joueur3.up = SDL_SCANCODE_I;
    joueur3.down = SDL_SCANCODE_K;
    joueur3.left = SDL_SCANCODE_J;
    joueur3.right = SDL_SCANCODE_L;

    joueur4.up = SDL_SCANCODE_T;
    joueur4.down = SDL_SCANCODE_G;
    joueur4.left = SDL_SCANCODE_F;
    joueur4.right = SDL_SCANCODE_H;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    drawPlayer(&joueur1);
    drawPlayer(&joueur2);
    drawPlayer(&joueur3);
    drawPlayer(&joueur4);

    move_player(&joueur1, 100, 100, &joueur2);
    move_player(&joueur3, 200, 200, &joueur4);

    t_input input;
    initInput(&input, 200, 300);
    //  int collision = SDL_HasIntersection(&rect, &rect);
    while (!input.quit) {
        updateInput(&input);
        // collision = SDL_HasIntersection(&rect, &rect);

        handle_input(&joueur1, &joueur2, &input, window);
        handle_input(&joueur2, &joueur1, &input, window);
        handle_input(&joueur3, &joueur1, &input, window);
        handle_input(&joueur4, &joueur1, &input, window);

        SDL_RenderClear(renderer);
        dessin_rect(renderer, &joueur1.rect, 0, 255, 0, 255);
        dessin_rect(renderer, &joueur2.rect, 0, 0, 255, 255);
        dessin_rect(renderer, &joueur3.rect, 255, 0, 0, 255);
        dessin_rect(renderer, &joueur4.rect, 255, 255, 255, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}