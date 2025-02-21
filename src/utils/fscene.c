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

uint8_t getObjectTypeId(t_objectManager* manager, int index) {
    if (index >= manager->count) {
        return -1;  // Index hors limites
    }

    int poolIndex = index / POOL_SIZE;
    int localIndex = index % POOL_SIZE;

    t_objectMemoryPool* pool = manager->firstPool;
    for (int i = 0; i < poolIndex; i++) {
        pool = pool->next;
    }

    return pool->items[localIndex].typeId;
}

void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, t_fonctionType typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...) {
    va_list args;
    va_start(args, indexObj);

    /* Étape 1 : Compter le nombre de paramètres */
    va_list count_args;
    va_copy(count_args, args);
    int param_count = 0;
    while (va_arg(count_args, void*) != NULL) param_count++;
    va_end(count_args);

    /* Étape 2 : Allouer le tableau dynamiquement */
    void** params = malloc(param_count * sizeof(void*));

    /* Étape 3 : Remplir le tableau */
    for (int i = 0; i < param_count; i++) {
        params[i] = va_arg(args, void*);
    }
    va_end(args);

    /* Traitement des objets */
    for (int i = 0; i < scene->objectManager->count; i++) {
        if (getObjectTypeId(scene->objectManager, i) == typeObject) {
            /* Création d'une copie des paramètres */
            void** final_params = malloc((param_count + 1) * sizeof(void*));

            /* Insertion de l'objet */
            memcpy(final_params, params, indexObj * sizeof(void*));
            final_params[indexObj] = getObject(scene->objectManager, i);
            memcpy(final_params + indexObj + 1,
                   params + indexObj,
                   (param_count - indexObj) * sizeof(void*));

            /* Création de la fonction */
            t_fonctionParam* sf = malloc(sizeof(t_fonctionParam));
            sf->fonction = fonct;
            sf->param = final_params;
            sf->nb_param = param_count + 1;

            /* Ajout à la scène */
            scene->fonctions[typeFunction] = realloc(scene->fonctions[typeFunction],
                                                     (scene->nbFonctions[typeFunction] + 1) * sizeof(t_fonctionParam*));
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