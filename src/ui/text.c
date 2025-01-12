#include "text.h"

TTF_Font* loadFont(char* filename, int size) {
    TTF_Font* font = TTF_OpenFont(filename, size);
    if (!font) {
        fprintf(stderr, "Erreur de chargement de la police %s : %s\n", filename, TTF_GetError());
    }
    return font;
}

SDL_bool initTextEngine() {
    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return SDL_FALSE;
    }
    return SDL_TRUE;
}

t_text createText(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color) {
    t_text label;
    label.text = malloc(strlen(text) + 1);  // Allocation dynamique de la chaîne
    if (label.text == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le texte.\n");
        label.texture = NULL;
        return label;
    }

    strcpy(label.text, text);

    label.font = font;
    label.color = color;

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text, color);
    if (!surface) {
        fprintf(stderr, "Erreur de création de surface du texte \"%s\" : %s\n", text, TTF_GetError());
        free(label.text);
        label.texture = NULL;
        return label;
    }

    // Créer la texture du texte principal
    label.rect = (SDL_Rect){0, 0, surface->w, surface->h};
    label.texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!label.texture) {
        fprintf(stderr, "Erreur de création de texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(label.text);
        label.texture = NULL;
        return label;
    }

    SDL_FreeSurface(surface);  // Libérer la surface après avoir créé la texture
    return label;
}

void drawText(SDL_Renderer* renderer, t_text* text) {
    SDL_RenderCopy(renderer, text->texture, NULL, &text->rect);
}

void freeText(t_text* text) {
    if (text->text != NULL) {
        free(text->text);
    }
    if (text->texture != NULL) {
        SDL_DestroyTexture(text->texture);
    }
}