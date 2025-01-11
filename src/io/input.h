#include <SDL2/SDL.h>

typedef struct {
    SDL_bool key[SDL_NUM_SCANCODES];
    SDL_bool quit;
    int x, y;
    int xrel, yrel;
    int xwheel, ywheel;
    SDL_bool mouse[6];
    int windowWidth;
    int windowHeight;
} t_input;

void updateEvent(t_input* input);
void initInput(t_input* input, int width, int height);