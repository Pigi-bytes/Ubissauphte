
/**
 * @file fichierLoader.h
 * @brief Lecture et chargement de donnees dans un fichier texte
 */
#ifndef FICHIER_LOADER_H
#define FICHIER_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/objectManager.h"

/**
 * @def MAX_LINE_LENGTH_FILE
 * @brief Taille maximal pour une ligne
 */
#define MAX_LINE_LENGTH_FILE 100

/**
 * @struct t_pairData
 * @brief Structure contenant des donnees sous la forme d'une cle et d'une valeur
 */
typedef struct {
    char *key;    ///< chaine de caractère pour la cle
    char *value;  ///< chaine de caractère pour la valeur
} t_pairData;

/**
 * @struct t_block
 * @brief Structure contenant des t_pairData sous la forme d'un block
 */
typedef struct {
    t_objectManager *pairManager;  ///< Tableau de t_pairData géré par un t_objetManager
} t_blockData;

/**
 * @struct t_block
 * @brief Structure contenant des t_block dans un tableau
 */
typedef struct {
    t_objectManager *blockManager;  ///<  Tableau de t_block géré par un t_objetManager
} t_fichier;

/**
 * @enum t_valueType
 * @brief Enum permettant de gérer le typage lors de la recherche d'une valeur dans un fichier
 */
typedef enum {
    INT,     ///< Recherche d'entier
    FLOAT,   ///< Recherche de flottant
    DOUBLE,  ///< Recherche de double
    STRING,  ///< Recherche d'une chaine de caractère
} t_valueType;

/**
 * @brief Récupère des donnees dans un fichier
 * @param filename nom du fichier contenant les donnes
 */
t_fichier *chargerFichier(char *filename);

/**
 * @brief Recherche une valeur dans un block de donnees
 * @param block Block contenant la valeur recherchée
 * @param name Nom de la valeur recherchée
 * @param result Prend une adresse et récupère la valeur recherchée
 * @param type permet de cast result dans le type de la valeur recherché e
 * @return Renvoie True si la valeur est bien récupéré sinon false
 */
SDL_bool getValue(t_blockData *block, char *name, void *result, t_valueType type);

/**
 * @brief Sauvegarde des donnees dans un fichier
 * @param fichier t_fichier contenant les donnees
 * @param filename Nom du fichier
 */
void saveFichier(t_fichier *fichier, char *filename);

/**
 * @brief Crée un nouveau t_block
 */
t_blockData *createNewBlock();

/**
 * @brief Création d'un t_pairData
 * @param key cle de la pair
 * @param value valeu de la pair
 */
t_pairData *createPairData(char *key, char *value);

/**
 * @brief Ajoute un block dans un t_fichier
 * @param fichier Fichier dans lequel on ajoute le block
 * @param Block Block ajouté
 */
void addBlock(t_fichier *fichier, t_blockData *Block);

/**
 * @brief Ajoute une Pair de data dans un block d'un t_fichier
 * @param block Block dans lequel on ajoute la data
 * @param pair data ajouté
 */
void addPairData(t_blockData *block, t_pairData *pair);

/**
 * @brief Libère la mémoire allouée pour un block
 * @param data donnees auxquels on à allouée de la mémoire
 */
void blockFreeFunc(void *data);

/**
 * @brief Libère la mémoire allouée pour une pair de donnee (cle/valeur)
 * @param data donnee à laquelle on à allouée de la mémoire
 */
void pairFreeFunc(void *data);

/**
 * @brief Libère la mémoire allouée pour un t_fichier
 * @param fichier t_fichier pour lequel on a alloué de la mémoire
 */
void freeFichier(t_fichier *fichier);

#endif
