/**
 * @file listeBlock.h
 * @brief Gestion des blocs et tuiles du jeu
 *
 * Ce module définit les différents types de blocs utilisés dans le jeu
 * et fournit les fonctions pour les manipuler et les gérer.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef LISTB_H
#define LISTB_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBTYPE 7  ///< Nombre de types de blocs différents

/*=== ENUMERATIONS ===*/

/**
 * @enum nom_block
 * @brief Enumération de tous les types de blocs disponibles
 */
typedef enum {
    /* Blocs de mur */
    MUR_ANGLE_DROIT,
    MUR_ANGLE_GAUCHE,
    MUR_ARRONDI_INF_GAUCHE,
    MUR_ARRONDI_INF_DROIT,
    MUR_ARRONDI_SUP_GAUCHE,
    MUR_ARRONDI_SUP_DROIT,
    MUR_ANGLE_CONTINUE_GAUCHE,
    MUR_ANGLE_CONTINUE_DROIT,
    MUR_BORDURE_MUR_AVANT,
    MUR_BORDURE_MUR_AVANT_PILONNE,
    MUR_BORDURE_MUR_ARRIERE,
    MUR_BORDURE_MUR_GAUCHE,
    MUR_BORDURE_MUR_DROIT,

    /* Blocs de sol */
    SOL_SIMPLE,
    SOL_FRACTURE,
    SOL_POINT,
    SOL_OMBRE_MUR,
    SOL_OMBRE_MUR2,
    SOL_OMBRE_MUR_GAUCHE,
    SOL_OMBRE_MUR_GAUCHE2,
    SOL_OMBRE_ARRONDI_GAUCHE,
    SOL_OMBRE_ANGLE_GAUCHE,
    SOL_BAS_PILONNE,
    SOL_BAS_FONTAINE_EAUX_PROFOND,
    SOL_BAS_FONTAINE_EAUX_GRILLE,
    SOL_BAS_FONTAINE_SANS_EAUX_PROFOND,
    SOL_BAS_FONTAINE_SANS_EAUX_GRILLE,
    SOL_BAS_BOITE,
    SOL_TOMBE,

    /* Blocs frontaux */
    FRONTALE_BRIQUE_SIMPLE,
    FRONTALE_BRIQUE_FENETRE,
    FRONTALE_DRAPEAU,
    FRONTALE_FONTAINE_EAUX,
    FRONTALE_FONTAINE_PAS_EAUX,
    FRONTALE_ANGLE_GAUCHE,
    FRONTALE_ANGLE_DROIT,
    FRONTALE_CORP_PILONNE,
    FRONTALE_PORTE_SOLO,
    FRONTALE_PORTE_DUO_GAUCHE,
    FRONTALE_PORTE_DUO_DROITE,

    /* Blocs de plafond */
    PLAFOND_SIMPLE,
    PLAFOND_FRACTURE,
    PLAFOND_POINT,

    /* Blocs décoratifs */
    DECO_HAUT_BOITE,
    DECO_CROIX_TOMBE,
    DECO_PIERRE_TOMBALE,
    DECO_TABLE,
    DECO_ENCLUME,
    COMPDECO_TONNEAU,
    DECO_PIQUE,

    /* Blocs composites décoratifs */
    COMPDECO_DEBUT_ESCALIER,
    COMPDECO_MILLIEU_ESCALIER,
    COMPDECO_FIN_ESCALIER,
    COMPDECO_SOLO_ESCALIER,
    COMPDECO_COFFRE,

    /* Directions */
    DIRECTION_DROITE,
    DIRECTION_GAUCHE,
    DIRECTION_BAS,
    DIRECTION_HAUT,
} nom_block;

/**
 * @enum t_blocktype
 * @brief Types principaux de blocs
 */
typedef enum {
    MUR_TYPE,       ///< Bloc de mur
    PLAFOND_TYPE,   ///< Bloc de plafond
    FRONTAL_TYPE,   ///< Bloc frontal
    SOL_TYPE,       ///< Bloc de sol
    DECO_TYPE,      ///< Bloc décoratif
    COMPDECO_TYPE,  ///< Bloc composite décoratif
    DIRECTION_TYPE  ///< Direction
} t_blocktype;

