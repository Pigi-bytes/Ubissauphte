#include "objectManager.h"

t_objectManager* initObjectManager(t_objectType type, void (*freeFunc)(void*), int capacity) {
    t_objectManager* manager = (t_objectManager*)malloc(sizeof(t_objectManager));
    if (!manager) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le gestionnaire d'objets.\n");
        exit(EXIT_FAILURE);
    }

    manager->freeFunc = freeFunc;
    manager->count = 0;
    manager->capacity = capacity;
    manager->type = type;
    manager->items = (t_typedObject**)malloc(manager->capacity * sizeof(t_typedObject*));
    if (!manager->items) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les items.\n");
        exit(EXIT_FAILURE);
    }

    DEBUG_PRINT("Gestionnaire d'objets créé à l'adresse %p avec une capacité initiale de %d\n", manager, manager->capacity);
    return manager;
}

t_typedObject* createTypedObject(t_objectType type, void* data) {
    t_typedObject* obj = (t_typedObject*)malloc(sizeof(t_typedObject));
    if (!obj) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour l'objet.\n");
        exit(EXIT_FAILURE);
    }

    obj->type = type;
    obj->data = data;
    DEBUG_PRINT("Objet créé à l'adresse %p avec type %d\n", obj, obj->type);
    return obj;
}

void addObject(t_objectManager* manager, t_typedObject* object) {
    DEBUG_PRINT("Ajout de l'objet d'adresse %p au gestionnaire d'objets à l'adresse %p\n", object, manager);

    if (manager->type != object->type) {
        fprintf(stderr, "Erreur : tentative d'ajout d'un objet de type %d incorrect dans un gestionnaire de type %d\n", object->type, manager->type);
        exit(EXIT_FAILURE);
    }

    if (manager->count >= manager->capacity) {
        manager->capacity *= 2;
        manager->items = (t_typedObject**)realloc(manager->items, manager->capacity * sizeof(t_typedObject*));
        if (!manager->items) {
            fprintf(stderr, "Erreur lors du redimensionnement de la mémoire pour les items.\n");
            exit(EXIT_FAILURE);
        }
        DEBUG_PRINT("Capacité du gestionnaire atteinte. Redimensionnement de la capacité de %d à %d \n", manager->capacity, manager->capacity);
    }

    manager->items[manager->count] = object;
    manager->count++;

    DEBUG_PRINT("Objet ajouté. Nombre d'objets dans le gestionnaire : %d, capacité : %d\n", manager->count, manager->capacity);
}

void* getObject(t_objectManager* manager, size_t index) {
    if (manager == NULL || index >= manager->count) {
        return NULL;
    }

    t_typedObject* obj = manager->items[index];
    return obj->data;
}

void freeObjectManager(t_objectManager* manager) {
    DEBUG_PRINT("Libération du gestionnaire d'objets à l'adresse %p. Nombre d'objets à libérer : %d\n", manager, manager->count);

    for (int i = 0; i < manager->count; ++i) {
        DEBUG_PRINT("Libération de l'objet à l'adresse %p\n", manager->items[i]);
        manager->freeFunc(manager->items[i]->data);
        free(manager->items[i]);
    }

    free(manager->items);
    free(manager);
    DEBUG_PRINT("Gestionnaire d'objets à l'adresse %p libéré.\n", manager);
}