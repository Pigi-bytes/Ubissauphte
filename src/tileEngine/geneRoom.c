#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../io/input.h"

#define WIDTH 400
#define HEIGHT 400

#define window_width 1280
#define window_height 960


SDL_FPoint point_sin(SDL_FPoint a){
    a.x = sin(a.x);
    a.y = sin(a.y);
    return a;
}

float dot(SDL_FPoint a,SDL_FPoint b){
    return a.x*b.x + a.y*b.y;
}

float float_fract(float val){
    return val - floor(val);
}

float random_float(SDL_FPoint st){
    SDL_FPoint point={12.9898,78.233};
    return float_fract(sin(dot(st,point))*43758.5453123);
    
}


SDL_FPoint point_floor(SDL_FPoint st){
    st.x = floor(st.x);
    st.y = floor(st.y);
    return st;
}



SDL_FPoint point_fract(SDL_FPoint st){
    st.x = st.x - floor(st.x);
    st.y = st.y - floor(st.y);
    return st;
}

SDL_FPoint smooth_step(SDL_FPoint st){
    st.x = st.x * st.x *(3.0-2.0*st.x);
    st.y = st.y * st.y *(3.0-2.0*st.y);
    return st;
}

float interpo(float a,float b,SDL_FPoint u){
    return (a * (1 - u.x) + b * u.x);
}

float noise(SDL_FPoint st){
    SDL_FPoint i = point_floor(st);
    SDL_FPoint f = point_fract(st);

    SDL_FPoint g = {i.x +1.0,i.y};
    SDL_FPoint h = {i.x,i.y+1.0};
    SDL_FPoint l = {i.x+1,i.y+1.0};



    float a = random_float(i);
    float b = random_float(g);
    float c = random_float(h);
    float d = random_float(l);

    SDL_FPoint u = smooth_step(f);

    return (interpo(a,b,u) + (c-a) * u.y * (1.0-u.x) + (d-b)*u.x * u.y);
}

void render_noise(SDL_Renderer *renderer, float values[HEIGHT][WIDTH]) {
    float cell_width = (float)window_width / WIDTH;
    float cell_height = (float)window_height / HEIGHT;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {

            float n = values[y][x];
            int color = (int)((n>0.3) * 255.0); //si n est supérieur à 0.3 la couleur est noir sinon blanche
            
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            
           
            SDL_Rect rect = {
                (int)(x * cell_width),   
                (int)(y * cell_height), 
                (int)cell_width,       
                (int)cell_height        
            };
            SDL_RenderFillRect(renderer, &rect);  
        }
    }
}


int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Bruit de Perlin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);


    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    float values[HEIGHT][WIDTH];

    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            SDL_FPoint st ={(float)i/6,(float)j/6}; //plus le dénominateur est petit plus il y à de murs
            SDL_FPoint st2 ={(float)i/3,(float)j/3};
            SDL_FPoint st3 ={(float)i/2,(float)j/2};
            values[i][j] = noise(st);
            values[i][j] = (values[i][j] + noise(st2))/2.0f;
            values[i][j] = (values[i][j] + noise(st3))/2.0f;
        }
    }


    t_input input;
    initInput(&input, window_width, window_height);
    while (!input.quit) {
        updateInput(&input);


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        render_noise(renderer, values);
        SDL_RenderPresent(renderer);
    }



    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}
