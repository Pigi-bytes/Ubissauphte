#include <stdio.h>
#include <stdlib.h>

#include "./objectManager.h"

typedef struct {
    int taille;
    t_objectManager **contenue;
    int *index;
    int nbindex;
} t_scene;

t_scene *initScene(int taille);
void ajoutObjectManager(t_scene *scene, t_typedObject *type);
void freeScene(t_scene *scene);
void afficherAdresse(t_scene *scene);
void afficherInt(t_scene *scene);
void afficherFloat(t_scene *scene);
void freeInt(void *);
void freePoint(void *);
