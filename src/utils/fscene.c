#include "fscene.h"

#include <stdlib.h>

t_scene* createScene(t_objectManager* manager, char* name) {
    t_scene* scene = malloc(sizeof(t_scene));
    scene->name = strdup(name);
    scene->objectManager = manager;

    for (int i = 0; i < NUM_FONCTION; i++) {
        scene->fonctions[i] = NULL;
        scene->nbFonctions[i] = 0;
    }
    return scene;
}

void sceneRegisterFunction(t_scene* scene, uint8_t typeObject, t_fonctionType typeFunction, void (*fonct)(t_fonctionParam*), int indexObj, ...) {
    va_list args;
    va_start(args, indexObj);

    va_list count_args;
    va_copy(count_args, args);
    int param_count = 0;
    while (va_arg(count_args, void*) != NULL) param_count++;
    va_end(count_args);

    void** params = malloc(param_count * sizeof(void*));

    for (int i = 0; i < param_count; i++) {
        params[i] = va_arg(args, void*);
    }
    va_end(args);

    for (int i = 0; i < scene->objectManager->count; i++) {
        if (getObjectTypeId(scene->objectManager, i) == typeObject) {
            void** final_params = malloc((param_count + 1) * sizeof(void*));

            for (int j = 0; j < indexObj; j++) {
                final_params[j] = params[j];
            }
            int j;
            if (indexObj != -1) {
                final_params[indexObj] = getObject(scene->objectManager, i);
                j = indexObj;
            } else {
                j = 0;
            }
            for (; j < param_count; j++) {
                if (indexObj != -1)
                    final_params[j + 1] = params[j];
                else
                    final_params[j] = params[j];
            }

            t_fonctionParam* sf = malloc(sizeof(t_fonctionParam));
            sf->fonction = fonct;
            sf->param = final_params;
            sf->nb_param = param_count + 1;

            scene->fonctions[typeFunction] = realloc(scene->fonctions[typeFunction], (scene->nbFonctions[typeFunction] + 1) * sizeof(t_fonctionParam*));
            scene->fonctions[typeFunction][scene->nbFonctions[typeFunction]++] = sf;
        }
    }

    free(params);
}

void executeSceneFunctions(t_scene* scene, t_fonctionType ftype) {
    for (int i = 0; i < scene->nbFonctions[ftype]; i++) {
        callFonction(scene->fonctions[ftype][i]);
    }
}

void freeScene(void* object) {
    t_scene* scene = (t_scene*)object;
    freeObjectManager(scene->objectManager);
    for (int i = 0; i < NUM_FONCTION; i++) {
        for (int j = 0; j < scene->nbFonctions[i]; j++) {
            freeFonction(&(scene->fonctions[i][j]));
        }
        free(scene->fonctions[i]);
    }
    free(scene->name);
    free(scene);
}

t_sceneController* initSceneController() {
    t_sceneController* controller = malloc(sizeof(t_sceneController));

    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, freeScene, "ALLSCENE_TYPE");

    controller->scene = initObjectManager(registre);
    controller->currentScene = -1;
    controller->lastMenu = malloc(sizeof(t_history));
    controller->lastMenu->nb_Menu = -1;
    controller->lastMenu->historique = malloc(sizeof(t_scene*) * 4);

    return controller;
}

void addScene(t_sceneController* controller, t_scene* scene) {
    addObject(controller->scene, scene, getTypeIdByName(controller->scene->registry, "ALLSCENE_TYPE"));
}

void setScene(t_sceneController* controller, char* name) {
    for (int i = 0; i < controller->scene->count; i++) {
        t_scene* scene = (t_scene*)getObject(controller->scene, i);
        if (strcmp(scene->name, name) == 0) {
            void* elt = NULL;
            if (controller->currentScene != -1) {
                elt = getObject(controller->scene, controller->currentScene);
            }
            t_scene* salleDep = NULL;
            if (elt != NULL) {
                salleDep = (t_scene*)elt;
            }
            if (salleDep && strcmp(salleDep->name, "main")) {
                controller->lastMenu->historique[++controller->lastMenu->nb_Menu] = (t_scene*)getObject(controller->scene, controller->currentScene);
                printf("ajouter\n");
            }
            if (!strcmp(scene->name, "main")) {
                controller->lastMenu->nb_Menu = -1;
            }
            controller->currentScene = i;
            executeSceneFunctions(scene, HANDLE_RESIZE);

            return;
        }
    }
}
t_scene* GetSceneByName(t_sceneController* controller, char* name) {
    for (int i = 0; i < controller->scene->count; i++) {
        t_scene* scene = (t_scene*)getObject(controller->scene, i);
        if (strcmp(scene->name, name) == 0) {
            return scene;
        }
    }
    return NULL;  // sinon ya un warning
}

int indiceByscene(t_sceneController* controller, t_scene* scene) {
    for (int i = 0; i < controller->scene->count; i++) {
        if (getObject(controller->scene, i) == scene) {
            return i;
        }
    }
    return -1;
}

void getPrevuisScene(t_sceneController* controller) {
    controller->currentScene = indiceByscene(controller, controller->lastMap);
    t_scene* scene = (t_scene*)getObject(controller->scene, controller->currentScene);
    executeSceneFunctions(scene, HANDLE_RESIZE);
}

