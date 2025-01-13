#include "objectManager.h"

#define TAILLE 3
typedef struct {
    int taille = TAILLE;
    t_objectManager* scene[TAILLE];
} t_scene;
