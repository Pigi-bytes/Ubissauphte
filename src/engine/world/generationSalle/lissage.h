/**
 * @file lissage.h
 * @brief Algorithmes de lissage pour la génération de cartes
 *
 * Ce module contient les fonctions de lissage et d'analyse
 * des motifs de cases pour améliorer la génération procédurale
 * des cartes du jeu.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef LISSAGE_H
#define LISSAGE_H

#include <SDL2/SDL.h>

#include "grille.h"

/*=== FONCTIONS DE BASE ===*/

/**
 * @brief Vérifie si une case n'a pas de voisins
 * @param c Pointeur vers la case à vérifier
 * @return SDL_TRUE si la case est isolée, SDL_FALSE sinon
 */
SDL_bool caseSansVoisins(t_case* c);

/**
 * @brief Vérifie si une case est de type sol
 * @param c Pointeur vers la case à vérifier
 * @return SDL_TRUE si c'est un sol, SDL_FALSE sinon
 */
SDL_bool caseIsSol(t_case* c);

/**
 * @brief Vérifie si une case est de type plafond
 * @param c Pointeur vers la case à vérifier
 * @return SDL_TRUE si c'est un plafond, SDL_FALSE sinon
 */
SDL_bool caseIsPlafond(t_case* c);

/*=== FONCTIONS DE RÈGLES ===*/

/**
 * @brief Vérifie si une case est isolée
 * @param c Pointeur vers la case à analyser
 * @return SDL_TRUE si la case est isolée, SDL_FALSE sinon
 */
SDL_bool isIsolatedCase(t_case* c);

/**
 * @brief Vérifie les motifs horizontaux/verticaux
 * @param c Pointeur vers la case à analyser
 * @return SDL_TRUE si un motif est détecté, SDL_FALSE sinon
 */
SDL_bool isHorizontalOrVerticalPath(t_case* c);

/**
 * @brief Détermine si un mur arrière doit être ajouté
 * @param c Pointeur vers la case à analyser
 * @return SDL_TRUE si un mur arrière est nécessaire, SDL_FALSE sinon
 */
SDL_bool shouldAddBackWall(t_case* c);

/**
 * @brief Détecte un motif diagonal supérieur
 * @param c Pointeur vers la case à analyser
 * @return SDL_TRUE si le motif est détecté, SDL_FALSE sinon
 */
SDL_bool isDiagonalTopPattern(t_case* c);

/**
 * @brief Détecte un motif diagonal en coin
 * @param c Pointeur vers la case à analyser
 * @return SDL_TRUE si le motif est détecté, SDL_FALSE sinon
 */
SDL_bool isDiagonalCornerPattern(t_case* c);

/**
 * @brief Détecte un motif diagonal inférieur
 * @param c Pointeur vers la case à analyser
 * @return SDL_TRUE si le motif est détecté, SDL_FALSE sinon
 */
SDL_bool isDiagonalBottomPattern(t_case* c);

/*=== FONCTIONS D'AFFECTATION ===*/

/**
 * @brief Applique le traitement pour un motif diagonal en coin
 * @param grille Pointeur vers la grille
 * @param i Coordonnée x de la case
 * @param j Coordonnée y de la case
 */
void handleDiagonalCornerPattern(t_grille* grille, int i, int j);

/**
 * @brief Applique le traitement pour un motif diagonal inférieur
 * @param grille Pointeur vers la grille
 * @param i Coordonnée x de la case
 * @param j Coordonnée y de la case
 */
void handleDiagonalBottomPattern(t_grille* grille, int i, int j);

/*=== FONCTION PRINCIPALE ===*/

/**
 * @brief Applique le lissage sur toute la grille
 * @param grille Pointeur vers la grille à lisser
 *
 * Cette fonction analyse chaque case de la grille et applique
 * les règles de lissage pour améliorer la cohérence visuelle.
 */
void lissage(t_grille* grille);

#endif