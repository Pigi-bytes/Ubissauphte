#include "debug.h"

#include <SDL2/SDL.h>

#if DEBUG_MODE

typedef enum { DEBUG_RECT,
               DEBUG_CIRCLE,
               DEBUG_LINE } DebugCommandType;

struct DebugCommand {
    DebugCommandType type;
    SDL_Color color;
    union {
        struct {
            SDL_Rect rect;
            int border;
        };
        struct {
            int x, y, radius;
        };
        struct {
            int x1, y1, x2, y2;  // Coordonnées des deux points de la ligne
            int lineWidth;       // Largeur de la ligne
        };
    };
};

#define MAX_DEBUG_COMMANDS 10000

static DebugCommand command_stack[MAX_DEBUG_COMMANDS];
static int command_stack_index = 0;
static SDL_Renderer* renderer = NULL;

void Debug_Init(SDL_Renderer* r) {
    renderer = r;
}
void Debug_PushRect(const SDL_Rect* rect, int border_width) {
    if (command_stack_index < MAX_DEBUG_COMMANDS) {
        DebugCommand* cmd = &command_stack[command_stack_index++];
        *cmd = (DebugCommand){
            .type = DEBUG_RECT,
            .color = RECTANGLE_COLOR,
            .rect = *rect,
            .border = border_width};
    }
}

// Pousser un cercle dans la pile des commandes
void Debug_PushCircle(int x, int y, int radius) {
    if (command_stack_index < MAX_DEBUG_COMMANDS) {
        DebugCommand* cmd = &command_stack[command_stack_index++];
        *cmd = (DebugCommand){
            .type = DEBUG_CIRCLE,
            .color = CIRCLE_COLOR,
            .x = x,
            .y = y,
            .radius = radius};
    }
}

void Debug_PushLine(int x1, int y1, int x2, int y2, SDL_Color color, int lineWidth) {
    if (command_stack_index < MAX_DEBUG_COMMANDS) {
        DebugCommand* cmd = &command_stack[command_stack_index++];
        *cmd = (DebugCommand){
            .type = DEBUG_LINE,
            .color = color,
            .x1 = x1,
            .y1 = y1,
            .x2 = x2,
            .y2 = y2,
            .lineWidth = lineWidth};
    }
}

void DrawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int width) {
    float angle = atan2f(y2 - y1, x2 - x1);
    float offsetX = cosf(angle + 3.14) * width / 2;
    float offsetY = sinf(angle + 3.14) * width / 2;

    for (int i = -width / 2; i <= width / 2; i++) {
        int newX1 = x1 + offsetX * i;
        int newY1 = y1 + offsetY * i;
        int newX2 = x2 + offsetX * i;
        int newY2 = y2 + offsetY * i;

        SDL_RenderDrawLine(renderer, newX1, newY1, newX2, newY2);
    }
}

void Debug_RenderAll(void) {
    SDL_Color original;
    SDL_GetRenderDrawColor(renderer, &original.r, &original.g, &original.b, &original.a);

    for (int i = 0; i < command_stack_index; i++) {
        DebugCommand* current = &command_stack[i];

        SDL_SetRenderDrawColor(renderer,
                               current->color.r,
                               current->color.g,
                               current->color.b,
                               current->color.a);

        switch (current->type) {
            case DEBUG_RECT:
                for (int i = 0; i < current->border; i++) {
                    SDL_Rect r = {
                        current->rect.x - i,
                        current->rect.y - i,
                        current->rect.w + 2 * i,
                        current->rect.h + 2 * i};
                    SDL_RenderDrawRect(renderer, &r);
                }
                break;

            case DEBUG_CIRCLE:
                for (int w = -current->radius; w <= current->radius; w++) {
                    for (int h = -current->radius; h <= current->radius; h++) {
                        if (w * w + h * h <= current->radius * current->radius) {
                            SDL_RenderDrawPoint(renderer,
                                                current->x + w,
                                                current->y + h);
                        }
                    }
                }
                break;

            case DEBUG_LINE:
                if (current->lineWidth > 1) {
                    DrawThickLine(renderer, current->x1, current->y1, current->x2, current->y2, current->lineWidth);
                } else {
                    SDL_RenderDrawLine(renderer, current->x1, current->y1, current->x2, current->y2);
                }
                break;
        }
    }

    SDL_SetRenderDrawColor(renderer, original.r, original.g, original.b, original.a);

    Debug_ClearStack();
}

// Réinitialiser la pile de commandes
void Debug_ClearStack(void) {
    command_stack_index = 0;
}

#endif
