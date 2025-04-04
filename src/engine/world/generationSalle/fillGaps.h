/**
 * @file fillgaps.h
 * @brief Remplissage des trous dans les grilles de jeu
 *
 * Ce module fournit des fonctions pour détecter et combler les trous
 * dans les matrices représentant les cartes du jeu.
 * Il utilise des structures de pile et des algorithmes d'écoulement par bloc.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef FILLGAPS_H
#define FILLGAPS_H

#include <SDL2/SDL.h>

#include "grille.h"

/*=== STRUCTURE DE DONNEES ===*/

/**
 * @struct t_element
 * @brief Element d'une pile de points (pour parcours de matrice)
 */
typedef struct s_element {
    SDL_Point *point;             ///< Coordonnées du point
    struct s_element *precedent;  ///< Element précédent dans la pile
} t_element;

/*=== OPERATIONS SUR PILES ===*/

/**
 * @brief Initialise la pile globale
 */
void initPile();

/**
 * @brief Vérifie si la pile est vide
 * @return 1 si vide, 0 sinon
 */
int PileVide();

/**
 * @brief Empile un point
 * @param p Point à empiler
 */
void empiler(SDL_Point *p);

/**
 * @brief Dépile un point
 * @return Point dépilé ou NULL si pile vide
 */
SDL_Point *depiler();

/**
 * @brief Vide complètement la pile
 */
void viderPile();

/*=== FONCTIONS DE REMPLISSAGE ===*/

/**
 * @brief Crée une matrice de float
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @return Matrice allouée
 */
float **createMatriceFloat(int nbLigne, int nbColonne);

/**
 * @brief Convertit une matrice float en int
 * @param mat Matrice source
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @return Nouvelle matrice d'entiers
 */
int **matriceFloat2Int(float **mat, int nbLigne, int nbColonne);

/**
 * @brief Libère une matrice de float
 * @param mat Matrice à libérer
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 */
void freeMatriceFloat(float **mat, int nbLigne, int nbColonne);

/**
 * @brief Libère une matrice d'entiers
 * @param mat Matrice à libérer
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 */
void freeMatriceInt(int **mat, int nbLigne, int nbColonne);

/**
 * @brief Vérifie si une matrice ne contient pas de 0
 * @param mat Matrice à vérifier
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @return SDL_TRUE si aucun 0, SDL_FALSE sinon
 */
SDL_bool matriceSans0(int **mat, int nbLigne, int nbColonne);

/**
 * @brief Compte les éléments dans un bloc continu
 * @param mat Matrice source
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @param num_Block Numéro du bloc à compter
 * @return Nombre d'éléments dans le bloc
 */
int nombreElementsInBlock(int **mat, int nbLigne, int nbColonne, int num_Block);

/**
 * @brief Rend la matrice continue (remplit les trous)
 * @param mat Matrice à modifier
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 */
void rendreMatriceContinue(int **mat, int nbLigne, int nbColonne);

/**
 * @brief Comble un trou spécifique dans la matrice
 * @param mat Matrice à modifier
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @param copie Copie de la matrice originale
 * @param num_Block Numéro du bloc à remplir
 */
void comblerTrou(int **mat, int nbLigne, int nbColonne, int **copie, int num_Block);

#endif