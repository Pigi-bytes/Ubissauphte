/**
 * @file option_menu2.h
 * @brief Gestion du menu des options avancées
 *
 * Ce module permet de créer et gérer le second menu d'options
 * contenant les paramètres avancés du jeu.
 *
 *  @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef OPTION_MENU2_H
#define OPTION_MENU2_H

#include "../commun.h"
#include "../ui/slider.h"

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise le menu des options avancées
 * @param context Contexte global du jeu contenant les configurations
 * @return Pointeur vers la scène du menu des options créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createOption2Menu(t_context* context);

#endif