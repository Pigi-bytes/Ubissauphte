/**
 * @file core.h
 * @brief Structures et fonctions de base pour la physique et les collisions
 */

#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

/**
 * @struct t_circle
 * @brief Représente un cercle pour les collisions circulaires
 */
typedef struct {
    float x, y;    ///< Position du centre du cercle
    float radius;  ///< Rayon du cercle
} t_circle;

/**
 * @struct t_sector
 * @brief Représente un secteur angulaire pour les détections dans un cône
 */
typedef struct {
    SDL_FPoint origin;  ///< Point d'origine du secteur
    float startAngle;   ///< Angle de départ en radians
    float endAngle;     ///< Angle de fin en radians
    float radius;       ///< Portée du secteur
} t_sector;

/**
 * @struct t_collisionData
 * @brief Contient les informations résultant d'une collision
 */
typedef struct {
    SDL_FPoint normal;  ///< Vecteur normal à la surface de collision
    float depth;        ///< Profondeur de pénétration
} t_collisionData;

/**
 * @def M_PI
 * @brief Constante Pi pour les calculs trigonométriques
 */
#define M_PI 3.14159265358979323846

/**
 * @def EPSILON
 * @brief Petite valeur pour comparer des flottants avec une tolérance
 */
#define EPSILON 0.001f

/**
 * @brief Interpole linéairement entre deux angles
 * @param a Premier angle en radians
 * @param b Second angle en radians
 * @param t Facteur d'interpolation (0.0 à 1.0)
 * @return Angle interpolé en radians
 */
float lerpAngle(float a, float b, float t);

/**
 * @brief Applique une fonction de lissage à une valeur
 * @param t Valeur à lisser (entre 0.0 et 1.0)
 * @return Valeur lissée selon la courbe de Hermite (3t² - 2t³)
 */
float smoothStepf(float t);

#endif