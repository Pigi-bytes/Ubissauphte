/**
 * @file context.h
 * @brief Gestion du contexte global du jeu
 *
 * Ce fichier contient les définitions des structures et fonctions
 * pour gérer l'état global du jeu (fenêtre, rendu, contrôles, options, etc.)
 *
 * @author Ubissauphte
 * @date 2025
 * @version 1.0
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <dirent.h>
#include <stdio.h>

#include "engine/core/frame.h"
#include "engine/entities/components/equipment/equipementsManager.h"
#include "engine/world/genmap.h"
#include "io/audioManager.h"
#include "io/input.h"
#include "ui/fpsDisplay.h"
#include "utils/fscene.h"

/**
 * @enum difficulty
 * @brief Définit les différents niveaux de difficulté du jeu
 */
typedef enum {
    EASY,     ///< Difficulté facile
    NORMAL,   ///< Difficulté normale
    HARD,     ///< Difficulté difficile
    DEMONIC,  ///< Difficulté démoniaque
    LEGEND,   ///< Difficulté légendaire
} difficulty;

/**
 * @struct t_control
 * @brief Contient les configurations des touches de contrôle
 */
typedef struct {
    SDL_Scancode up;          ///< Touche pour se déplacer vers le haut
    SDL_Scancode down;        ///< Touche pour se déplacer vers le bas
    SDL_Scancode left;        ///< Touche pour se déplacer vers la gauche
    SDL_Scancode right;       ///< Touche pour se déplacer vers la droite
    SDL_Scancode dash;        ///< Touche pour effectuer un dash
    SDL_Scancode escape;      ///< Touche pour mettre le jeux en pause
    SDL_Scancode map;         ///< Touche pour afficher la carte
    SDL_Scancode interact;    ///< Touche pour interagir avec l'environnement
    SDL_Scancode inventaire;  ///< Touche pour ouvrir l'inventaire
    SDL_Scancode activable1;  ///< Touche pour l'action secondaire
} t_control;

/**
 * @struct t_option
 * @brief Contient les options de configuration du jeu
 */
typedef struct {
    SDL_bool PleinEcran;    ///< Mode plein écran activé/désactivé
    SDL_bool FlagCommande;  ///< Afficher les commandes
    int fps;                ///< Nombre d'images par seconde cible
} t_option;

/**
 * @struct t_context
 * @brief Contient le contexte global du jeu
 */
typedef struct {
    SDL_Window* window;                  ///< Fenêtre principale
    SDL_Renderer* renderer;              ///< Moteur de rendu
    t_input* input;                      ///< Gestion des entrées
    t_control* control;                  ///< Configuration des contrôles
    TTF_Font* font;                      ///< Police d'écriture UI
    TTF_Font* gameFont;                  ///< Police d'écriture jeu
    t_audiomanager* audioManager;        ///< Gestionnaire audio
    t_sceneController* sceneController;  ///< Contrôleur de scènes
    t_frameData* frameData;              ///< Données de frame
    t_option* option;                    ///< Options du jeu
    t_fpsDisplay* fpsDisplay;            ///< Affichage FPS
    t_level* currentLevel;               ///< Niveau actuel
    int nbLevel;                         ///< Nombre total de niveaux
    difficulty difficulty;               ///< Difficulté actuelle
    t_item** itemListe;                  ///< Liste des objets du jeu
    int nbItem;                          ///< Nombre d'objets dans la liste
    t_tileset* playerSkin;               ///< Tileset des skin
    t_tileset* tileSet;                  ///< Tileset du jeux

} t_context;

/**
 * @brief Crée une structure d'options par défaut
 * @return Pointeur vers la structure d'options créée
 */
t_option* creeOption();

/**
 * @brief Libère la mémoire d'une structure d'options
 * @param elt Pointeur vers la structure à libérer
 */
void freeOption(void* elt);

#endif