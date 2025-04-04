/**
 * @file inventaire_menu.h
 * @brief Gestion du menu d'inventaire du joueur
 *
 * Ce module permet de créer et gérer l'interface d'inventaire
 * où le joueur peut consulter et utiliser ses objets collectés.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef INVENTAIRE_MENU_H
#define INVENTAIRE_MENU_H

#include "../commun.h"

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise le menu d'inventaire principal
 * @param context Contexte global du jeu contenant les ressources
 * @param player Pointeur vers le joueur dont l'inventaire doit être affiché
 * @return Pointeur vers la scène de l'inventaire créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene *createMainInv(t_context *context, t_joueur *player);

#endif