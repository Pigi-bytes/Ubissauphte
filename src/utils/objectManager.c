#include "objectManager.h"

void initTypeRegistry(t_typeRegistry* registre) {
    // Initialisation memoire via memset pour garantir l'absence de valeurs dans la memoire
    memset(registre, 0, sizeof(t_typeRegistry));
    registre->nextTypeId = 0;
}

uint8_t registerType(t_typeRegistry* registre, freeFunc freeFunc, char* name) {
    // Récupère l'ID disponible actuel et l'incrémente pour le prochain ajout
    uint8_t id = registre->nextTypeId++;
    registre->types[id] = (t_typeMetadata){.freeFunc = freeFunc, .name = name};
    return id;
}

t_objectManager* initObjectManager(t_typeRegistry* registre) {
    // Allocation du gestionnaire principal
    t_objectManager* manager = (t_objectManager*)malloc(sizeof(t_objectManager));

    // Creation du premier bloc mémoire
    manager->firstPool = (t_objectMemoryPool*)malloc(sizeof(t_objectMemoryPool));
    manager->firstPool->next = NULL;  // Pas de bloc suivant

    manager->currentPool = manager->firstPool;
    manager->nbItemsInPool = 0;
    manager->count = 0;
    manager->registry = registre;
    return manager;
}

void addObject(t_objectManager* manager, void* data, uint8_t typeId) {
    // Vérifie si le pool actif est plein
    if (manager->nbItemsInPool >= POOL_SIZE) {
        // Allocation d'un nouveau bloc mémoire
        t_objectMemoryPool* newPool = (t_objectMemoryPool*)malloc(sizeof(t_objectMemoryPool));
        newPool->next = NULL;  // Pas de bloc suivant

        // Chainage du nouveau bloc avec le precedent
        manager->currentPool->next = newPool;
        manager->currentPool = newPool;
        manager->nbItemsInPool = 0;
    }

    // Stockage de l'objet
    manager->currentPool->items[manager->nbItemsInPool++] = (t_typedObject){.data = data, .typeId = typeId};
    manager->count++;
}

void freeObjectManager(t_objectManager* manager) {
    t_objectMemoryPool* pool = manager->firstPool;

    // Parcours de tous les blocs memoires
    while (pool != NULL) {
        // Détermine combien d'objets sont utilisés dans ce bloc
        int items2Free = POOL_SIZE;          // Bloc plein par default
        if (pool == manager->currentPool) {  // Si le bloc est pas plein on regarde combien d'element sont stocké
            manager->nbItemsInPool;
        }

        // Libération de chaque objet dans le bloc
        for (int i = 0; i < items2Free; i++) {
            t_typedObject entry = pool->items[i];
            // Récupère les metadonnées du type
            t_typeMetadata* metaData = &manager->registry->types[entry.typeId];
            metaData->freeFunc(entry.data);
        }

        // Passage au bloc suivant et libération du bloc actuel
        t_objectMemoryPool* next = pool->next;
        free(pool);
        pool = next;
    }
    free(manager);
}