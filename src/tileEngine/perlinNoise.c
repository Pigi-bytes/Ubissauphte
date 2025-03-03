#include "perlinNoise.h"

SDL_FPoint point_sin(SDL_FPoint a) {
    a.x = sin(a.x);
    a.y = sin(a.y);
    return a;
}

float dot(SDL_FPoint a, SDL_FPoint b) {
    return a.x * b.x + a.y * b.y;
}

float float_fract(float val) {
    return val - floor(val);
}

float random_float(SDL_FPoint st,SDL_FPoint seed) {
    // SDL_FPoint point = {12.9898, 78.233};
    return float_fract(sin(dot(st, seed)) * 43758.5453123);
}

SDL_FPoint point_floor(SDL_FPoint st) {
    st.x = floor(st.x);
    st.y = floor(st.y);
    return st;
}

SDL_FPoint point_fract(SDL_FPoint st) {
    st.x = st.x - floor(st.x);
    st.y = st.y - floor(st.y);
    return st;
}

SDL_FPoint smooth_step(SDL_FPoint st) {
    st.x = st.x * st.x * (3.0 - 2.0 * st.x);
    st.y = st.y * st.y * (3.0 - 2.0 * st.y);
    return st;
}

float interpo(float a, float b, SDL_FPoint u) {
    return (a * (1 - u.x) + b * u.x);
}

float noise(SDL_FPoint st,SDL_FPoint seed) {
    SDL_FPoint i = point_floor(st);
    SDL_FPoint f = point_fract(st);

    SDL_FPoint g = {i.x + 1.0, i.y};
    SDL_FPoint h = {i.x, i.y + 1.0};
    SDL_FPoint l = {i.x + 1, i.y + 1.0};

    float a = random_float(i, seed);
    float b = random_float(g, seed);
    float c = random_float(h, seed);
    float d = random_float(l, seed);

    SDL_FPoint u = smooth_step(f);

    return (interpo(a, b, u) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y);
}

void render_noise(SDL_Renderer *renderer, float  ** values,int nbLigne,int nbColonne) {
    float cell_width = (float)window_width / nbLigne;
    float cell_height = (float)window_height / nbColonne;

    for (int y = 0; y < nbLigne; ++y) {
        for (int x = 0; x < nbColonne; ++x) {
            float n = values[y][x];
            int color = (n * 255.0);  // si n est supérieur à 0.3 la couleur est noir sinon blanche plus c'est petit moins il y à de noir

            SDL_SetRenderDrawColor(renderer, color, color, color, 255);

            SDL_Rect rect = {
                (int)(x * cell_width),
                (int)(y * cell_height),
                (int)cell_width,
                (int)cell_height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}