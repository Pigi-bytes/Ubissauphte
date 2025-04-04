/**
 * @file perlin.h
 * @brief Générateur de bruit de Perlin en 2D
 *
 * Ce module implémente un générateur de bruit procédural
 * utilisant l'algorithme de Perlin pour créer des textures
 * et motifs organiques.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef PERLIN_H
#define PERLIN_H

#include <SDL2/SDL.h>

/*=== FONCTIONS MATHÉMATIQUES ===*/

/**
 * @brief Calcule le sinus d'un point (coordonnées x et y)
 * @param a Point d'entrée
 * @return Point avec x=sin(a.x) et y=sin(a.y)
 */
SDL_FPoint pointSin(SDL_FPoint a);

/**
 * @brief Produit scalaire entre deux points
 * @param a Premier point
 * @param b Deuxième point
 * @return Valeur du produit scalaire
 */
float dot(SDL_FPoint a, SDL_FPoint b);

/**
 * @brief Partie fractionnaire d'un float
 * @param val Valeur d'entrée
 * @return Partie fractionnaire (val - floor(val))
 */
float floatFract(float val);

/*=== GÉNÉRATION ALÉATOIRE ===*/

/**
 * @brief Génère un float pseudo-aléatoire basé sur une position et une seed
 * @param st Position d'entrée
 * @param seed Graine aléatoire
 * @return Valeur pseudo-aléatoire entre 0.0 et 1.0
 */
float randomFloat(SDL_FPoint st, SDL_FPoint seed);

/*=== OPERATIONS SUR POINTS ===*/

/**
 * @brief Arrondi chaque composante d'un point vers le bas
 * @param st Point d'entrée
 * @return Point avec x=floor(st.x) et y=floor(st.y)
 */
SDL_FPoint pointFloor(SDL_FPoint st);

/**
 * @brief Partie fractionnaire d'un point
 * @param st Point d'entrée
 * @return Point où chaque composante = fract(st.x) et fract(st.y)
 */
SDL_FPoint pointFract(SDL_FPoint st);

/**
 * @brief Applique une interpolation lissée (smoothstep) à un point
 * @param st Point d'entrée
 * @return Point avec smoothstep appliqué à chaque composante
 */
SDL_FPoint smoothStep(SDL_FPoint st);

/*=== INTERPOLATION ===*/

/**
 * @brief Interpolation linéaire entre deux valeurs
 * @param a Première valeur
 * @param b Deuxième valeur
 * @param u Point de contrôle de l'interpolation
 * @return Valeur interpolée
 */
float interpo(float a, float b, SDL_FPoint u);

/*=== FONCTION PRINCIPALE ===*/

/**
 * @brief Génère du bruit de Perlin 2D
 * @param st Coordonnées d'entrée
 * @param seed Graine pour la génération aléatoire
 * @return Valeur de bruit entre -1.0 et 1.0
 */
float noise(SDL_FPoint st, SDL_FPoint seed);

#endif