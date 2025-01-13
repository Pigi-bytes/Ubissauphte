#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

#include "objectManager.h"

void initScene(t_scene scene) {
    for (int i = 0; i < scene.taille; i++) {
        scene.scene[i] = NULL;
    }
}

void ajoutObjet(t_scene scene, t_typedObject, t_objectManager) {
}