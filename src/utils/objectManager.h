/**
 * @file object_manager.h
 * @brief Gestionnaire d'objets typés avec un systeme de free automatique
 *
 * On veut eviter d'avoir a update la longeur de la list h24
 * On a un index capacité (le nombre d'element max que l'on peut ajouter) et count le nombre d'element deja ajouté
 * on double la capacité au lieu de rajouté +1 => pour 256 texture par exemple on fait seulement 3 realloc pour 224 (en partant de 32)
 */
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"
/**
 * @def INITIAL_CAPACITY
 * @brief Capacité initiale par défaut d'un gestionnaire
 */
#define INITIAL_CAPACITY 10

/**
 * @macro ADD_TYPED_OBJECT
 * @brief Macro helper pour ajouter un objet typé à un gestionnaire
 * @param manager Gestionnaire cible
 * @param type Type d'objet à créer
 * @param value_ptr Pointeur vers l'objet
 */
#define ADD_TYPED_OBJECT(manager, type, value_ptr) addObject(manager, createTypedObject(type, value_ptr))

/**
 * @enum t_objectType
 * @brief Liste des types d'objets gérés par le système
 */
typedef enum {
    BUTTON_TYPE,       ///< Objet de type bouton UI
    SDL_TEXTURE_TYPE,  ///< Objet de texture SDL
    TEXT_TYPE,         ///< Objet de texte UI
    NB_OF_TYPE         ///< Nombre de type total
} t_objectType;

/**
 * @struct t_typedObject
 * @brief Objet générique typé avec pointeur vers ses données
 */
typedef struct {
    t_objectType type;  ///< Type de l'objet (détermine l'interprétation des données)
    void* data;         ///< Pointeur générique vers les données de l'objet
} t_typedObject;

/**
 * @struct t_objectManager
 * @brief Gestionnaire d'objets d'un type spécifique
 *
 * Gère automatiquement l'allocation mémoire et fournit des helpers d'ajout/récupération
 */
typedef struct {
    t_typedObject** items;    ///< Tableau dynamique de pointeurs d'objets
    int count;                ///< Nombre d'objets actuellement stockés
    int capacity;             ///< Capacité actuelle du tableau
    t_objectType type;        ///< Type des objets gérés par ce manager
    void (*freeFunc)(void*);  ///< Fonction de libération mémoire spécifique au type
} t_objectManager;

/**
 * @brief Initialise un nouveau gestionnaire d'objets
 * @param type Type des objets à gérer
 * @param freeFunc Fonction de libération mémoire pour ce type d'objet
 * @param capacity Capacité initiale, double a chaque fois que la capacité est atteinte
 * @return Pointeur vers le gestionnaire créé, Crash du programme en cas d'erreur
 *
 * @warning Doit être libéré avec freeObjectManager()
 */
t_objectManager* initObjectManager(t_objectType type, void (*freeFunc)(void*), int capacity);

/**
 * @brief Crée un objet typé avec des données
 * @param type Type de l'objet à créer
 * @param data Pointeur vers les données de l'objet
 * @return Pointeur vers l'objet créé, Crash du programme en cas d'erreur
 */
t_typedObject* createTypedObject(t_objectType type, void* data);

/**
 * @brief Ajoute un objet au gestionnaire (redimensionne en doublant la taille si besoin)
 * @param manager Gestionnaire cible
 * @param object Objet à ajouter
 *
 * @warning En cas d'erreur d'allocation, Crash du programme
 */
void addObject(t_objectManager* manager, t_typedObject* object);

/**
 * @brief Récupère un objet du gestionnaire par index
 * @param manager Gestionnaire cible
 * @param index Index de l'objet (doit être < count)
 * @return Pointeur vers les données de l'objet, NULL si index invalide
 */
void* getObject(t_objectManager* manager, size_t index);

/**
 * @brief Libère complètement un gestionnaire et tous ses objets
 * @param manager Pointeur vers le pointeur du gestionnaire
 *
 * @details Utilise la fonction freeFunc passée à l'initialisation
 * @note Met le pointeur manager à NULL après libération
 */
void freeObjectManager(t_objectManager** manager);

#endif  // OBJECT_MANAGER_H
