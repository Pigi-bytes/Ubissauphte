/**
 * @file slider.h
 * @brief Gestion des curseurs glissants pour réglages de valeurs
 *
 * Ce module fournit les structures et fonctions nécessaires pour créer
 * et manipuler des curseurs glissants (sliders), principalement utilisés
 * pour les réglages de volume et autres paramètres numériques.
 */

#ifndef SLIDER_H
#define SLIDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "../io/input.h"
#include "../utils/fonctionManager.h"
#include "button.h"
#include "../io/audioManager.h"

/**
 * @struct t_barreVolumme
 * @brief Représente un curseur glissant pour régler le volume
 *
 * Structure complète qui gère l'affichage et l'interaction avec un slider
 */
typedef struct {
    SDL_Rect barre;            ///< Rectangle de la barre de fond
    float volumme;             ///< Valeur actuelle (0.0 à 1.0)
    SDL_Rect curseur;          ///< Rectangle du curseur en position actuelle
    SDL_Rect curseurDefault;   ///< Position par défaut du curseur
    SDL_Color colorBarre;      ///< Couleur de la barre de fond
    SDL_Color colorCurseur;    ///< Couleur du curseur
    SDL_bool isClicked;        ///< État de clic sur le curseur
    t_fonctionParam* OnClick;  ///< Fonction à appeler lors d'un changement de valeur
} t_barreVolumme;

/**
 * @brief Crée un nouveau curseur de volume
 * @param barre Rectangle définissant la barre
 * @param curseur Rectangle définissant le curseur
 * @param couleur Couleur de la barre
 * @param couleurCurseur Couleur du curseur
 * @param OnClick Fonction à appeler lors d'un changement
 * @return Pointeur vers le curseur créé
 */
t_barreVolumme* CreerBarreVolume(SDL_Rect barre, SDL_Rect curseur, SDL_Color couleur,
                                 SDL_Color couleurCurseur, t_fonctionParam* OnClick);

/**
 * @brief Affiche un curseur de volume
 * @param renderer Renderer SDL pour l'affichage
 * @param barre Curseur à afficher
 */
void renderBarreVolumme(SDL_Renderer* renderer, t_barreVolumme* barre);

/**
 * @brief Gère les entrées pour un curseur de volume
 * @param input Gestionnaire d'entrées
 * @param barre Curseur à manipuler
 * @param context Contexte pour gérer le volume
 */
void handleInputButtonVolumme(t_input* input, t_barreVolumme* barre, t_context* context);

/**
 * @brief Libère la mémoire utilisée par un curseur
 * @param elt Pointeur vers le curseur à libérer
 */
void freeBv(void* elt);

#endif