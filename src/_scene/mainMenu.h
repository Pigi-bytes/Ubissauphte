/**
 * @file main_menu.h
 * @brief Gestion du menu principal du jeu
 *
 * Ce module permet de créer et gérer le menu principal du jeu
 * avec ses différentes options et fonctionnalités.

 *  @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../commun.h"
#include "mainWorld.h"

/*=== FONCTIONS ===*/

/**
 * @brief Crée et initialise le menu principal
 * @param context Contexte global du jeu
 * @param player Référence vers le joueur (peut être NULL pour nouveau jeu)
 * @return Pointeur vers la scène du menu principal créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createMainMenu(t_context* context, t_joueur** player);

/**
 * @brief Convertit une difficulté en chaîne de caractères
 * @param d Niveau de difficulté à convertir
 * @return Chaîne représentant la difficulté (doit NE PAS être libérée)
 *
 */
char* indiceFromDifficulty(difficulty d);

t_scene* chargement(t_context* context, t_joueur** player);

#endif