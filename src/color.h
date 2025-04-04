/**
 * @file color.h
 * @brief Définition des couleurs utilisées dans le projet
 *
 * Ce fichier contient les définitions des couleurs de base
 * sous forme de macros SDL_Color pour une utilisation aisée
 * dans le rendu graphique.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*=== COULEURS PRÉDÉFINIES ===*/

/**
 * @def WHITE
 * @brief Couleur blanche (RGB: 255,255,255)
 */
#define WHITE ((SDL_Color){255, 255, 255, 255})

/**
 * @def BLACK
 * @brief Couleur noire (RGB: 0,0,0)
 */
#define BLACK ((SDL_Color){0, 0, 0, 255})

/**
 * @def MAGENTA
 * @brief Couleur magenta (RGB: 255,0,255)
 */
#define MAGENTA ((SDL_Color){255, 0, 255, 255})

/**
 * @def BLUE
 * @brief Couleur bleue (RGB: 0,0,255)
 */
#define BLUE ((SDL_Color){0, 0, 255, 255})

/**
 * @def GREEN
 * @brief Couleur verte (RGB: 0,255,0)
 */
#define GREEN ((SDL_Color){0, 255, 0, 255})

/**
 * @def RED
 * @brief Couleur rouge (RGB: 255,0,0)
 */
#define RED ((SDL_Color){255, 0, 0, 255})

#endif