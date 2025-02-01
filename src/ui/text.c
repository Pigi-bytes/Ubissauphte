#include "text.h"

TTF_Font* loadFont(char* filename, int size) {
    TTF_Font* font = TTF_OpenFont(filename, size);
    if (!font) {
        fprintf(stderr, "Erreur de chargement de la police %s : %s\n", filename, TTF_GetError());
    } else {
        DEBUG_PRINT("Police %s chargée avec succès à l'adresse : %p\n", filename, (void*)font);
    }
    return font;
}

SDL_bool initTextEngine() {
    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return SDL_FALSE;
    }
    DEBUG_PRINT("SDL_ttf initialisé avec succès.\n");
    return SDL_TRUE;
}

t_text createTextOutline(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color, SDL_Color colorOutline, int outlineSize) {
    t_text label;

    label.text = malloc(strlen(text) + 1);
    if (label.text == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le texte.\n");
        label.texture = NULL;
        return label;
    }

    strcpy(label.text, text);
    label.font = font;

    TTF_SetFontOutline(font, outlineSize);
    SDL_Surface* surfaceOutline = TTF_RenderUTF8_Blended(font, text, colorOutline);
    if (!surfaceOutline) {
        fprintf(stderr, "Erreur de création de surface pour le contour : %s\n", TTF_GetError());
        free(label.text);
        label.texture = NULL;
        return label;
    }

    TTF_SetFontOutline(font, 0);
    SDL_Surface* surfaceText = TTF_RenderUTF8_Blended(font, text, color);
    if (!surfaceText) {
        fprintf(stderr, "Erreur de création de surface pour le texte principal : %s\n", TTF_GetError());
        SDL_FreeSurface(surfaceOutline);
        free(label.text);
        label.texture = NULL;
        return label;
    }

    SDL_Surface* combinedSurface = SDL_CreateRGBSurfaceWithFormat(0, surfaceOutline->w, surfaceOutline->h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!combinedSurface) {
        fprintf(stderr, "Erreur de création de surface combinée : %s\n", SDL_GetError());
        SDL_FreeSurface(surfaceOutline);
        SDL_FreeSurface(surfaceText);
        free(label.text);
        label.texture = NULL;
        return label;
    }

    SDL_BlitSurface(surfaceOutline, NULL, combinedSurface, NULL);
    SDL_Rect textPosition = {outlineSize, outlineSize, surfaceText->w, surfaceText->h};
    SDL_BlitSurface(surfaceText, NULL, combinedSurface, &textPosition);

    label.texture = SDL_CreateTextureFromSurface(renderer, combinedSurface);
    if (!label.texture) {
        fprintf(stderr, "Erreur de création de texture combinée : %s\n", SDL_GetError());
        SDL_FreeSurface(surfaceOutline);
        SDL_FreeSurface(surfaceText);
        SDL_FreeSurface(combinedSurface);
        free(label.text);
        label.texture = NULL;
        return label;
    }

    label.rect.w = combinedSurface->w;
    label.rect.h = combinedSurface->h;
    label.rect.x = 0;
    label.rect.y = 0;

    SDL_FreeSurface(surfaceOutline);
    SDL_FreeSurface(surfaceText);
    SDL_FreeSurface(combinedSurface);

    return label;
}

t_text createText(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color) {
    t_text label;

    label.text = malloc(strlen(text) + 1);
    if (label.text == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le texte.\n");
        label.texture = NULL;
        return label;
    }

    strcpy(label.text, text);
    label.font = font;

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text, color);
    if (!surface) {
        fprintf(stderr, "Erreur de création de surface du texte \"%s\" : %s\n", text, TTF_GetError());
        free(label.text);
        label.texture = NULL;
        return label;
    }

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
    DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, text->rect, 3);
}

void drawAllText(t_objectManager* manager, SDL_Renderer* renderer) {
    for (int i = 0; i < manager->count; ++i) {
        t_text* text = (t_text*)manager->items[i]->data;
        drawText(renderer, text);
    }
}

void drawallTextcall(t_fonctionParam* fonction) {
    t_objectManager* manager = (t_objectManager*)fonction->param[0];
    SDL_Renderer* renderer = (SDL_Renderer*)fonction->param[1];
    drawAllText(manager, renderer);
}

void freeText(t_text* text) {
    if (text->text != NULL) {
        free(text->text);
    }
    if (text->texture != NULL) {
        SDL_DestroyTexture(text->texture);
    }
}
