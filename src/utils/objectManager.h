#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"

#define INITIAL_CAPACITY 10
#define ADD_TYPED_OBJECT(manager, type, value_ptr) addObject(manager, createTypedObject(type, value_ptr))

typedef enum {
    BUTTON_TYPE = 0,
    SDL_TEXTURE_TYPE,
    TEXT_TYPE,
    AUDIO_TYPE,
} t_objectType;

typedef struct {
    t_objectType type;
    void* data;
} t_typedObject;

typedef struct {
    t_typedObject** items;
    int count;
    int capacity;
    t_objectType type;
    void (*freeFunc)(void*);
} t_objectManager;

t_objectManager* initObjectManager(t_objectType type, void (*freeFunc)(void*), int capacity);
t_typedObject* createTypedObject(t_objectType type, void* data);
void addObject(t_objectManager* manager, t_typedObject* object);
void* getObject(t_objectManager* manager, size_t index);
void freeObjectManager(t_objectManager** manager);

#endif  // OBJECT_MANAGER_H
