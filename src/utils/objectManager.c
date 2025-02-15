#include "objectManager.h"

void initTypeRegistry(t_typeRegistry* registre) {
    memset(registre, 0, sizeof(t_typeRegistry));
    registre->nextTypeId = 0;
}

uint8_t registerType(t_typeRegistry* registre, freeFunc freeFunc, char* name) {
    uint8_t id = registre->nextTypeId++;
    registre->types[id] = (t_typeMetadata){.freeFunc = freeFunc, .name = name};
    return id;
}

t_objectManager* initObjectManager(t_typeRegistry* registre) {
    t_objectManager* manager = (t_objectManager*)malloc(sizeof(t_objectManager));
    manager->firstPool = (t_objectMemoryPool*)malloc(sizeof(t_objectMemoryPool));
    manager->firstPool->next = NULL;
    manager->currentPool = manager->firstPool;
    manager->nbItemsInPool = 0;
    manager->count = 0;
    manager->registry = registre;
    return manager;
}

void addObject(t_objectManager* manager, void* data, uint8_t typeId) {
    if (manager->nbItemsInPool >= POOL_SIZE) {
        t_objectMemoryPool* newPool = (t_objectMemoryPool*)malloc(sizeof(t_objectMemoryPool));
        newPool->next = NULL;
        manager->currentPool->next = newPool;
        manager->currentPool = newPool;
        manager->nbItemsInPool = 0;
    }

    manager->currentPool->items[manager->nbItemsInPool++] = (t_typedObject){.data = data, .typeId = typeId};
    manager->count++;
}

void freeObjectManager(t_objectManager* manager) {
    t_objectMemoryPool* pool = manager->firstPool;
    while (pool != NULL) {
        int items2Free = (pool == manager->currentPool) ? manager->nbItemsInPool : POOL_SIZE;
        for (int i = 0; i < items2Free; i++) {
            t_typedObject entry = pool->items[i];
            t_typeMetadata* metaData = &manager->registry->types[entry.typeId];
            if (metaData->freeFunc) {
                metaData->freeFunc(entry.data);
            }
        }

        t_objectMemoryPool* next = pool->next;
        free(pool);
        pool = next;
    }
    free(manager);
}