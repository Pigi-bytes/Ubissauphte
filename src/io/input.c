#include "input.h"

void updateEvent(t_input* input) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            input->quit = SDL_TRUE;
        else if (event.type == SDL_KEYDOWN) {
            input->key[event.key.keysym.scancode] = PRESS;
        } else if (event.type == SDL_KEYUP) {
            input->key[event.key.keysym.scancode] = input->key[event.key.keysym.scancode] == PRESS ? RELACH : RESET;
        }else if (event.type == SDL_MOUSEMOTION) {
            input->x = event.motion.x;
            input->y = event.motion.y;
            input->xrel = event.motion.xrel;
            input->yrel = event.motion.yrel;
        } else if (event.type == SDL_MOUSEWHEEL) {
            input->xwheel = event.wheel.x;
            input->ywheel = event.wheel.y;
        } else if (event.type == SDL_MOUSEBUTTONDOWN)
            input->mouse[event.button.button] = SDL_TRUE;
        else if (event.type == SDL_MOUSEBUTTONUP)
            input->mouse[event.button.button] = SDL_FALSE;
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                input->windowWidth = event.window.data1;   // Mise à jour de la largeur
                input->windowHeight = event.window.data2;  // Mise à jour de la hauteur
            }
        }
    }
}

void initInput(t_input* input, int width, int height) {
    // Initialiser tous les champs à 0
    memset(input->key, SDL_FALSE, sizeof(input->key));  // SDL_FALSE est 0
    input->quit = SDL_FALSE;
    input->x = 0;
    input->y = 0;
    input->xrel = 0;
    input->yrel = 0;
    input->xwheel = 0;
    input->ywheel = 0;
    memset(input->mouse, SDL_FALSE, sizeof(input->mouse));  // SDL_FALSE pour les boutons souris
    input->windowWidth = width;
    input->windowHeight = height;
}
