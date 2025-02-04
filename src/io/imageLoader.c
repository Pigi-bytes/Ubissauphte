#include "imageLoader.h"

SDL_Texture* loadImage(char* filename, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(filename);
    if (!surface) {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}