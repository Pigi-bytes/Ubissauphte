/**
 * @file touche.h
 * @brief Gestion des touches affichées et interactives
 *
 * Ce module permet de créer et gérer des éléments d'interface
 * représentant des touches clavier configurables et cliquables.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef TOUCHE_H
#define TOUCHE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "../color.h"
#include "../io/input.h"
#include "../utils/fonctionManager.h"
#include "button.h"
#include "text.h"

/*=== STRUCTURES ===*/

/**
 * @struct t_touche
 * @brief Représente une touche clavier affichée et interactive
 */
typedef struct {
    t_button* button;        ///< Bouton associé à la touche
    SDL_Scancode* scancode;  ///< Code SDL de la touche clavier
    SDL_bool flagCommande;   ///< Indicateur d'état des commandes
    char nom[10];            ///< Nom affiché de la touche
} t_touche;

/*=== FONCTIONS ===*/

/**
 * @brief Affiche une touche à l'écran
 * @param ctx Contexte du jeu
 * @param touche Touche à afficher
 */
void renderTouche(t_context* ctx, t_touche* touche);

/**
 * @brief Gère les interactions avec une touche
 * @param ctx Contexte du jeu
 * @param touche Touche à gérer
 */
void handleInputTouche(t_context* ctx, t_touche* touche);

/**
 * @brief Crée une nouvelle touche interactive
 * @param text Texte à afficher
 * @param color Couleur normale
 * @param colorOnClick Couleur au clic
 * @param rect Rectangle de position/dimension
 * @param OnClick fonction lié au clic
 * @param scancode Code SDL de la touche
 * @param nom Nom affiché
 * @return Nouvelle touche configurée
 */
t_touche* createTouche(t_text* text,
                       SDL_Color color,
                       SDL_Color colorOnClick,
                       SDL_Rect rect,
                       t_fonctionParam* OnClick,
                       SDL_Scancode* scancode,
                       char* nom);

/**
 * @brief Met à jour l'affichage des commandes
 * @param fonction Paramètres de mise à jour
 */
void miseAjourCommande(t_fonctionParam* fonction);

/**
 * @brief Libère une touche de la mémoire
 * @param elt Touche à libérer (passé en void* pour compatibilité)
 */
void freeTouche(void* elt);

#endif