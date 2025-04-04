/**
 * @file commande_menu.h
 * @brief Gestion du menu des commandes
 *
 * Ce module permet de créer et gérer le menu d'affichage
 * des commandes/contrôles du jeu.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef COMMANDE_MENU_H
#define COMMANDE_MENU_H

#include "../commun.h"  // Inclusion des déclarations communes

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise le menu des commandes
 * @param context Contexte global du jeu contenant les configurations
 * @return Pointeur vers la scène du menu des commandes créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createCommandeMenu(t_context* context);

#endif