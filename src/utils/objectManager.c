#include "objectManager.h"

t_typeRegistry* createTypeRegistry() {
    t_typeRegistry* registre = (t_typeRegistry*)malloc(sizeof(t_typeRegistry));
    // Initialisation memoire via memset pour garantir l'absence de valeurs dans la memoire
    memset(registre, 0, sizeof(t_typeRegistry));
    registre->nextTypeId = 0;
    return registre;
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

void* getObject(t_objectManager* manager, int index) {
    if (index >= manager->count) {
        return NULL;  // Index hors limites
    }

    // Calcule le pool et l'index local dans le pool
    int poolIndex = index / POOL_SIZE;   // Division pour trouver le pool
    int localIndex = index % POOL_SIZE;  // Modulo pour trouver l'index dans le pool

    // Parcourt les pools jusqu'à atteindre le bon
    t_objectMemoryPool* pool = manager->firstPool;
    for (int i = 0; i < poolIndex; i++) {
        pool = pool->next;
    }

    return pool->items[localIndex].data;
}

void freeObjectManager(t_objectManager* manager) {
    t_objectMemoryPool* pool = manager->firstPool;

    // Parcours de tous les blocs memoires
    while (pool != NULL) {
        // Détermine combien d'objets sont utilisés dans ce bloc
        int items2Free = POOL_SIZE;          // Bloc plein par default
        if (pool == manager->currentPool) {  // Si le bloc est pas plein on regarde combien d'element sont stocké
            items2Free = manager->nbItemsInPool;
        }

        // Libération de chaque objet dans le bloc
        for (int i = 0; i < items2Free; i++) {
            t_typedObject entry = pool->items[i];
            // Récupère les metadonnées du type
            t_typeMetadata* metaData = &manager->registry->types[entry.typeId];
            if (entry.data != NULL) {
                metaData->freeFunc(entry.data);
            }
        }

        // Passage au bloc suivant et libération du bloc actuel
        t_objectMemoryPool* next = pool->next;
        free(pool);
        pool = next;
    }
    if (manager->registry != NULL) {
        free(manager->registry);
    }
    free(manager);
}