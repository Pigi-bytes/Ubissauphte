#include "button.h"

t_button* createButton(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick) {
    t_button* button = malloc(sizeof(t_button));

    if (!button) {
        return NULL;
    }

    button->rect = rect;
    button->rectDefault = rect;
    button->color = color;
    button->colorDefault = color;
    button->colorOnClick = colorOnClick;
    button->isClicked = SDL_FALSE;
    button->isHovered = SDL_FALSE;
    button->OnClick = OnClick;
    button->label = text;

    text->rect.x = button->rect.x + (button->rect.w - text->rect.w) / 2;
    text->rect.y = button->rect.y + (button->rect.h - text->rect.h) / 2;

    return button;
}

#include <math.h>

void renderRoundedRect(SDL_Renderer* renderer, SDL_Rect* rect, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect innerRect = {rect->x + radius, rect->y + radius, rect->w - 2 * radius, rect->h - 2 * radius};
    SDL_RenderFillRect(renderer, &innerRect);

    // Dessiner les côtés
    SDL_Rect topRect = {rect->x + radius, rect->y, rect->w - 2 * radius, radius};
    SDL_Rect bottomRect = {rect->x + radius, rect->y + rect->h - radius, rect->w - 2 * radius, radius};
    SDL_Rect leftRect = {rect->x, rect->y + radius, radius, rect->h - 2 * radius};
    SDL_Rect rightRect = {rect->x + rect->w - radius, rect->y + radius, radius, rect->h - 2 * radius};
    SDL_RenderFillRect(renderer, &topRect);
    SDL_RenderFillRect(renderer, &bottomRect);
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_RenderFillRect(renderer, &rightRect);

    for (int w = 0; w < radius; w++) {
        for (int h = 0; h < radius; h++) {
            if ((w * w + h * h) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, rect->x + radius - w, rect->y + radius - h);                      // Coin supérieur gauche
                SDL_RenderDrawPoint(renderer, rect->x + rect->w - radius + w, rect->y + radius - h);            // Coin supérieur droit
                SDL_RenderDrawPoint(renderer, rect->x + radius - w, rect->y + rect->h - radius + h);            // Coin inférieur gauche
                SDL_RenderDrawPoint(renderer, rect->x + rect->w - radius + w, rect->y + rect->h - radius + h);  // Coin inférieur droit
            }
        }
    }
}

void renderButton(t_context* ctx, t_button* button) {
    int cornerRadius = 10;  // Rayon des coins arrondis
    renderRoundedRect(ctx->renderer, &button->rect, cornerRadius, button->color);
    renderText(ctx->renderer, button->label);

    Debug_PushRect(&button->rect, 3, COLORDEFAULT);

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
}

SDL_bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect) {
    if (mouseX >= rect->x && mouseX <= rect->x + rect->w &&
        mouseY >= rect->y && mouseY <= rect->y + rect->h) {
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

void handleInputButton(t_context* ctx, t_button* button) {
    int newWidth = button->rectDefault.w * SCALE_FACTOR;
    int newHeight = button->rectDefault.h * SCALE_FACTOR;
    int deltaX = (newWidth - button->rectDefault.w) / 2;
    int deltaY = (newHeight - button->rectDefault.h) / 2;

    if (isMouseInsideRect(ctx->input->mouseX, ctx->input->mouseY, &button->rect)) {
        if (!button->isHovered) {
            button->isHovered = SDL_TRUE;                                          // Marquer comme survolé
            jouerSFX("assets/sound/onHoverButton.wav", 25, 0, ctx->audioManager);  // Jouer le son de survol
        }

        button->rect.w = newWidth;
        button->rect.h = newHeight;
        button->rect.x = button->rectDefault.x - deltaX;
        button->rect.y = button->rectDefault.y - deltaY;

        if (ctx->input->mouseButtons[SDL_BUTTON_LEFT]) {
            if (!button->isClicked) {
                button->isClicked = SDL_TRUE;
                button->color = button->colorOnClick;
            }
        } else if (button->isClicked) {
            button->isClicked = SDL_FALSE;
            button->color = button->colorDefault;
            if (button->OnClick) {
                jouerSFX("assets/sound/clickButton.wav", 100, 0, ctx->audioManager);
                callFonction(button->OnClick);
            }
        }
    } else {
        button->isHovered = SDL_FALSE;

        button->rect = button->rectDefault;
        if (button->isClicked) {
            button->isClicked = SDL_FALSE;
            button->color = button->colorDefault;
        }
    }
}

void freeButton(void* object) {
    t_button* button = (t_button*)object;
    freeText(&(button->label));
    free(button);
    freeFonction(&(button->OnClick));
}