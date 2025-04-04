/**
 * @file option_menu.h
 * @brief Gestion du menu des options du jeu
 *
 * Ce module permet de créer et gérer le menu des options
 * contenant les paramètres configurables du jeu (graphismes, sons, contrôles).
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef OPTION_MENU_H
#define OPTION_MENU_H

#include "../commun.h"
#include "../ui/slider.h"

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise le menu des options principales
 * @param context Contexte global du jeu contenant la configuration actuelle
 * @return Pointeur vers la scène du menu des options créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler..

 */
t_scene* createOptionMenu(t_context* context);

#endif