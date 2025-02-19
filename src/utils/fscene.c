#include "fscene.h"

#include <stdlib.h>

t_scene* createScene(t_objectManager* manager, char* name) {
    t_scene* scene = (t_scene*)malloc(sizeof(t_scene));
    scene->objectManager = manager;
    scene->updateParams = NULL;
    scene->renderParams = NULL;
    scene->objectCount = 0;
    scene->loaded = false;

    scene->name = name;

    for (int i = 0; i < 256; i++) {
        scene->updateFunctions[i] = NULL;
        scene->renderFunctions[i] = NULL;
    }
    return scene;
}

void sceneRegisterUpdateFunction(t_scene* scene, uint8_t typeId, void (*updateFunc)(t_fonctionParam*)) {
    if (typeId < 256) scene->updateFunctions[typeId] = updateFunc;
}

void sceneRegisterRenderFunction(t_scene* scene, uint8_t typeId, void (*renderFunc)(t_fonctionParam*)) {
    if (typeId < 256) scene->renderFunctions[typeId] = renderFunc;
}

void preloadScene(t_scene* scene, t_input* input, SDL_Renderer* renderer) {
    if (scene->loaded) return;

    t_objectManager* manager = scene->objectManager;
    scene->objectCount = manager->count;
    scene->updateParams = malloc(scene->objectCount * sizeof(t_fonctionParam*));
    scene->renderParams = malloc(scene->objectCount * sizeof(t_fonctionParam*));

    int index = 0;
    t_objectMemoryPool* pool = manager->firstPool;
    while (pool && index < scene->objectCount) {
        int itemsInPool = (pool == manager->currentPool) ? manager->nbItemsInPool : POOL_SIZE;
        for (int i = 0; i < itemsInPool && index < scene->objectCount; i++, index++) {
            t_typedObject* obj = &pool->items[i];
            uint8_t typeId = obj->typeId;

            if (scene->updateFunctions[typeId]) {
                scene->updateParams[index] = creerFonction(scene->updateFunctions[typeId], input, obj->data, NULL);
            } else {
                scene->updateParams[index] = NULL;
            }

            if (scene->renderFunctions[typeId]) {
                scene->renderParams[index] = creerFonction(scene->renderFunctions[typeId], obj->data, renderer, NULL);
            } else {
                scene->renderParams[index] = NULL;
            }
        }
        pool = pool->next;
    }
    scene->loaded = true;
}

void sceneUpdate(t_scene* scene) {
    if (!scene->loaded) return;

    for (int i = 0; i < scene->objectCount; i++) {
        if (scene->updateParams[i]) {
            callFonction(scene->updateParams[i]);
        }
    }
}

void sceneRender(t_scene* scene) {
    if (!scene->loaded) return;

    for (int i = 0; i < scene->objectCount; i++) {
        if (scene->renderParams[i]) {
            callFonction(scene->renderParams[i]);
        }
    }
}

void freeScene(t_scene* scene) {
    if (scene->loaded) {
        for (int i = 0; i < scene->objectCount; i++) {
            if (scene->updateParams[i]) freeFonction(&scene->updateParams[i]);
            if (scene->renderParams[i]) freeFonction(&scene->renderParams[i]);
        }
        free(scene->updateParams);
        free(scene->renderParams);
    }
    free(scene);
}

t_sceneManager* createSceneManager(t_scene* s1, t_scene* s2) {
    t_sceneManager* sm = (t_sceneManager*)malloc(sizeof(t_sceneManager));
    sm->scene1 = s1;
    sm->scene2 = s2;
    sm->currentScene = s1;  // Démarre avec la première scène
    return sm;
}

void switchScene(t_sceneManager* manager) {
    if (manager->currentScene == manager->scene1) {
        manager->currentScene = manager->scene2;
    } else {
        manager->currentScene = manager->scene1;
    }
}