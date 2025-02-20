#include "fscene.h"

#include <stdlib.h>

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

void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, int typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...) {
    va_list args, args_copy;
    va_start(args, indexObj);
    va_copy(args_copy, args);  // Pour relire les arguments plus tard

    int paramCount = 0;
    void* arg;
    while ((arg = va_arg(args, void*)) != NULL) {
        paramCount++;
    }
    va_end(args);

    for (int i = 0; i < scene->objectManager->count; i++) {
        int id = getObjectTypeId(scene->objectManager, i);
        if (id == typeObject) {
            void** newParams = malloc(sizeof(void*) * (paramCount + 2));

            for (int j = 0; j < paramCount; j++) {
                newParams[j] = va_arg(args_copy, void*);
            }
            va_end(args_copy);

            for (int j = paramCount; j > indexObj; j--) {
                newParams[j] = newParams[j - 1];
            }
            newParams[indexObj] = getObject(scene->objectManager, i);
            newParams[paramCount + 1] = NULL;                        

            t_fonctionParam* funct = malloc(sizeof(t_fonctionParam));
            funct->param = newParams;
            funct->fonction = fonct;
            funct->nb_param = paramCount + 1;

            int currentCount = *(scene->nbFonctions[typeFunction]);
            scene->fonctions[typeFunction] = realloc(scene->fonctions[typeFunction], (currentCount + 1) * sizeof(t_fonctionParam*));
            scene->fonctions[typeFunction][currentCount] = funct;
            *(scene->nbFonctions[typeFunction]) += 1;

            printf("ajout objet de type %d dans la fonction %d \n", typeObject, typeFunction);
        }
    }
}

t_scene* createScene(char* name, t_objectManager* objectManager) {
    t_scene* scene = malloc(sizeof(t_scene));
    scene->objectManager = objectManager;
    scene->fonctions = malloc(NUM_FONCTION * sizeof(t_fonctionParam**));
    for (int i = 0; i < NUM_FONCTION; i++) {
        scene->fonctions[i] = NULL;
    }

    scene->nbFonctions = malloc(NUM_FONCTION * sizeof(int*));
    for (int i = 0; i < NUM_FONCTION; i++) {
        scene->nbFonctions[i] = malloc(sizeof(int));
        *(scene->nbFonctions[i]) = 0;
    }

    scene->loaded = false;
    scene->name = name;

    return scene;
}