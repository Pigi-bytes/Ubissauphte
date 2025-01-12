#include <SDL2/SDL.h>

#define RESET 0
#define RELACH 2
#define PRESS 1

typedef struct {
    int key[SDL_NUM_SCANCODES+1]; 
    SDL_bool quit;                 
    int x, y;                       
    int buffer;
    int xrel, yrel;                  
    int xwheel, ywheel;              
    SDL_bool mouse[6];               
    int windowWidth;                
    int windowHeight;                
} t_input;

void updateEvent(t_input* input);
void initInput(t_input* input, int width, int height);