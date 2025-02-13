#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef struct {
    int key[SDL_NUM_SCANCODES];
    SDL_bool quit;
    SDL_bool resized;
    int mouseX, mouseY;
    int mouseXWheel, mouseYWheel;
    SDL_bool mouseButtons[6];
    int windowWidth;
    int windowHeight;
} t_input;

void updateInput(t_input* input);
void initInput(t_input* input, int windowWidth, int windowHeight);

SDL_bool keyPressOnce(t_input* input, SDL_Scancode scancode);

#endif  // INPUT_H
