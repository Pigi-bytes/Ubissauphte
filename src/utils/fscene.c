#include "./fscene.h"

int count = 0;

void (*tableFonctfree[4])(void *);
typedef struct {
    float x;
    float y;
} t_point;

// fonction qui friee un object de type entier
void freeInt(void *object) {
    int *nb = (int *)object;
    free(nb);
    // DEBUG_MALLOC(nb);
}

// fonction qui friee un object de type Point
void freePoint(void *object) {
    t_point *nb = (t_point *)object;
    free(nb);
    // DEBUG_PRINT("[free]t_point %f %f a l'adresse %p\n", nb->x, nb->y, nb);
}

void (*tableFonctfree[4])(void *) = {freeInt, freePoint, freeButton, freeText};

// fonction qui affiche les object de type INT_TYPE d'une scène
void afficherInt(t_scene *scene) {
    printf("\n|entier présent dans la scene|\n");
    for (int i = 0; i < scene->contenue[0]->manage->count; i++) {
        printf("|nb %d : %d--------------------|\n", i, *(int *)((scene->contenue[0])->manage->items[i]->data));
    }
    printf("|----------------------------|\n\n");
}

// fonction qui affiche les object de type POINT_TYPE d'une scène
void afficherPoint(t_scene *scene) {
    printf("\n|point présent dans la scene|\n");
    for (int i = 0; i < scene->contenue[1]->manage->count; i++) {
        t_point *valeur = (t_point *)((scene->contenue[1])->manage->items[i]->data);
        printf("|nb %d x :%.2f y:%.2f--------|\n", i, valeur->x, valeur->y);
    }
    printf("|---------------------------|\n\n");
}

// fonction qui initialise une scène en mettant tout les pointeurs sur objectManageur à NULL
t_scene *initScene(int taille) {
    t_scene *scene = (t_scene *)malloc(sizeof(t_scene));
    count++;
    // DEBUG_PRINT("[MALLOC] Creation a l'adresse %p\n", scene);
    scene->contenue = (t_fonctionManager **)malloc(sizeof(t_fonctionManager *) * taille);
    count++;
    if (!scene->contenue) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->scene\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    scene->index = (int *)malloc(sizeof(int));
    count++;
    if (!scene->index) {
        fprintf(stderr, "Erreur d'allocation mémoire pour scene->index\n");
        free(scene);
        exit(EXIT_FAILURE);
    }
    scene->nbindex = 0;
    // DEBUG_MALLOC(scene->contenue);
    for (int i = 0; i < taille; i++) {
        scene->contenue[i] = (t_fonctionManager *)malloc(sizeof(t_fonctionManager));
        count++;
        scene->contenue[i]->manage = NULL;
        scene->contenue[i]->fonction = NULL;
        // DEBUG_PRINT("valeur mis à null pour l'adresse %p\n", scene->contenue[i]);
    }
    scene->taille = taille;
    return scene;
}

// fonction qui ajoute un objet dans la scéne en le triant par type
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
    // DEBUG_PRINT("object ajouter à l'adresse %p\n", scene->contenue[object->type]);
}

// fonction qui free une scène
void freeScene(t_scene **scene) {
    if (!scene || !(*scene)) return;

    for (int i = 0; i < (*scene)->taille; i++) {
        // DEBUG_FREE((*scene)->contenue[(*scene)->index[i]]->manage);
        if ((*scene)->contenue[i] && (*scene)->contenue[i]->manage) {
            freeObjectManager((*scene)->contenue[i]->manage);
            count--;
            (*scene)->contenue[i]->manage = NULL;
        }
        if ((*scene)->contenue[i] && (*scene)->contenue[i]->fonction) {
            freeFonction(&(*scene)->contenue[i]->fonction);
            count--;
            (*scene)->contenue[i]->fonction = NULL;
        }
        // DEBUG_FREE((*scene)->contenue[(*scene)->index[i]]->manage);
        free((*scene)->contenue[i]);
        count--;
        (*scene)->contenue[i] = NULL;
        // DEBUG_FREE((*scene)->contenue[(*scene)->index[i]]->manage);
    }
    if ((*scene)->index) {
        free((*scene)->index);
        count--;
        (*scene)->index = NULL;
    }
    (*scene)->nbindex = 0;
    free(*scene);
    count--;
    (*scene) = NULL;
    // DEBUG_PRINT("[FREE] Scène libérée avec succès.\n");
}

// fonction qui affiche les adresses des pointeurs sur ObjectManageur
void afficherAdresse(t_scene *scene) {
    for (int i = 0; i < scene->taille; i++) printf("adresse de l'element %d : %p\n", i, scene->contenue[i]);
}
