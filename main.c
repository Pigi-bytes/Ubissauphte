#include <SDL2/SDL.h>
#include <stdio.h>

#include "io/input.h"
#include "ui/button.h"
#include "utils/fscene.h"
#include "utils/objectManager.h"

void PlayClick() {
    printf("Play\n");
}

void OptionClick() {
    printf("Option\n");
}

void Quit() {
    printf("Quit\n");
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    t_objectManager* menu = initObjectManager(BUTTON_TYPE, freeButton);

    initTextEngine();
    TTF_Font* font = loadFont("../assets/fonts/JetBrainsMono-Regular.ttf", 16);

    ADD_TYPED_OBJECT(menu, BUTTON_TYPE, createButton(createTextOutline(renderer, "Play", font, (SDL_Color){0, 0, 0}, (SDL_Color){255, 255, 255}, 2), (SDL_Color){255, 0, 255}, (SDL_Color){0, 0, 255}, (SDL_Rect){100, 100, 250, 100}, PlayClick));
    ADD_TYPED_OBJECT(menu, BUTTON_TYPE, createButton(createTextOutline(renderer, "Option", font, (SDL_Color){0, 0, 0}, (SDL_Color){255, 255, 255}, 2), (SDL_Color){255, 0, 255}, (SDL_Color){0, 0, 255}, (SDL_Rect){100, 250, 250, 100}, OptionClick));
    ADD_TYPED_OBJECT(menu, BUTTON_TYPE, createButton(createTextOutline(renderer, "Quit ", font, (SDL_Color){0, 0, 0}, (SDL_Color){255, 255, 255}, 2), (SDL_Color){255, 0, 255}, (SDL_Color){0, 0, 255}, (SDL_Rect){100, 450, 250, 100}, Quit));

    t_input input;
    initInput(&input, 800, 600);
    while (!input.quit) {
        updateInput(&input);
        SDL_RenderPresent(renderer);
        call(scene->contenue[BUTTON_TYPE], HANDLEBUTTONCALL);
        call(scene->contenue[BUTTON_TYPE], RENDERBUTTONCALL);
        call(scene->contenue[TEXT_TYPE], DRAWTALLEXTCALL);
    }

    freeObjectManager(menu);
    return 0;
}
