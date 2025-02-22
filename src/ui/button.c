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
    button->OnClick = OnClick;
    button->label = text;

    text->rect.x = button->rect.x + (button->rect.w - text->rect.w) / 2;
    text->rect.y = button->rect.y + (button->rect.h - text->rect.h) / 2;

    return button;
}

void renderButton(SDL_Renderer* renderer, t_button* button) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);
    renderText(renderer, button->label);

    DEBUG_DRAW_RECTANGLE_WITH_WIDTH(renderer, button->rect, 3);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

SDL_bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect) {
    if (mouseX >= rect->x && mouseX <= rect->x + rect->w &&
        mouseY >= rect->y && mouseY <= rect->y + rect->h) {
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

void handleButtonClick(t_input* input, t_button* button) {
    int newWidth = button->rectDefault.w * SCALE_FACTOR;
    int newHeight = button->rectDefault.h * SCALE_FACTOR;
    int deltaX = (newWidth - button->rectDefault.w) / 2;
    int deltaY = (newHeight - button->rectDefault.h) / 2;

    if (isMouseInsideRect(input->mouseX, input->mouseY, &button->rect)) {
        button->rect.w = newWidth;
        button->rect.h = newHeight;
        button->rect.x = button->rectDefault.x - deltaX;
        button->rect.y = button->rectDefault.y - deltaY;

        if (input->mouseButtons[SDL_BUTTON_LEFT]) {
            if (!button->isClicked) {
                button->isClicked = SDL_TRUE;
                button->color = button->colorOnClick;
                if (button->OnClick) {
                    callFonction(button->OnClick);
                }
            }
        } else if (button->isClicked) {
            button->isClicked = SDL_FALSE;
            button->color = button->colorDefault;
        }
    } else {
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