/**
 * @file button.h
 * @brief Système de gestion des boutons pour l'interface utilisateur
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @def SCALE_FACTOR
 * @brief Facteur d'agrandissement lors du survol d'un bouton
 */
#define SCALE_FACTOR 1.1f;

#include "../context.h"
#include "../io/input.h"
#include "../utils/fonctionManager.h"
#include "text.h"

/**
 * @struct t_button
 * @brief Représente un bouton interactif de l'interface utilisateur
 */
typedef struct {
    SDL_Rect rect;             ///< Rectangle actuel du bouton (peut être modifié lors d'animations)
    SDL_Rect rectDefault;      ///< Rectangle par défaut du bouton (taille et position initiales)
    SDL_Color color;           ///< Couleur actuelle du bouton
    SDL_Color colorOnClick;    ///< Couleur du bouton lors d'un clic
    SDL_Color colorDefault;    ///< Couleur par défaut du bouton
    t_text* label;             ///< Texte affiché sur le bouton
    t_fonctionParam* OnClick;  ///< Fonction à exécuter lors du clic
    SDL_bool isClicked;        ///< Indique si le bouton est actuellement cliqué
    SDL_bool isHovered;        ///< Indique si le curseur survole le bouton
} t_button;

/**
 * @brief Crée un nouveau bouton
 * @param text Texte à afficher sur le bouton
 * @param color Couleur par défaut du bouton
 * @param colorOnClick Couleur du bouton lors d'un clic
 * @param rect Rectangle définissant la position et taille du bouton
 * @param OnClick Fonction à exécuter lors du clic
 * @return Pointeur vers le bouton créé
 */
t_button* createButton(t_text* text, SDL_Color color, SDL_Color colorOnClick, SDL_Rect rect, t_fonctionParam* OnClick);

/**
 * @brief Affiche un bouton à l'écran
 * @param ctx Contexte du jeu
 * @param button Bouton à afficher
 */
void renderButton(t_context* ctx, t_button* button);

/**
 * @brief Traite les entrées utilisateur pour un bouton
 * @param ctx Contexte du jeu
 * @param button Bouton à contrôler
 */
void handleInputButton(t_context* ctx, t_button* button);

/**
 * @brief Vérifie si le curseur se trouve à l'intérieur d'un rectangle
 * @param mouseX Position X du curseur
 * @param mouseY Position Y du curseur
 * @param rect Rectangle à vérifier
 * @return SDL_TRUE si le curseur est dans le rectangle, SDL_FALSE sinon
 */
SDL_bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);

/**
 * @brief Libère la mémoire utilisée par un bouton
 * @param object Pointeur vers le bouton à libérer
 */
void freeButton(void* object);

#endif  // BUTTON_H