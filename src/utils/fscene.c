#include "fscene.h"

#include <stdlib.h>

t_scene* createScene(t_objectManager* manager, char* name) {
    t_scene* scene = malloc(sizeof(t_scene));
    scene->name = strdup(name);
    scene->objectManager = manager;

    for (int i = 0; i < NUM_FONCTION; i++) {
        scene->fonctions[i] = NULL;
        scene->nbFonctions[i] = 0;
    }
    return scene;
}

void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, t_fonctionType typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...) {
    va_list args;
    va_start(args, indexObj);

    va_list count_args;
    va_copy(count_args, args);
    int param_count = 0;
    while (va_arg(count_args, void*) != NULL) param_count++;
    va_end(count_args);

    void** params = malloc(param_count * sizeof(void*));

    for (int i = 0; i < param_count; i++) {
        params[i] = va_arg(args, void*);
    }
    va_end(args);

    for (int i = 0; i < scene->objectManager->count; i++) {
        if (getObjectTypeId(scene->objectManager, i) == typeObject) {
            void** final_params = malloc((param_count + 1) * sizeof(void*));

            for (int j = 0; j < indexObj; j++) {
                final_params[j] = params[j];
            }
            final_params[indexObj] = getObject(scene->objectManager, i);
            for (int j = indexObj; j < param_count; j++) {
                final_params[j + 1] = params[j];
            }

            t_fonctionParam* sf = malloc(sizeof(t_fonctionParam));
            sf->fonction = fonct;
            sf->param = final_params;
            sf->nb_param = param_count + 1;

            scene->fonctions[typeFunction] = realloc(scene->fonctions[typeFunction], (scene->nbFonctions[typeFunction] + 1) * sizeof(t_fonctionParam*));
            scene->fonctions[typeFunction][scene->nbFonctions[typeFunction]++] = sf;
        }
    }

    free(params);
}

void executeSceneFunctions(t_scene* scene, t_fonctionType ftype) {
    for (int i = 0; i < scene->nbFonctions[ftype]; i++) {
        callFonction(scene->fonctions[ftype][i]);
    }
}

void freeScene(t_scene* scene) {
    freeObjectManager(scene->objectManager);
    for (int i = 0; i < NUM_FONCTION; i++) {
        for (int j = 0; j < scene->nbFonctions[i]; j++) {
            freeFonction(&(scene->fonctions[i][j]));
        }
        free(scene->fonctions[i]);
    }
    free(scene->name);
    free(scene);
}