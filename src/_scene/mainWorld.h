/**
 * @file main_world.h
 * @brief Gestion du monde principal du jeu
 *
 * Ce module permet de créer et gérer les différentes cartes et niveaux
 * du jeu, y compris les salles normales, les salles de marchands et
 * les salles de boss.
 *
 *  @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef MAIN_WORLD_H
#define MAIN_WORLD_H

#include "../commun.h"
#include "../engine/entities/mobs/boss_slime.h"
#include "../engine/entities/mobs/ghost.h"
#include "../engine/entities/mobs/wizard.h"
#include "../engine/entities/tilesEntity.h"
#include "../ui/hud.h"

/*=== FONCTIONS DE CREATION DE CARTES ===*/

/**
 * @brief Crée le monde principal du jeu
 * @param context Contexte global du jeu
 * @param salle Configuration de la salle à générer
 * @param player Référence vers le joueur
 * @param level Grille du niveau (peut être NULL pour génération procédurale)
 * @return Pointeur vers la scène créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createMainWord(t_context* context, t_salle* salle, t_joueur** player, t_grid** level);

/**
 * @brief Crée une carte de marchand
 * @param context Contexte global du jeu
 * @param salle Configuration de la salle de marchand
 * @param player Référence vers le joueur
 * @param level Grille du niveau
 * @return Pointeur vers la scène créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createMarchantMap(t_context* context, t_salle* salle, t_joueur** player, t_grid** level);

/**
 * @brief Crée une carte de boss
 * @param context Contexte global du jeu
 * @param salle Configuration de la salle de boss
 * @param player Référence vers le joueur
 * @param level Grille du niveau
 * @return Pointeur vers la scène créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createBossMap(t_context* context, t_salle* salle, t_joueur** player, t_grid** level);

/*=== FONCTIONS DE GESTION DE NIVEAU ===*/

/**
 * @brief Crée une carte à partir d'une configuration
 * @param context Contexte global du jeu
 * @param salle Tableau de salles disponibles
 * @param rectcord Rectangle définissant la zone à générer
 * @param player Joueur actuel
 * @param nbSalle Nombre de salles disponibles
 * @return Pointeur vers la scène créée
 *
 * @note La scène retournée doit être libérée avec freeScene()
 * quand elle n'est plus utilisée sauf si mis dans uns scèneControler.
 */
t_scene* createMapWord(t_context* context, t_salle** salle, SDL_Rect* rectcord, t_joueur* player, int nbSalle);

/**
 * @brief Crée un niveau complet
 * @param context Contexte global du jeu
 * @param nbSalle Pointeur vers le nombre de salles (sera mis à jour)
 * @param joueur Référence vers le joueur
 *
 * @note Cette fonction initialise toutes les structures nécessaires
 * pour un nouveau niveau
 */
void CreateNiveau(t_context* context, int* nbSalle, t_joueur** joueur);

/**
 * @brief Wrapper pour la création de niveau (utilisation avec fonctionManager)
 * @param f Paramètres encapsulés pour la création de niveau
 *
 * @note Version compatible avec le système de callback du fonctionManager
 */
void CreateNiveauWrapper(t_fonctionParam* f);

#endif