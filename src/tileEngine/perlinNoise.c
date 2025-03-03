#include "perlinNoise.h"

SDL_FPoint pointSin(SDL_FPoint a) {
    a.x = sin(a.x);
    a.y = sin(a.y);
    return a;
}

float dot(SDL_FPoint a, SDL_FPoint b) {
    return a.x * b.x + a.y * b.y;
}

float floatFract(float val) {
    return val - floor(val);
}

float randomFloat(SDL_FPoint st,SDL_FPoint seed) {
    // SDL_FPoint point = {12.9898, 78.233};
    return floatFract(sin(dot(st, seed)) * 43758.5453123);
}

SDL_FPoint pointFloor(SDL_FPoint st) {
    st.x = floor(st.x);
    st.y = floor(st.y);
    return st;
}

SDL_FPoint pointFract(SDL_FPoint st) {
    st.x = st.x - floor(st.x);
    st.y = st.y - floor(st.y);
    return st;
}

SDL_FPoint smoothStep(SDL_FPoint st) {
    st.x = st.x * st.x * (3.0 - 2.0 * st.x);
    st.y = st.y * st.y * (3.0 - 2.0 * st.y);
    return st;
}

float interpo(float a, float b, SDL_FPoint u) {
    return (a * (1 - u.x) + b * u.x);
}

float noise(SDL_FPoint st,SDL_FPoint seed) {
    SDL_FPoint i = pointFloor(st);
    SDL_FPoint f = pointFract(st);

    SDL_FPoint g = {i.x + 1.0, i.y};
    SDL_FPoint h = {i.x, i.y + 1.0};
    SDL_FPoint l = {i.x + 1, i.y + 1.0};

    float a = randomFloat(i, seed);
    float b = randomFloat(g, seed);
    float c = randomFloat(h, seed);
    float d = randomFloat(l, seed);

    SDL_FPoint u = smoothStep(f);

    return (interpo(a, b, u) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y);
}