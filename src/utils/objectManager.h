#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"

// Iterateur specialisé par type pour un accés O(1) au lieu de O(n)
// Ressource utilisé:
// - https://peerdh.com/blogs/programming-insights/memory-pool-management-techniques-in-c
// - https://en.wikipedia.org/wiki/Memory_pool
// - https://8dcc.github.io/programming/pool-allocator.html <- Version super basique

/**
 * @typedef freeFunc
 * @brief Pointeur de fonction pour liberer un objet
 * @param ptr Pointeur vers l'objet a liberer
 */
typedef void (*freeFunc)(void*);

/**
 * @def POOL_SIZE
 * @brief Taille fixe de chaque bloc memoire
 */
#define POOL_SIZE 64

/**
 * @def MAX_UINT8_T
 * @brief Nombre maximal de types supportés (valeur max uint8_t)
 */
#define MAX_UINT8_T 256

/**
 * @struct t_typeMetadata
 * @brief Metadonneée d'un type enregistré (son nom et sa fonction de libération)
 */
typedef struct {
    freeFunc freeFunc;  ///< Fonction de libération spécifique au type
    char* name;         ///< Nom du type (aucune utilité juste pour debug)
} t_typeMetadata;

/**
 * @struct t_typeRegistry
 * @brief Registre central des types
 *
 * Maintient une collection de tous les types enregistrés avec leurs métadonnées. Limité à MAX_UINT8_T types.
 */
typedef struct {
    t_typeMetadata types[256];  ///< Tableau des Metadonneée par ID de type
    uint8_t nextTypeId;         ///< Prochain ID disponible pour l'enregistrement
} t_typeRegistry;

/**
 * @brief Initialise un registre de types
 * @param registre Pointeur vers le registre à initialiser
 */
void initTypeRegistry(t_typeRegistry* registre);

/**
 * @brief Enregistre un nouveau type dans le registre
 * @param registre Registre cible
 * @param freeFunc Fonction de libération pour ce type
 * @param name Nom descriptif du type
 * @return uint8_t ID assigné au type
 */
uint8_t registerType(t_typeRegistry* registre, freeFunc freeFunc, char* name);

/**
 * @struct t_typedObject
 * @brief Objet type stocké dans le pool
 */
typedef struct {
    void* data;      ///< Pointeur vers les données de l'objet
    uint8_t typeId;  ///< ID du type associé dans le registre
} t_typedObject;

/**
 * @struct t_objectMemoryPool
 * @brief Bloc memoire pour stockage d'objets
 *
 * Chaque pool contient POOL_SIZE objets et un pointeur vers le prochain bloc si besoin
 */
typedef struct t_objectMemoryPool {
    t_typedObject items[POOL_SIZE];   ///< Slots de stockage d'objets
    struct t_objectMemoryPool* next;  ///< Prochain bloc dans la chaîne
} t_objectMemoryPool;

/**
 * @struct t_objectManager
 * @brief Gestionnaire des objets
 */
typedef struct {
    t_objectMemoryPool* firstPool;    ///< Premier bloc de memoire
    t_objectMemoryPool* currentPool;  ///< Dernier bloc de memoire alloué
    int nbItemsInPool;                ///< Nombre d'objet dans le currentPool
    int count;                        ///< Nombre d'objet total
    t_typeRegistry* registry;         ///< Registre des types
} t_objectManager;

/**
 * @brief Initialise un nouveau gestionnaire de memoire
 * @param registre Registre de types a utiliser
 * @return Une Instance initialisée
 */
t_objectManager* initObjectManager(t_typeRegistry* registre);

/**
 * @brief Ajoute un objet au gestionnaire
 * @param manager Gestionnaire cible
 * @param data Données de l'objet (deja malloc)
 * @param typeId ID du type enregistré pour cet objet
 */
void addObject(t_objectManager* manager, void* data, uint8_t typeId);

/**
 * @brief Récupère un objet à partir de son index dans le gestionnaire d'objets.
 *
 * @param manager Pointeur vers le gestionnaire d'objets.
 * @param index Index de l'objet à récupérer (commence à 0).
 * @return Pointeur vers l'objet correspondant à l'index, ou NULL si l'index est invalide.
 */
void* getObject(t_objectManager* manager, int index);

/**
 * @brief Libere toute la memoire du gestionnaire et ses objets
 * @param manager Gestionnaire a détruire
 *
 * Parcourt tous les blocs et applique la fonction free
 * spécifique à chaque type
 */
void freeObjectManager(t_objectManager* manager);

#endif  // OBJECT_MANAGER_H
