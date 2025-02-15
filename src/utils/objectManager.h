#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"

typedef void (*freeFunc)(void*);

// Iterateur specialisé par type pour un accés O(1) au lieu de O(n)
// Ressource utilisé:
// - https://peerdh.com/blogs/programming-insights/memory-pool-management-techniques-in-c
// - https://en.wikipedia.org/wiki/Memory_pool
// - https://8dcc.github.io/programming/pool-allocator.html <- Version super basique

#define POOL_SIZE 64  // Taille d'un bloc mémoire
#define MAX_UINT8_T 256

typedef struct {
    freeFunc freeFunc;
    char* name;
} t_typeMetadata;

typedef struct {
    t_typeMetadata types[256];
    uint8_t nextTypeId;  // Prochain ID disponible
} t_typeRegistry;

void initTypeRegistry(t_typeRegistry* registre);
uint8_t registerType(t_typeRegistry* registre, freeFunc freeFunc, char* name);

typedef struct {
    void* data;
    uint8_t typeId;
} t_typedObject;

typedef struct t_objectMemoryPool {
    t_typedObject items[POOL_SIZE];
    struct t_objectMemoryPool* next;
} t_objectMemoryPool;

typedef struct {
    t_objectMemoryPool* firstPool;
    t_objectMemoryPool* currentPool;
    int nbItemsInPool;
    t_typeRegistry* registry;
    int count;
} t_objectManager;

t_objectManager* initObjectManager(t_typeRegistry* registre);
void addObject(t_objectManager* manager, void* data, uint8_t typeId);
void freeObjectManager(t_objectManager* manager);

#endif  // OBJECT_MANAGER_H