/*=== STRUCTURES ===*/

/**
 * @struct t_block
 * @brief Représente un bloc individuel
 */
typedef struct {
    int tiles;                  ///< Index du bloc dans la texture
    int rotation;               ///< Rotation actuelle du bloc
    SDL_bool collisions;        ///< Si le bloc a des collisions
    t_blocktype type;           ///< Type principal du bloc
    double proba;               ///< Probabilité d'apparition
    nom_block name;             ///< Nom spécifique du bloc
    SDL_bool rotationAutorise;  ///< Si la rotation est autorisée
} t_block;

/**
 * @struct t_listeBlock
 * @brief Liste de blocs
 */
typedef struct {
    int nbElem;            ///< Nombre d'éléments dans la liste
    t_block** listeBlock;  ///< Tableau de pointeurs vers des blocs
} t_listeBlock;

/*=== FONCTIONS ===*/

/**
 * @brief Initialise toutes les listes de blocs
 * @return Tableau de listes de blocs organisé par type
 */
t_listeBlock** InitAllBlock();

/**
 * @brief Libère la mémoire des listes de blocs
 * @param listAllBlock Listes de blocs à libérer
 */
void freeListeBlock(t_listeBlock** listAllBlock);

/**
 * @brief Ajoute un bloc à la liste correspondante
 * @param listAllBlock Listes de blocs
 * @param type Type du bloc à ajouter
 * @param name Nom spécifique du bloc
 * @param tailes Index du bloc
 * @param rotation Rotation initiale
 * @param proba Probabilité d'apparition
 * @param collisions Si le bloc a des collisions
 * @param rotationAutorise Si la rotation est autorisée
 */
void ajouterBlock(t_listeBlock** listAllBlock, t_blocktype type, nom_block name,
                  int tailes, int rotation, double proba,
                  SDL_bool collisions, SDL_bool rotationAutorise);

/**
 * @brief Récupère la liste des blocs d'un type spécifique
 * @param listAllBlock Listes complètes de blocs
 * @param type Type de bloc recherché
 * @return Liste des blocs du type demandé
 */
t_listeBlock* listeByType(t_listeBlock** listAllBlock, t_blocktype type);

/**
 * @brief Applique une rotation aléatoire à un bloc
 * @param block Bloc à faire tourner
 */
void rotationAleatoire(t_block* block);

/**
 * @brief Trouve un bloc par son nom
 * @param listBlock Liste de blocs à chercher
 * @param name Nom du bloc recherché
 * @return Pointeur vers le bloc trouvé ou NULL
 */
t_block* blockByName(t_listeBlock* listBlock, nom_block name);

/**
 * @brief Vérifie si un bloc est d'un type spécifique
 * @param block Bloc à vérifier
 * @param nom Nom à comparer
 * @return SDL_TRUE si le bloc correspond, SDL_FALSE sinon
 */
SDL_bool blockIs(t_block* block, nom_block nom);

/**
 * @brief Vérifie si un bloc est une ombre
 * @param block Bloc à vérifier
 * @return SDL_TRUE si c'est une ombre, SDL_FALSE sinon
 */
SDL_bool blockIsOmbre(t_block* block);

/**
 * @brief Sélectionne un bloc aléatoire selon son type
 * @param listBlock Liste de blocs
 * @return Bloc sélectionné aléatoirement
 */
t_block* randomBlocByType(t_listeBlock* listBlock);

/**
 * @brief Crée les listes de tous les blocs
 * @return Listes de blocs initialisées
 */
t_listeBlock** createListAllBlock();

/**
 * @brief Copie les valeurs d'un bloc vers un autre
 * @param src Bloc source
 * @param det Bloc destination (double pointeur pour allocation)
 */
void copierVal(t_block* src, t_block** det);

#endif