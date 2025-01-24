
#include "./fscene.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"
#include "./objectManager.h"

typedef struct {
    float x;
    float y;
} t_point;

// fonction qui friee un object de type entier
void freeInt(void *object) {
    int *nb = (int *)object;
    DEBUG_PRINT("[free]int %d a l'adresse %p\n", *nb, nb);
}

// fonction qui friee un object de type Point
void freePoint(void *object) {
    t_point *nb = (t_point *)object;
    DEBUG_PRINT("[free]t_point %f %f a l'adresse %p\n", nb->x, nb->y, nb);
}

// fonction qui affiche les object de type INT_TYPE d'une scène
void afficherInt(t_scene *scene) {
    printf("\n|entier présent dans la scene|\n");
    for (int i = 0; i < scene->contenue[0]->count; i++) {
        printf("|nb %d : %d--------------------|\n", i, *(int *)((scene->contenue[0])->items[i]->data));
    }
    printf("|----------------------------|\n\n");
}

// fonction qui affiche les object de type POINT_TYPE d'une scène
void afficherPoint(t_scene *scene) {
    printf("\n|point présent dans la scene|\n");
    for (int i = 0; i < scene->contenue[1]->count; i++) {
        t_point *valeur = (t_point *)((scene->contenue[1])->items[i]->data);
        printf("|nb %d x :%.2f y:%.2f--------|\n", i, valeur->x, valeur->y);
    }
    printf("|---------------------------|\n\n");
}

// fonction qui initialise une scène en mettant tout les pointeurs sur objectManageur à NULL
t_scene *initScene(int taille) {
    t_scene *scene = (t_scene *)malloc(sizeof(t_scene));
    DEBUG_PRINT("[MALLOC] Creation a l'adresse %p\n", scene);
    scene->contenue = (t_objectManager **)malloc(sizeof(t_objectManager *) * taille);
    if (!scene->contenue) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->scene\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    scene->index = malloc(sizeof(int));
    if (!scene->index) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->index\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    scene->nbindex = 0;
    DEBUG_PRINT("[MALLOC] Creation a l'adresse %p\n", scene->contenue);
    for (int i = 0; i < taille; i++) {
        scene->contenue[i] = NULL;
        DEBUG_PRINT("valeur mis à null pour l'adresse %p\n", scene->contenue[i]);
    }
    scene->taille = taille;
    return scene;
}

// fonction qui ajoute un objet dans la scéne en le triant par type
void ajoutObjectManager(t_scene *scene, t_typedObject *object) {
    if (!(scene->contenue[object->type])) {
        scene->contenue[object->type] = initObjectManager(object->type, freeInt);
        if (scene->nbindex) {
            scene->index = realloc(scene->index, sizeof(scene->index) + 1);
        }
        scene->index[scene->nbindex] = object->type;
        scene->nbindex++;
    }
    addObject(scene->contenue[object->type], object);
    DEBUG_PRINT("object ajouter à l'adresse %p\n", scene->contenue[object->type]);
}

// fonction qui free une scène
void freeScene(t_scene *scene) {
    for (int i = 0; i < scene->nbindex; i++) {
        DEBUG_PRINT("valeur en entrée : %p \n", scene->contenue[scene->index[i]]);
        if (!(scene->contenue[scene->index[i]])) {
            DEBUG_PRINT("pointeur Null : \n");
        } else {
            DEBUG_PRINT("[Pres-Free]scene : %p\n", scene->contenue[scene->index[i]]);
            freeObjectManager(scene->contenue[scene->index[i]]);
            DEBUG_PRINT("[Post-Free]scene : %p\n", scene->contenue[scene->index[i]]);
            scene->contenue[scene->index[i]] = NULL;
            DEBUG_PRINT("[FREE]scene : %p\n", scene->contenue[scene->index[i]]);
        }
    }
    free(scene->contenue);
    scene->contenue = NULL;
    free(scene->index);
    scene->index = NULL;
    scene->nbindex = 0;
    free(scene);
    scene = NULL;
    DEBUG_PRINT("[FREE] Scène libérée avec succès.\n");
}

// fonction qui affiche les adresses des pointeurs sur ObjectManageur
void afficherAdresse(t_scene *scene) {
    for (int i = 0; i < scene->taille; i++) DEBUG_PRINT("adresse de l'element %d : %p\n", i, scene->contenue[i]);
}

int main() {
    t_scene *test = initScene(3);
    int quatre = 4;
    int cinq = 5;
    t_point *boint = malloc(sizeof(t_point));
    boint->x = 3.2;
    boint->y = 4.5;
    t_typedObject *point1 = createTypedObject(POINT_TYPE, boint);
    t_typedObject *nb = createTypedObject(INT_TYPE, &quatre);
    t_typedObject *nb2 = createTypedObject(INT_TYPE, &cinq);
    ajoutObjectManager(test, nb);
    ajoutObjectManager(test, nb2);
    ajoutObjectManager(test, point1);
    afficherAdresse(test);
    afficherInt(test);
    afficherPoint(test);
    freeScene(test);
}