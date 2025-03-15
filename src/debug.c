#include "debug.h"

#include <SDL2/SDL.h>
#include <math.h>  // Pour sin() et cos()

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

static SDL_Color DEFAULT_COLOR_RECT = {41, 182, 246, 255};  // Rouge par défaut pour le rectangle
static SDL_Color DEFAULT_COLOR_CIRCLE = {255, 0, 0, 255};   // Vert par défaut pour le cercle
static SDL_Color DEFAULT_COLOR_LINE = {0, 0, 255, 255};     // Bleu par défaut pour la ligne

void Debug_Init(SDL_Renderer* r) {
    renderer = r;
}

void Debug_PushRect(const SDL_Rect* rect, int border_width, SDL_Color color) {
    if (command_stack_index < MAX_DEBUG_COMMANDS) {
        if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
            color = DEFAULT_COLOR_RECT;  // Utilisation de la couleur par défaut si aucune couleur n'est donnée
        }
        DebugCommand* cmd = &command_stack[command_stack_index++];
        *cmd = (DebugCommand){
            .type = DEBUG_RECT,
            .color = color,
            .rect = *rect,
            .border = border_width};
    }
}

// Pousser un cercle vide dans la pile des commandes avec la possibilité de spécifier une couleur
void Debug_PushCircle(int x, int y, int radius, SDL_Color color) {
    if (command_stack_index < MAX_DEBUG_COMMANDS) {
        if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
            color = DEFAULT_COLOR_CIRCLE;  // Utilisation de la couleur par défaut si aucune couleur n'est donnée
        }
        DebugCommand* cmd = &command_stack[command_stack_index++];
        *cmd = (DebugCommand){
            .type = DEBUG_CIRCLE,
            .color = color,
            .x = x,
            .y = y,
            .radius = radius};
    }
}

// Pousser une ligne dans la pile des commandes avec la possibilité de spécifier une couleur
void Debug_PushLine(int x1, int y1, int x2, int y2, int lineWidth, SDL_Color color) {
    if (command_stack_index < MAX_DEBUG_COMMANDS) {
        if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
            color = DEFAULT_COLOR_LINE;  // Utilisation de la couleur par défaut si aucune couleur n'est donnée
        }
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

// Fonction pour dessiner un cercle vide en utilisant sin et cos
void Debug_RenderCircle(int x, int y, int radius) {
    const int numSegments = 360 * 2;                         // 360 points pour un cercle
    const float angleIncrement = 2 * 3.14159 / numSegments;  // Incrément d'angle pour chaque segment

    // Dessiner chaque segment du cercle
    for (int i = 0; i < numSegments; i++) {
        float angle = i * angleIncrement;    // Calculer l'angle pour ce point
        int xPos = x + radius * cos(angle);  // Calculer la coordonnée x
        int yPos = y + radius * sin(angle);  // Calculer la coordonnée y

        SDL_RenderDrawPoint(renderer, xPos, yPos);  // Dessiner le point
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
                Debug_RenderCircle(current->x, current->y, current->radius);
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
