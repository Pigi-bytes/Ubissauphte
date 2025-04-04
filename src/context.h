#ifndef CONTEXT_H
#define CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <dirent.h>
#include <stdio.h>

#include "engine/core/frame.h"
#include "engine/entities/components/equipment/equipementsManager.h"
#include "engine/world/genmap.h"
#include "io/audioManager.h"
#include "io/input.h"
#include "ui/fpsDisplay.h"
#include "utils/fscene.h"

typedef enum {
    EASY,
    NORMAL,
    HARD,
    DEMONIC,
    LEGEND,
} difficulty;

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
    SDL_Scancode dash;
    SDL_Scancode escape;
    SDL_Scancode map;
    SDL_Scancode interact;
    SDL_Scancode inventaire;
    SDL_Scancode activable1;
} t_control;

typedef struct {
    SDL_bool PleinEcran;
    SDL_bool FlagCommande;
    int fps;
} t_option;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    t_input* input;
    t_control* control;
    TTF_Font* font;
    TTF_Font* gameFont;
    t_audiomanager* audioManager;
    t_sceneController* sceneController;
    t_frameData* frameData;
    t_option* option;
    t_fpsDisplay* fpsDisplay;
    t_level* currentLevel;
    int nbLevel;
    difficulty difficulty;
    t_item** itemListe;
    int nbItem;
} t_context;

t_option* creeOption();
void freeOption(void* elt);

#endif