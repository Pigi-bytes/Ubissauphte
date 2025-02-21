#include "input.h"

void updateInput(t_input* input) {
    SDL_Event event;
    input->resized = SDL_FALSE;  // Réinitialiser à chaque frame
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            input->quit = SDL_TRUE;
        else if (event.type == SDL_KEYDOWN) {
            input->key[event.key.keysym.scancode] = SDL_TRUE;
        } else if (event.type == SDL_KEYUP) {
            input->key[event.key.keysym.scancode] = SDL_FALSE;
        } else if (event.type == SDL_MOUSEMOTION) {
            input->mouseX = event.motion.x;
            input->mouseY = event.motion.y;
        } else if (event.type == SDL_MOUSEWHEEL) {
            input->mouseXWheel = event.wheel.x;
            input->mouseYWheel = event.wheel.y;
        } else if (event.type == SDL_MOUSEBUTTONDOWN)
            input->mouseButtons[event.button.button] = SDL_TRUE;
        else if (event.type == SDL_MOUSEBUTTONUP)
            input->mouseButtons[event.button.button] = SDL_FALSE;
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                input->windowWidth = event.window.data1;   // Mise à jour de la largeur
                input->windowHeight = event.window.data2;  // Mise à jour de la hauteur
                input->resized = SDL_TRUE;                 // Mise à jour du flag
            }
        }
    }
}

t_input* initInput(int windowWidth, int windowHeight) {
    t_input* input = malloc(sizeof(t_input));
    memset(input->key, SDL_FALSE, sizeof(input->key));
    input->quit = SDL_FALSE;
    input->mouseX = 0;
    input->mouseY = 0;
    input->mouseXWheel = 0;
    input->mouseYWheel = 0;
    memset(input->mouseButtons, SDL_FALSE, sizeof(input->mouseButtons));
    input->windowWidth = windowWidth;
    input->windowHeight = windowHeight;
    return input;
}

SDL_bool keyPressOnce(t_input* input, SDL_Scancode scancode) {
    // static -> Reste la meme entre chaque appel de fonction (crack broken, on met les touches ou on reste appuyé dedans)
    static SDL_bool keyProcessed[SDL_NUM_SCANCODES] = {SDL_FALSE};

    if (input->key[scancode] && !keyProcessed[scancode]) {
        keyProcessed[scancode] = SDL_TRUE;
        return SDL_TRUE;
    } else if (!input->key[scancode] && keyProcessed[scancode]) {
        keyProcessed[scancode] = SDL_FALSE;
    }
    return SDL_FALSE;
}

void freeInput(t_input* input) {
    if (input != NULL) {
        free(input);
    }
}