/**
 * @file text.h
 * @brief Gestion du rendu de texte avec SDL2_ttf
 *
 * Ce module fournit des fonctions pour créer, mettre à jour, afficher et
 * libérer des éléments textuels dans l'interface graphique.
 */

#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"

/**
 * @struct t_text
 * @brief Structure représentant un élément textuel dans l'interface
 */
typedef struct {
    char* text;            ///< Texte à afficher
    SDL_Texture* texture;  ///< Texture SDL contenant le rendu du texte
    TTF_Font* font;        ///< Police utilisée pour le rendu
    SDL_Rect rect;         ///< Position et dimensions du texte à l'écran
} t_text;

/**
 * @brief Charge une police depuis un fichier avec une taille spécifiée
 * @param filename Chemin vers le fichier de police
 * @param size Taille de la police en points
 * @return Pointeur vers la police chargée
 */
TTF_Font* loadFont(char* filename, int size);

/**
 * @brief Initialise le moteur de rendu de texte
 * @return SDL_TRUE si l'initialisation a réussi, SDL_FALSE sinon
 */
SDL_bool initTextEngine();

/**
 * @brief Crée un texte avec contour
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param text Chaîne de caractères à afficher
 * @param font Police à utiliser
 * @param color Couleur du texte
 * @param colorOutline Couleur du contour
 * @param outlineSize Épaisseur du contour en pixels
 * @return Pointeur vers la structure de texte créée
 */
t_text* createTextOutline(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color, SDL_Color colorOutline, int outlineSize);

/**
 * @brief Met à jour un texte avec contour existant
 * @param text Pointeur vers le pointeur de texte à mettre à jour
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param newText Nouveau texte à afficher
 * @param color Couleur du texte
 * @param CouleurOutline Couleur du contour
 * @param sizeOutline Épaisseur du contour en pixels
 */
void updateTextOutline(t_text** text, SDL_Renderer* renderer, char* newText, SDL_Color color, SDL_Color CouleurOutline, int sizeOutline);

/**
 * @brief Crée un texte simple sans contour
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param text Chaîne de caractères à afficher
 * @param font Police à utiliser
 * @param color Couleur du texte
 * @return Pointeur vers la structure de texte créée
 */
t_text* createText(SDL_Renderer* renderer, char* text, TTF_Font* font, SDL_Color color);

/**
 * @brief Met à jour un texte simple existant
 * @param text Pointeur vers le pointeur de texte à mettre à jour
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param newText Nouveau texte à afficher
 * @param color Couleur du texte
 */
void updateText(t_text** text, SDL_Renderer* renderer, char* newText, SDL_Color color);

/**
 * @brief Affiche un texte à l'écran
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param text Texte à afficher
 */
void renderText(SDL_Renderer* renderer, t_text* text);

/**
 * @brief Libère la mémoire utilisée par un élément textuel
 * @param objet Pointeur vers l'élément textuel à libérer
 */
void freeText(void* objet);

#endif