#include <SDL2/SDL.h>

SDL_FPoint pointSin(SDL_FPoint a);
float dot(SDL_FPoint a, SDL_FPoint b);
float floatFract(float val);
float randomFloat(SDL_FPoint st, SDL_FPoint seed);
SDL_FPoint pointFloor(SDL_FPoint st);
SDL_FPoint pointFract(SDL_FPoint st);
SDL_FPoint smoothStep(SDL_FPoint st);
float interpo(float a, float b, SDL_FPoint u);
float noise(SDL_FPoint st,SDL_FPoint seed);
