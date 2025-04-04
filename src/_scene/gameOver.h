/**
 * @file game_over.h
 * @brief Gestion de l'écran de fin de partie
 *
 * Ce module permet de créer et gérer l'écran de Game Over
 * affiché lorsque le joueur perd la partie.
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "../commun.h"

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise l'écran de Game Over
 * @param context Contexte global du jeu contenant les données nécessaires
 * @return Pointeur vers la scène Game Over créée
 *
 *  @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 *
 */
t_scene* CreateGameOver(t_context* context);

#endif