// scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../debug.h"
#include "../io/input.h"
#include "fonctionManager.h"
#include "objectManager.h"

typedef enum {
    ON_LOAD = 0,
    RENDER = 1,
    UPDATE = 2,
    ON_EXIT = 3,
    NUM_FONCTION = 4
} FONCTION_TYPE;

typedef struct t_scene {
    t_objectManager* objectManager;
    t_fonctionParam*** fonctions;  // Pointeur sur fonction dans un tableau [NUM_FONCTION][on sais pas]
    int** nbFonctions;             // Nb de fonction le tableau [NUM_FONCTION][on sais pas]
    bool loaded;
    char* name;  // Debug
} t_scene;

void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, int typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...);

t_scene* createScene(char* name, t_objectManager* objectManager);

#endif