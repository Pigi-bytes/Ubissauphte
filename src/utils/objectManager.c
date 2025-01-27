#include "objectManager.h"

t_objectManager* initObjectManager(t_objectType type, void (*freeFunc)(void*)) {
    DEBUG_PRINT("Création d'un gestionnaire d'objets de type %d\n", type);
    t_objectManager* manager = (t_objectManager*)malloc(sizeof(t_objectManager));
    manager->freeFunc = freeFunc;
    manager->count = 0;
    manager->capacity = INITIAL_CAPACITY;
    manager->items = (t_typedObject**)malloc(manager->capacity * sizeof(t_typedObject*));
    manager->type = type;
    return manager;
}

t_typedObject* createTypedObject(t_objectType type, void* data) {
    t_typedObject* obj = (t_typedObject*)malloc(sizeof(t_typedObject));
    obj->type = type;
    obj->data = data;
    return obj;
}

void addObject(t_objectManager* manager, t_typedObject* object) {
    DEBUG_PRINT("Ajout de l'objet d'adress %p au gestionnaire à l'adresse : %p \n", object, manager);
    if (manager->type != object->type) {
        fprintf(stderr, "Erreur: Tentative d'ajout d'un objet de type %d incorrect dans manager de type %d\n", object->type, manager->type);
        exit(1);
    }
    if (manager->count >= manager->capacity) {
        DEBUG_PRINT("Capacité du gestionnaire atteinte. Redimensionnement à %d...\n", manager->capacity * 2);
        manager->capacity *= 2;
        manager->items = (t_typedObject**)realloc(manager->items, manager->capacity * sizeof(t_typedObject*));
    }
    manager->items[manager->count] = object;
    manager->count++;
    DEBUG_PRINT("Objet ajouté. Nombre d'objets dans le gestionnaire : %d, capacité : %d \n", manager->count, manager->capacity);
}

void freeObjectManager(t_objectManager* manager) {
    for (int i = 0; i < manager->count; ++i) {
        manager->freeFunc(manager->items[i]->data);
        free(manager->items[i]);
        manager->items[i] = NULL;
    }
    free(manager->items);
    manager->items = NULL;
    free(manager);
<<<<<<< HEAD
=======
    manager = NULL;
    DEBUG_PRINT("Gestionnaire d'objets libéré avec succès.\n");
>>>>>>> scene
}