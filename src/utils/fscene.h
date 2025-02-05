#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"
#include "../tileEngine/tilesManager.h"
#include "../ui/button.h"
#include "../ui/text.h"
#include "./fonctionManager.h"
#include "./objectManager.h"

#define ADD_OBJECT_TO_SCENE(scene, type, value_ptr) ajoutObjectManager(scene, createTypedObject(type, value_ptr));

typedef struct {
    int taille;
    t_fonctionManager **contenue;
    int *index;
    int nbindex;
} t_scene;

t_scene *initScene(int taille);
void ajoutObjectManager(t_scene *scene, t_typedObject *type);
void freeScene(t_scene **scene);
void callRender(t_scene *scene);