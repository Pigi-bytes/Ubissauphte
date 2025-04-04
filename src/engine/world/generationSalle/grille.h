/**
 * @file grille.h
 * @brief Gestion des grilles et cases du jeu
 *
 * Ce module définit la structure des grilles utilisées dans a génerations des salles,
 * ainsi que les fonctions pour les manipuler et les gérer.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef GRILLE_H
#define GRILLE_H

#include <stdarg.h>

#include "listeBlock.h"

/*=== CONSTANTES ===*/

/**
 * @defgroup voisins_constants Constantes des voisins
 * @brief Définit les positions relatives des voisins d'une case
 * @{
 */
#define VOISIN_GAUCHE 0            ///< Voisin direct à gauche
#define VOISIN_DROIT 1             ///< Voisin direct à droite
#define VOISIN_BAS 2               ///< Voisin direct en bas
#define VOISIN_HAUT 3              ///< Voisin direct en haut
#define VOISIN_DIAG_GAUCHE_HAUT 4  ///< Voisin diagonal haut-gauche
#define VOISIN_DIAG_DROIT_HAUT 5   ///< Voisin diagonal haut-droite
#define VOISIN_DIAG_GAUCHE_BAS 6   ///< Voisin diagonal bas-gauche
#define VOISIN_DIAG_DROIT_BAS 7    ///< Voisin diagonal bas-droite

#define VOISIN_GAUCHE2 8             ///< Voisin à 2 cases à gauche
#define VOISIN_DROIT2 9              ///< Voisin à 2 cases à droite
#define VOISIN_BAS2 10               ///< Voisin à 2 cases en bas
#define VOISIN_HAUT2 11              ///< Voisin à 2 cases en haut
#define VOISIN_DIAG_GAUCHE_HAUT2 12  ///< Voisin diagonal haut-gauche à 2 cases
#define VOISIN_DIAG_DROIT_HAUT2 13   ///< Voisin diagonal haut-droite à 2 cases
#define VOISIN_DIAG_GAUCHE_BAS2 14   ///< Voisin diagonal bas-gauche à 2 cases
#define VOISIN_DIAG_DROIT_BAS2 15    ///< Voisin diagonal bas-droite à 2 cases

#define VOISIN_CENTRE_BAS2_DROIT 16   ///< Voisin spécial bas-droite à 2 cases
#define VOISIN_CENTRE_BAS2_GAUCHE 17  ///< Voisin spécial bas-gauche à 2 cases

/**
 * @defgroup type_case_constants Constantes des types de cases
 * @brief Définit les différents types de cases possibles
 * @{
 */
#define OBSTACLE 1   ///< Case de type obstacle
#define SOL 0        ///< Case de type sol
#define ELTAJOUTE 2  ///< Case spéciale ajoutée
#define SORTIE 3     ///< Case de type sortie

/*=== STRUCTURES ===*/

/**
 * @struct t_case
 * @brief Représente une case individuelle dans la grille
 */
typedef struct s_case {
    int i, j;                      ///< Coordonnées (x,y) dans la grille
    t_block* tiles;                ///< Bloc associé à cette case
    int val;                       ///< Valeur de la case (type)
    struct s_case* tabVoisin[18];  ///< Tableau des voisins (18 positions)
} t_case;

/**
 * @struct t_grille
 * @brief Représente une grille complète du jeu
 */
typedef struct s_grille {
    int nbLigne;       ///< Nombre de lignes
    int nbColonne;     ///< Nombre de colonnes
    t_case*** grille;  ///< Matrice 2D de cases
    char nom[100];     ///< Nom identifiant la grille
} t_grille;

/*=== MACROS UTILITAIRES ===*/

#ifndef CASE_NULL
#define CASE_NULL (&case_null)  ///< Case nulle par défaut
#endif

/**
 * @def EXISTE_VOISIN(c, ...)
 * @brief Macro pour vérifier l'existence de voisins
 * @param c Case de départ
 * @param ... Liste des positions des voisins à vérifier
 */
#define EXISTE_VOISIN(c, ...) existeVoisin(c, ##__VA_ARGS__, NULL)

/*=== FONCTIONS ===*/

/**
 * @brief Vérifie si des coordonnées sont dans la matrice
 * @param x Position x
 * @param y Position y
 * @param maxx Taille maximale en x
 * @param maxy Taille maximale en y
 * @return 1 si dans la matrice, 0 sinon
 */
int inMatrice(int x, int y, int maxx, int maxy);

/**
 * @brief Initialise une nouvelle case
 * @param i Position x
 * @param j Position y
 * @param val Valeur initiale
 * @return Pointeur vers la case créée
 */
t_case* initCase(int i, int j, int val);

/**
 * @brief Initialise une nouvelle grille
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @return Pointeur vers la grille créée
 */
t_grille* initGrille(int nbLigne, int nbColonne);

/**
 * @brief Ajoute une case à la grille
 * @param g Grille cible
 * @param i Position x
 * @param j Position y
 * @param val Valeur de la case
 */
void ajouterCase(t_grille* g, int i, int j, int val);

/**
 * @brief Libère la mémoire d'une grille
 * @param g Grille à libérer
 */
void freeGrille(t_grille* g);

/**
 * @brief Stocke les voisins d'une case
 * @param g Grille contenant la case
 * @param i Position x de la case
 * @param j Position y de la case
 * @param nbLigne Nombre total de lignes
 * @param nbColonne Nombre total de colonnes
 */
void stockerEtatVoisin(t_grille* g, int i, int j, int nbLigne, int nbColonne);

/**
 * @brief Convertit une matrice d'entiers en grille
 * @param entier Matrice source
 * @param nbLigne Nombre de lignes
 * @param nbColonne Nombre de colonnes
 * @return Nouvelle grille initialisée
 */
t_grille* intToGrilleNiveau(int** entier, int nbLigne, int nbColonne);

/**
 * @brief Vérifie si une case existe
 * @param c Case à vérifier
 * @return 1 si la case existe, 0 sinon
 */
int existe(const t_case* c);

/**
 * @brief Vérifie si des voisins existent
 * @param c Case de départ
 * @param ... Liste des positions des voisins (terminée par NULL)
 * @return 1 si tous les voisins existent, 0 sinon
 */
int existeVoisin(t_case* c, ...);

#endif