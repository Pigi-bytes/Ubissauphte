// scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../debug.h"
#include "../io/input.h"
#include "fonctionManager.h"
#include "objectManager.h"

typedef struct t_scene {
    t_objectManager* objectManager;
    void (*updateFunctions[256])(t_fonctionParam*);
    void (*renderFunctions[256])(t_fonctionParam*);
    t_fonctionParam** updateParams;
    t_fonctionParam** renderParams;
    int objectCount;
    bool loaded;
    char* name;  // Debug
} t_scene;

typedef struct {
    t_scene* scene1;
    t_scene* scene2;
    t_scene* currentScene;
} t_sceneManager;

t_scene* createScene(t_objectManager* manager, char* name);
t_sceneManager* createSceneManager(t_scene* s1, t_scene* s2);
void switchScene(t_sceneManager* manager);

void sceneRegisterUpdateFunction(t_scene* scene, uint8_t typeId, void (*updateFunc)(t_fonctionParam*));
void sceneRegisterRenderFunction(t_scene* scene, uint8_t typeId, void (*renderFunc)(t_fonctionParam*));
void preloadScene(t_scene* scene, t_input* input, SDL_Renderer* renderer);
void sceneUpdate(t_scene* scene);
void sceneRender(t_scene* scene);
void freeScene(t_scene* scene);

#endif