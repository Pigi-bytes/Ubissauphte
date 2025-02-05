#include "button.h"

#include <stdarg.h>

const float scaleFactor = 1.1f;

t_button* createButton(t_text text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick) {
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

    button->label.rect.x = button->rect.x + (button->rect.w - button->label.rect.w) / 2;
    button->label.rect.y = button->rect.y + (button->rect.h - button->label.rect.h) / 2;

    return button;
}

void renderButton(SDL_Renderer* renderer, t_button* button) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);
    drawText(renderer, &button->label);

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
    int newWidth = button->rectDefault.w * scaleFactor;
    int newHeight = button->rectDefault.h * scaleFactor;
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
                    callSolo(button->OnClick);
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
    freeFonctionParam(&(button->OnClick));
}

void renderAllButton(t_objectManager* manager, SDL_Renderer* renderer) {
    for (int i = 0; i < manager->count; ++i) {
        t_button* button = (t_button*)manager->items[i]->data;
        renderButton(renderer, button);
    }
}

void handleAllButtonInput(t_objectManager* manager, t_input* input) {
    for (int i = 0; i < manager->count; ++i) {
        t_button* button = (t_button*)manager->items[i]->data;
        handleButtonClick(input, button);
    }
}

// fonction qui lie la fonction présent dans le fonction manager à la fonction initial
void handleAllButtonInputCall(t_fonctionParam* fonction) {
    t_objectManager* manager = (t_objectManager*)fonction->param[0];
    t_input* input = (t_input*)fonction->param[1];
    handleAllButtonInput(manager, input);
}

void renderAllButtonCall(t_fonctionParam* fonction) {
    t_objectManager* manager = (t_objectManager*)fonction->param[0];
    SDL_Renderer* render = (SDL_Renderer*)fonction->param[1];
    renderAllButton(manager, render);
}
