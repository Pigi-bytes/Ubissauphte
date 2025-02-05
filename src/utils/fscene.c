#include "./fscene.h"

void (*tableFonctfree[3])(void *) = {freeButton, SDL_DestroyTextureWrapper, freeText};

t_scene *initScene(int taille) {
    t_scene *scene = (t_scene *)malloc(sizeof(t_scene));
    scene->contenue = (t_fonctionManager **)malloc(sizeof(t_fonctionManager *) * taille);
    if (!scene->contenue) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->scene\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    scene->index = (int *)malloc(sizeof(int));
    if (!scene->index) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->index\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    scene->nbindex = 0;
    for (int i = 0; i < taille; i++) {
        scene->contenue[i] = (t_fonctionManager *)malloc(sizeof(t_fonctionManager));
        scene->contenue[i]->manage = NULL;
        scene->contenue[i]->fonction = NULL;
    }
    scene->taille = taille;
    return scene;
}

void ajoutObjectManager(t_scene *scene, t_typedObject *object) {
    if (!(scene->contenue[object->type]->manage)) {
        scene->contenue[object->type]->manage = initObjectManager(object->type, tableFonctfree[object->type], INITIAL_CAPACITY);
        scene->contenue[object->type]->fonction = initFonction();
        if (scene->nbindex) {
            scene->index = realloc(scene->index, (scene->nbindex + 1) * sizeof(int));
        }
        scene->index[scene->nbindex] = object->type;
        scene->nbindex++;
    }
    addObject(scene->contenue[object->type]->manage, object);
}

void freeScene(t_scene **scene) {
    if (!scene || !(*scene)) return;

    for (int i = 0; i < (*scene)->taille; i++) {
        if ((*scene)->contenue[i] && (*scene)->contenue[i]->manage) {
            freeObjectManager(&(*scene)->contenue[i]->manage);
            (*scene)->contenue[i]->manage = NULL;
        }
        if ((*scene)->contenue[i] && (*scene)->contenue[i]->fonction) {
            freeFonction(&(*scene)->contenue[i]->fonction);
            (*scene)->contenue[i]->fonction = NULL;
        }
        free((*scene)->contenue[i]);
        (*scene)->contenue[i] = NULL;
    }
    free((*scene)->contenue);
    (*scene)->contenue = NULL;
    if ((*scene)->index) {
        free((*scene)->index);
        (*scene)->index = NULL;
    }
    (*scene)->nbindex = 0;
    free(*scene);
    (*scene) = NULL;
}

void callRender(t_scene *scene) {
    for (int i = 0; i < scene->taille; i++) {
        if (scene->contenue[i] && scene->contenue[i]->fonction) {
            call(scene->contenue[i], RENDERALL);
        }
    }
}
