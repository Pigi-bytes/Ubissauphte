#include "general.h"

SDL_FPoint point_sin(SDL_FPoint a);
float dot(SDL_FPoint a, SDL_FPoint b);
float float_fract(float val);
float random_float(SDL_FPoint st, SDL_FPoint seed);
SDL_FPoint point_floor(SDL_FPoint st);
SDL_FPoint point_fract(SDL_FPoint st);
SDL_FPoint smooth_step(SDL_FPoint st);
float interpo(float a, float b, SDL_FPoint u);
float noise(SDL_FPoint st,SDL_FPoint seed);
void render_noise(SDL_Renderer *renderer, float **values, int nbLigne, int nbColonne);
