/**
 * @file hud.h
 * @brief Gestion de l'interface utilisateur en jeu (HUD)
 *
 * Ce module fournit les fonctionnalités pour afficher les informations
 * importantes pendant le jeu comme la santé, l'expérience et les ressources
 * du joueur ainsi que l'état des compétences.
 */

#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../engine/entities/player.h"
#include "text.h"

/**
 * @struct t_hud
 * @brief Structure principale du HUD (Heads-Up Display)
 *
 * Gère tous les éléments visuels et informatifs affichés pendant le jeu
 */
typedef struct {
    TTF_Font* font;               ///< Police utilisée pour les textes du HUD
    SDL_Texture* player_icon;     ///< Icône représentant le joueur
    SDL_Texture* weapon_icon;     ///< Icône représentant l'arme équipée
    SDL_Texture* dash_icon;       ///< Icône représentant la capacité de dash
    t_animation* gold_animation;  ///< Animation pour l'affichage de l'or

    /**
     * @struct layout
     * @brief Disposition des éléments du HUD
     */
    struct {
        SDL_Rect player;      ///< Zone d'affichage de l'icône du joueur
        SDL_Rect health_bar;  ///< Barre de vie
        SDL_Rect xp_bar;      ///< Barre d'expérience
        SDL_Rect weapon;      ///< Zone d'affichage de l'arme
        SDL_Rect dash;        ///< Zone d'affichage du dash
        SDL_Rect gold;        ///< Zone d'affichage de l'or
    } layout;

    /**
     * @struct text
     * @brief Textes affichés dans le HUD
     */
    struct {
        t_text* health;  ///< Texte affichant la santé actuelle/maximale
        t_text* level;   ///< Texte affichant le niveau du joueur
        t_text* gold;    ///< Texte affichant la quantité d'or
    } text;

    /**
     * @struct state
     * @brief État actuel des éléments du HUD
     */
    struct {
        float health_ratio;         ///< Ratio actuel de vie (pour l'animation)
        float target_health_ratio;  ///< Ratio cible de vie
        float xp_ratio;             ///< Ratio actuel d'expérience
        float target_xp_ratio;      ///< Ratio cible d'expérience
        float weapon_cooldown;      ///< Cooldown actuel de l'arme (0-1)
        float dash_cooldown;        ///< Cooldown actuel du dash (0-1)
    } state;

    SDL_Color text_color;     ///< Couleur du texte
    SDL_Color outline_color;  ///< Couleur du contour du texte
} t_hud;

/**
 * @brief Crée une nouvelle instance du HUD
 * @param renderer Renderer SDL pour créer les textures
 * @param font Police à utiliser pour les textes
 * @param tileset Tileset contenant les graphismes
 * @return Pointeur vers le HUD créé
 */
t_hud* createHUD(SDL_Renderer* renderer, TTF_Font* font, t_tileset* tileset);

/**
 * @brief Met à jour les informations du HUD
 * @param hud HUD à mettre à jour
 * @param renderer Renderer SDL pour mettre à jour les textures
 * @param player Joueur dont les informations sont à afficher
 */
void updateHUD(t_hud* hud, SDL_Renderer* renderer, t_joueur* player);

/**
 * @brief Affiche le HUD à l'écran
 * @param renderer Renderer SDL pour l'affichage
 * @param hud HUD à afficher
 */
void renderHUD(SDL_Renderer* renderer, t_hud* hud);

/**
 * @brief Libère la mémoire utilisée par le HUD
 * @param hud Pointeur vers le HUD à libérer
 */
void freeHUD(void* hud);

/**
 * @brief Wrapper pour mettre à jour le HUD (compatible avec fonctionManager)
 * @param f Paramètres de la fonction
 */
void updateHUDWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour afficher le HUD (compatible avec fonctionManager)
 * @param f Paramètres de la fonction
 */
void renderHUDWrapper(t_fonctionParam* f);

#endif