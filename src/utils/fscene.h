/**
 * @file scene.h
 * @brief Gestion des scènes et système de composition
 *
 * Ce module permet de gérer des scènes composées d'objets
 * avec un système de fonctions enregistrables pour :
 * - Le rendu
 * - Les entrées
 * - La mise à jour
 * - Le redimensionnement
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

#include "../debug.h"
#include "../io/input.h"
#include "fonctionManager.h"
#include "objectManager.h"

/*=== MACROS UTILITAIRES ===*/

/**
 * @def ADD_OBJECT_TO_SCENE
 * @brief Macro pour ajouter un objet à une scène
 * @param scene Scène cible
 * @param obj Objet à ajouter
 * @param type Type de l'objet
 */
#define ADD_OBJECT_TO_SCENE(scene, obj, type) addObject(scene->objectManager, obj, type)

/*=== TYPES DE FONCTIONS ===*/

/**
 * @enum t_fonctionType
 * @brief Types de fonctions gérées par le système de scènes
 */
typedef enum {
    RENDER_GAME,    ///< Rendu du jeu (monde)
    RENDER_UI,      ///< Rendu de l'interface
    HANDLE_INPUT,   ///< Gestion des entrées
    HANDLE_RESIZE,  ///< Gestion du redimensionnement
    UPDATE,         ///< Mise à jour logique
    SET_BUFFER,     ///< Configuration des buffers
    RENDER_BUFFER,  ///< Rendu des buffers
    NUM_FONCTION    ///< Nombre total de types
} t_fonctionType;

/*=== STRUCTURES PRINCIPALES ===*/

/**
 * @struct t_scene
 * @brief Représente une scène composée d'objets
 */
typedef struct t_scene {
    char* name;                                 ///< Nom identifiant la scène
    t_objectManager* objectManager;             ///< Gestionnaire d'objets
    t_fonctionParam** fonctions[NUM_FONCTION];  ///< Tableau des fonctions liè à la scène
    int nbFonctions[NUM_FONCTION];              ///< Nombre de fonctions par type
} t_scene;

/**
 * @struct t_history
 * @brief Historique des scènes visitées
 */
typedef struct {
    t_scene** historique;  ///< Pile des scènes visitées
    int nb_Menu;           ///< Nombre de scènes dans l'historique
} t_history;

/**
 * @struct t_sceneController
 * @brief Contrôleur central des scènes
 */
typedef struct {
    t_objectManager* scene;  ///< Gestionnaire de scènes
    int currentScene;        ///< Index de la scène courante
    t_scene* lastMap;        ///< Dernière salle du niveau visitée
    t_history* lastMenu;     ///< Historique des menus
} t_sceneController;

/*=== FONCTIONS DE GESTION DES SCENES ===*/

/**
 * @brief Crée une nouvelle scène
 * @param objectManager Gestionnaire d'objets de la scène
 * @param name Nom identifiant la scène
 * @return Pointeur vers la scène créée
 */
t_scene* createScene(t_objectManager* objectManager, char* name);

/**
 * @brief Enregistre une fonction pour une scène
 * @param scene Scène cible
 * @param typeObject Type d'objet concerné
 * @param typeFunction Type de fonction à enregistrer
 * @param fonct wrapper vers la fonction
 * @param indexObj Index de l'objet de type typedFunction (-1 si non utilisé)
 * @param ... paramètre de la fonction à enregistré
 */
void sceneRegisterFunction(t_scene* scene, uint8_t typeObject,
                           t_fonctionType typeFunction,
                           void (*fonct)(t_fonctionParam*),
                           int indexObj, ...);

/**
 * @brief Exécute toutes les fonctions d'un type donné
 * @param scene Scène cible
 * @param ftype Type de fonction à exécuter
 */
void executeSceneFunctions(t_scene* scene, t_fonctionType ftype);

/**
 * @brief Libère une scène
 * @param object Scène à libérer (passé en void* pour compatibilité avec l'objectManager)
 */
void freeScene(void* object);

/*=== FONCTIONS DU CONTROLEUR ===*/

/**
 * @brief Initialise un nouveau contrôleur de scènes
 * @return Nouveau contrôleur initialisé
 */
t_sceneController* initSceneController();

/**
 * @brief Ajoute une scène au contrôleur
 * @param controller Contrôleur cible
 * @param scene Scène à ajouter
 */
void addScene(t_sceneController* controller, t_scene* scene);

/**
 * @brief Change la scène courante
 * @param controller Contrôleur cible
 * @param name Nom de la scène à activer
 */
void setScene(t_sceneController* controller, char* name);

/**
 * @brief Récupère la scène courante
 * @param voidcontroller Contrôleur
 * @return Scène actuellement active
 */
t_scene* getCurrentScene(t_sceneController* controller);

/**
 * @brief Libère un contrôleur de scènes
 * @param controller Double pointeur vers le contrôleur
 */
void freeSceneController(t_sceneController** controller);

/*=== FONCTIONS AVANCEES ===*/

/**
 * @brief Ajoute un objet à une scène avec enregistrement automatique
 * @param scene Scène cible
 * @param object Objet à ajouter
 * @param typeId Type de l'objet
 * @return Pointeur vers l'objet ajouté
 */
void* sceneAddObject(t_scene* scene, void* object, uint8_t typeId);

/**
 * @brief Supprime un objet d'une scène
 * @param scene Scène cible
 * @param object Objet à supprimer
 */
void sceneRemoveObject(t_scene* scene, void* object);

/**
 * @brief Enregistre une fonction pour un objet spécifique
 * @param scene Scène cible
 * @param object Objet concerné
 * @param typeId Type de l'objet
 * @param funcType Type de fonction
 * @param fonct Fonction callback
 * @param indexObj Index de l'objet
 * @param ... liste des paramètre utils à l'éxecution de la fonction
 */
void registerFunctionForObject(t_scene* scene, void* object, uint8_t typeId,
                               t_fonctionType funcType,
                               void (*fonct)(t_fonctionParam*),
                               int indexObj, ...);

/**
 * @brief Trouve l'index d'un objet dans son gestionnaire
 * @param manager Gestionnaire d'objets
 * @param object Objet à rechercher
 * @return Index de l'objet ou -1 si non trouvé
 */
int findObjectIndex(t_objectManager* manager, void* object);

/**
 * @brief Revient à la scène précédente
 * @param controller Contrôleur cible
 */
void getPrevuisScene(t_sceneController* controller);

/**
 * @brief Revient au menu précédent
 * @param controller Contrôleur cible
 */
void getPrevuisMenu(t_sceneController* controller);

/**
 * @brief Récupère l'index d'une scène
 * @param controller Contrôleur cible
 * @param scene Scène à rechercher
 * @return Index de la scène ou -1 si non trouvée
 */
int indiceByscene(t_sceneController* controller, t_scene* scene);

/**
 * @brief Libère une scène par son nom
 * @param controller Double pointeur vers le contrôleur
 * @param name Nom de la scène à libérer
 */
void freeSceneByName(t_sceneController** controller, char* name);

/**
 * @brief Récupère une scène par son nom
 * @param controller Contrôleur cible
 * @param name Nom de la scène
 * @return Scène trouvée ou NULL
 */
t_scene* GetSceneByName(t_sceneController* controller, char* name);

#endif