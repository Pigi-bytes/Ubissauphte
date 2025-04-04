/**
 * @file fpsDisplay.h
 * @brief Gestion de l'affichage des FPS (images par seconde)
 *
 * Ce module permet d'afficher et de mettre à jour un compteur de FPS
 * pour visualiser les performances d'exécution du jeu.
 */

#ifndef FPS_DISPLAY_H
#define FPS_DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../engine/core/frame.h"
#include "text.h"

/**
 * @struct fpsDisplay
 * @brief Structure gérant l'affichage des FPS
 */
typedef struct fpsDisplay {
    t_text* fpsText;        ///< Texte affiché
    SDL_bool showFPS;       ///< Flag pour afficher ou non
    Uint32 lastUpdateTime;  ///< Dernière mise à jour de l'affichage
} t_fpsDisplay;

/**
 * @brief Initialise l'affichage des FPS
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param font Police à utiliser pour l'affichage
 * @return Pointeur vers la structure d'affichage des FPS
 */
t_fpsDisplay* initFPSDisplay(SDL_Renderer* renderer, TTF_Font* font);

/**
 * @brief Met à jour l'affichage des FPS
 * @param display Structure d'affichage des FPS
 * @param frame Données de frame contenant les informations de timing
 * @param renderer Renderer SDL utilisé pour le rendu
 */
void updateFPSDisplay(t_fpsDisplay* display, t_frameData* frame, SDL_Renderer* renderer);

/**
 * @brief Affiche le compteur de FPS à l'écran
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param display Structure d'affichage des FPS
 */
void renderFPSDisplay(SDL_Renderer* renderer, t_fpsDisplay* display);

/**
 * @brief Libère la mémoire utilisée par l'affichage des FPS
 * @param object Pointeur vers la structure à libérer
 */
void freeFPSDisplay(void* object);

#endif