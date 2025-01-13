#include <stdio.h>
#include <stdlib.h>

#include "./objectManager.h"

typedef struct {
    int taille;
    t_objectManager **contenue;
} t_scene;

typedef void t_fonctFree(void *);
t_scene *initScene(int);
void ajoutObjectManager(t_scene *, t_typedObject *);
void freeScene(t_scene *);
void afficherAdresse(t_scene *);
void afficherInt(t_scene *);
void afficherFloat(t_scene *);
void freeInt(void *);
void freePoint(void *);
