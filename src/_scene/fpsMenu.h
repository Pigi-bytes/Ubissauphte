/**
 * @file fps_menu.h
 * @brief Gestion du menu des FPS (Frames Per Second)
 *
 * Ce module permet de créer et gérer le menu des FPS
 * et de configuration du taux de rafraîchissement du jeu.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef FPS_MENU_H
#define FPS_MENU_H

#include "../commun.h"

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise le menu des FPS
 * @param context Contexte global du jeu contenant les paramètres d'affichage
 * @return Pointeur vers la scène du menu FPS créée
 *
 *
 *  @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createFpsMenu(t_context* context);

#endif