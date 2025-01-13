
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"
#include "./objectManager.h"
#include "./scene.h"

typedef struct {
    float x;
    float y;
} t_point;

typedef void funct(void *);

void freeInt(void *object) {
    int *nb = (int *)object;
    free(nb);
    DEBUG_PRINT("[free]int %d a l'adresse %p", *nb, nb);
}

funct *fonction[] = {freeInt};
void afficheInt(t_scene *scene) {
    printf("\n|entier présent dans la scene|\n");
    for (int i = 0; i < scene->contenue[0]->count; i++) {
        printf("|nb %d : %d--------------------|\n", i, *(int *)((scene->contenue[0])->items[i]->data));
    }
    printf("|----------------------------|\n");
}

t_scene *initScene(int taille) {
    t_scene *scene = (t_scene *)malloc(sizeof(t_scene));
    DEBUG_PRINT("[MALLOC] Creation a l'adresse %p\n", scene);
    scene->contenue = (t_objectManager **)malloc(sizeof(t_objectManager *) * taille);
    if (!scene->contenue) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->scene\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    DEBUG_PRINT("[MALLOC] Creation a l'adresse %p\n", scene->contenue);
    for (int i = 0; i < taille; i++) {
        scene->contenue[i] = NULL;
    }
    scene->taille = taille;
    return scene;
}

void ajoutObjectManager(t_scene *s, t_typedObject *object) {
    if (!(s->contenue[object->type])) {
        s->contenue[object->type] = initObjectManager(object->type, fonction[object->type]);
    }
    addObject(s->contenue[object->type], object);
}

void freeScene(t_scene *scene) {
    for (int i = 0; i < scene->taille; i++) {
        if (scene->contenue[i] != NULL)
            freeObjectManager(scene->contenue[i]);
    }
    DEBUG_PRINT("[Free]scene : %p", scene);
}

int main() {
    t_scene *test = initScene(3);
    int quatre = 4;
    int cinq = 5;
    t_typedObject *nb = createTypedObject(INT_TYPE, &quatre);
    t_typedObject *nb2 = createTypedObject(INT_TYPE, &cinq);
    ajoutObjectManager(test, nb);
    ajoutObjectManager(test, nb2);
    afficheInt(test);
    freeScene(test);
    afficheInt(test);
}