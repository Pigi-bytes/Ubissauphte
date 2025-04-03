// scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

#include "../debug.h"
#include "../io/input.h"
#include "fonctionManager.h"
#include "objectManager.h"

#define ADD_OBJECT_TO_SCENE(scene, obj, type) addObject(scene->objectManager, obj, type)

typedef enum {
    RENDER_GAME,
    RENDER_UI,
    HANDLE_INPUT,
    HANDLE_RESIZE,
    UPDATE,
    SET_BUFFER,
    RENDER_BUFFER,
    NUM_FONCTION
} t_fonctionType;

typedef struct t_scene {
    char* name;
    t_objectManager* objectManager;
    t_fonctionParam** fonctions[NUM_FONCTION];
    int nbFonctions[NUM_FONCTION];
} t_scene;

typedef struct {
    t_scene** historique;
    int nb_Menu;
} t_history;

typedef struct {
    t_objectManager* scene;
    int currentScene;
    t_scene* lastMap;
    t_history* lastMenu;
} t_sceneController;

t_scene* createScene(t_objectManager* objectManager, char* name);
void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, t_fonctionType typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...);
void executeSceneFunctions(t_scene* scene, t_fonctionType ftype);
void freeScene(void* object);

t_sceneController* initSceneController();
void addScene(t_sceneController* controller, t_scene* scene);
void setScene(t_sceneController* controller, char* name);
t_scene* getCurrentScene(t_sceneController* voidcontroller);
void freeSceneController(t_sceneController** controller);

// Ajouter un objet à une scène et l'enregistrer automatiquement pour un type de fonction
void* sceneAddObject(t_scene* scene, void* object, uint8_t typeId);

// Supprimer un objet et toutes ses fonctions associées d'une scène
void sceneRemoveObject(t_scene* scene, void* object);

// Enregistrer une fonction pour un objet spécifique uniquement
void registerFunctionForObject(t_scene* scene, void* object, uint8_t typeId,
                               t_fonctionType funcType, void (*fonct)(t_fonctionParam*),
                               int indexObj, ...);

// Trouver l'index d'un objet dans le gestionnaire d'objets
int findObjectIndex(t_objectManager* manager, void* object);

// Revenir à la scene précedente
void getPrevuisScene(t_sceneController* controller);
void getPrevuisMenu(t_sceneController* controller);
int indiceByscene(t_sceneController* controller, t_scene* scene);
void freeSceneByName(t_sceneController** controller, char* name);
t_scene* GetSceneByName(t_sceneController* controller, char* name);

#endif