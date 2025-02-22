// scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../debug.h"
#include "../io/input.h"
#include "fonctionManager.h"
#include "objectManager.h"

#define ADD_OBJECT_TO_SCENE(scene, obj, type) addObject(scene->objectManager, obj, type)

typedef enum {
    RENDER_GAME,
    RENDER_UI,
    HANDLE_INPUT,
    UPDATE,
    SET_BUFFER,
    RENDER_BUFFER,
    NUM_FONCTION
} t_fonctionType;

typedef struct t_scene {
    char* name;
    t_objectManager* objectManager;
    t_fonctionParam** fonctions[NUM_FONCTION];
    int nbFonctions[NUM_FONCTION];
} t_scene;

t_scene* createScene(t_objectManager* objectManager, char* name);
void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, t_fonctionType typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...);
void executeSceneFunctions(t_scene* scene, t_fonctionType ftype);
void freeScene(t_scene* scene);

#endif