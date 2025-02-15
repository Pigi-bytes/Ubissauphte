#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils/objectManager.h"

#define window_width 1280
#define window_height 960

typedef struct {
    char* nom;
    char* prenom;
    int age;
} t_bonhomme;

typedef struct {
    int largeur;
    int hauteur;
    char* nom;
    char* couleur;
} t_rectangle;

void freeBonhomme(void* b) {
    t_bonhomme* bon = (t_bonhomme*)b;
    free(bon->nom);
    free(bon->prenom);
    free(bon);
}

void freeRectangle(void* r) {
    t_rectangle* rectangle = (t_rectangle*)r;
    free(rectangle->nom);
    free(rectangle->couleur);
    free(rectangle);
}

t_rectangle* genererRectangleAleatoire() {
    t_rectangle* rectangle = (t_rectangle*)malloc(sizeof(t_rectangle));
    rectangle->largeur = rand() % 100 + 1;  // Largeur entre 1 et 100
    rectangle->hauteur = rand() % 100 + 1;  // Hauteur entre 1 et 100
    rectangle->nom = strdup("Rect");
    rectangle->couleur = strdup("Couleur");
    return rectangle;
}

// Fonction pour générer un Bonhomme aléatoire
t_bonhomme* genererBonhommeAleatoire() {
    t_bonhomme* bonhomme = (t_bonhomme*)malloc(sizeof(t_bonhomme));
    bonhomme->nom = strdup("Nom");
    bonhomme->prenom = strdup("Prenom");
    bonhomme->age = rand() % 100;  // Âge entre 0 et 99
    return bonhomme;
}

void forEachObject(t_objectManager* mgr, void (*func)(void* data, uint8_t typeId)) {
    int count = 0;
    t_objectMemoryPool* pool = mgr->firstPool;
    while (pool != NULL && count < mgr->count) {
        int nbItemsInPool = (pool == mgr->currentPool) ? mgr->nbItemsInPool : POOL_SIZE;
        for (int i = 0; i < nbItemsInPool && count < mgr->count; i++, count++) {
            func(pool->items[i].data, pool->items[i].typeId);
        }
        pool = pool->next;
    }
}

void printObject(void* data, uint8_t typeId) {
    printf("Objet de type %d à l'adresse %p\n", typeId, data);
}

void forEachObjectOfType(t_objectManager* mgr, uint8_t typeId, void (*func)(void* data)) {
    int count = 0;
    t_objectMemoryPool* pool = mgr->firstPool;
    while (pool != NULL && count < mgr->count) {
        int nbItemsInPool = (pool == mgr->currentPool) ? mgr->nbItemsInPool : POOL_SIZE;
        for (int i = 0; i < nbItemsInPool && count < mgr->count; i++, count++) {
            if (pool->items[i].typeId == typeId) {
                func(pool->items[i].data);
            }
        }
        pool = pool->next;
    }
}

void afficherBonhomes(void* object) {
    t_bonhomme* bonhomme = (t_bonhomme*)object;
    printf("Nom : [%s] | Prenom : [%s] | Age : [%d] \n", bonhomme->nom, bonhomme->prenom, bonhomme->age);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    t_typeRegistry registre;
    initTypeRegistry(&registre);
    uint8_t BONHOMME_TYPE = registerType(&registre, freeBonhomme, "Bonhomme");
    const uint8_t RECTANGLE_TYPE = registerType(&registre, freeRectangle, "Rectangle");
    t_objectManager* manager = initObjectManager(&registre);

    // Générer 1000 objets aléatoires
    for (int i = 0; i < 20; i++) {
        if (rand() % 2 == 0) {
            t_bonhomme* bonhomme = genererBonhommeAleatoire();
            addObject(manager, bonhomme, BONHOMME_TYPE);
        } else {
            t_rectangle* rectangle = genererRectangleAleatoire();
            addObject(manager, rectangle, RECTANGLE_TYPE);
        }
    }

    forEachObjectOfType(manager, BONHOMME_TYPE, afficherBonhomes);

    freeObjectManager(manager);
    return EXIT_SUCCESS;
}