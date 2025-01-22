#include "objectManager.h"

t_objectManager* initObjectManager(t_objectType type, void (*freeFunc)(void*)) {
    DEBUG_PRINT("Création d'un gestionnaire d'objets de type %d\n", type);
    t_objectManager* manager = (t_objectManager*)malloc(sizeof(t_objectManager));
    manager->freeFunc = freeFunc;
    manager->count = 0;
    manager->capacity = INITIAL_CAPACITY;
    manager->items = (t_typedObject**)malloc(manager->capacity * sizeof(t_typedObject*));
    manager->type = type;
    DEBUG_PRINT("[MALLOC] Gestionnaire d'objets créé avec succès à l'adresse : %p\n", manager);
    return manager;
}

t_typedObject* createTypedObject(t_objectType type, void* data) {
    t_typedObject* obj = (t_typedObject*)malloc(sizeof(t_typedObject));
    obj->type = type;
    obj->data = data;
    DEBUG_PRINT("[MALLOC] Objet de type %d créé avec succès à l'adresse : %p\n", type, obj);
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
    DEBUG_PRINT("[FREE] Libération des ressources du gestionnaire d'objets à l'adresse : %p\n", manager);
    for (int i = 0; i < manager->count; ++i) {
        DEBUG_PRINT("[FREE] Libération des ressources de l'objet à l'adresse : %p\n", manager->items[i]);
        manager->freeFunc(manager->items[i]->data);
        free(manager->items[i]);
    }
    free(manager->items);
    free(manager);
    DEBUG_PRINT("Gestionnaire d'objets libéré avec succès.\n");
}