void getPrevuisMenu(t_sceneController* controller) {
    printf("retour en arrière");
    if (controller->lastMenu->nb_Menu >= 0) {
        controller->currentScene = indiceByscene(controller, controller->lastMenu->historique[controller->lastMenu->nb_Menu]);
        controller->lastMenu->nb_Menu--;
    }
    t_scene* scene = (t_scene*)getObject(controller->scene, controller->currentScene);
    executeSceneFunctions(scene, HANDLE_RESIZE);
}
t_scene* getCurrentScene(t_sceneController* controller) {
    t_scene* scene = (t_scene*)getObject(controller->scene, controller->currentScene);
    executeSceneFunctions(scene, HANDLE_RESIZE);
    return scene;
}

void freeSceneByName(t_sceneController** controller, char* name) {
    if (controller == NULL || (*controller) == NULL) {
        printf("errur");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (*controller)->scene->count; i++) {
        t_scene* scene = (t_scene*)getObject((*controller)->scene, i);

        if (scene && strcmp(scene->name, name) == 0) {
            // Supprime proprement la scène
            printf("supprimer\n");
            deleteObject((*controller)->scene, i);

            // Réajuste l'index courant si nécessaire
            if ((*controller)->currentScene >= i) {
                (*controller)->currentScene--;
            }
        }
    }
}

void freeSceneController(t_sceneController** controller) {
    if (!controller || !*controller) return;

    // Parcours récursif pour libérer toutes les ressources
    freeObjectManager((*controller)->scene);
    if ((*controller)->lastMenu) {
        free((*controller)->lastMenu->historique);
        free((*controller)->lastMenu);
    }
    *controller = NULL;
}

int findObjectIndex(t_objectManager* manager, void* object) {
    for (int i = 0; i < manager->count; i++) {
        if (getObject(manager, i) == object) {
            return i;
        }
    }
    return -1;  // Objet non trouvé
}

void* sceneAddObject(t_scene* scene, void* object, uint8_t typeId) {
    if (!scene || !object) return NULL;

    // Ajouter l'objet au gestionnaire d'objets
    addObject(scene->objectManager, object, typeId);

    return object;
}

void registerFunctionForObject(t_scene* scene, void* object, uint8_t typeId,
                               t_fonctionType funcType, void (*fonct)(t_fonctionParam*),
                               int indexObj, ...) {
    if (!scene || !object) return;

    va_list args;
    va_start(args, indexObj);

    va_list count_args;
    va_copy(count_args, args);
    int param_count = 0;
    while (va_arg(count_args, void*) != NULL) param_count++;
    va_end(count_args);

    void** params = malloc(param_count * sizeof(void*));
    for (int i = 0; i < param_count; i++) {
        params[i] = va_arg(args, void*);
    }
    va_end(args);
    void** final_params;

    // Construire le tableau de paramètres finaux
    final_params = malloc((param_count + 1) * sizeof(void*));
    for (int j = 0; j < indexObj; j++) {
        final_params[j] = params[j];
    }
    final_params[indexObj] = object;
    for (int j = indexObj; j < param_count; j++) {
        final_params[j + 1] = params[j];
    }

    // Créer et ajouter la fonction
    t_fonctionParam* sf = malloc(sizeof(t_fonctionParam));
    sf->fonction = fonct;
    sf->param = final_params;
    sf->nb_param = param_count + 1;

    scene->fonctions[funcType] = realloc(scene->fonctions[funcType],
                                         (scene->nbFonctions[funcType] + 1) * sizeof(t_fonctionParam*));
    scene->fonctions[funcType][scene->nbFonctions[funcType]++] = sf;

    free(params);
}

void sceneRemoveObject(t_scene* scene, void* object) {
    if (!scene || !object) return;

    // Trouver l'index de l'objet
    int objectIndex = findObjectIndex(scene->objectManager, object);
    if (objectIndex < 0) return;  // Objet non trouvé

    // Parcourir tous les types de fonctions pour supprimer celles liées à cet objet
    for (int funcType = 0; funcType < NUM_FONCTION; funcType++) {
        for (int i = 0; i < scene->nbFonctions[funcType]; i++) {
            t_fonctionParam* func = scene->fonctions[funcType][i];

            // Vérifier si cette fonction utilise l'objet
            SDL_bool containsObject = SDL_FALSE;
            for (int j = 0; j < func->nb_param; j++) {
                if (func->param[j] == object) {
                    containsObject = SDL_TRUE;
                    break;
                }
            }

            if (containsObject) {
                // Libérer cette fonction
                freeFonction(&func);

                // Déplacer les fonctions restantes pour combler le trou
                for (int j = i; j < scene->nbFonctions[funcType] - 1; j++) {
                    scene->fonctions[funcType][j] = scene->fonctions[funcType][j + 1];
                }
                scene->nbFonctions[funcType]--;
                i--;  // Ajuster l'index pour ne pas sauter d'élément
            }
        }
    }

    // Supprimer l'objet du gestionnaire d'objets
    deleteObject(scene->objectManager, objectIndex);
}