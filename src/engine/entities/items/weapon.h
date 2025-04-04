/**
 * @file weapon.h
 * @brief Système de gestion des armes pour les entités joueur
 */

#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>

#include "../../../debug.h"
#include "../../../utils/fonctionManager.h"
#include "../entity.h"

/**
 * @struct t_arme
 * @brief Représente une arme avec ses propriétés et attributs
 */
typedef struct {
    float mass;                 ///< Masse de l'arme (affecte la vitesse d'attaque)
    float damage;               ///< Dégâts infligés par l'arme
    float range;                ///< Portée de l'arme en pixels
    float angleAttack;          ///< Angle de l'arc d'attaque en radians
    float attackDuration;       ///< Durée d'une attaque en secondes
    float attackCooldown;       ///< Temps de récupération entre deux attaques
    t_fonctionParam* onEquipe;  ///< Fonction à exécuter lors de l'équipement
    int indice;                 ///< Indice de l'arme dans l'inventaire
    SDL_Texture* texture;       ///< Texture de l'arme
    SDL_Rect displayRect;       ///< Rectangle source pour l'affichage
} t_arme;

/**
 * @struct s_joueur
 * @brief Référence au type joueur pour éviter les inclusions circulaires
 */
typedef struct s_joueur t_joueur;

/**
 * @brief Affiche l'indicateur de portée de l'arme dans l'interface
 * @param renderer Renderer SDL pour l'affichage
 * @param player Joueur dont l'arme est affichée
 */
void renderWeaponRangeUI(SDL_Renderer* renderer, t_joueur* player);

/**
 * @brief Affiche l'arme pendant une attaque
 * @param renderer Renderer SDL pour l'affichage
 * @param player Joueur qui attaque
 * @param origin Position d'origine pour le rendu
 * @param pivotPoint Point de pivot pour la rotation
 * @param scaledWidth Largeur mise à l'échelle
 * @param scaledHeight Hauteur mise à l'échelle
 * @param weaponFlip Mode de retournement de l'arme
 */
void renderWeaponDuringAttack(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip);

/**
 * @brief Affiche l'arme en position de repos
 * @param renderer Renderer SDL pour l'affichage
 * @param player Joueur dont l'arme est affichée
 * @param origin Position d'origine pour le rendu
 * @param pivotPoint Point de pivot pour la rotation
 * @param scaledWidth Largeur mise à l'échelle
 * @param scaledHeight Hauteur mise à l'échelle
 * @param weaponFlip Mode de retournement de l'arme
 */
void renderWeaponIdle(SDL_Renderer* renderer, t_joueur* player, SDL_FPoint origin, SDL_Point pivotPoint, int scaledWidth, int scaledHeight, SDL_RendererFlip weaponFlip);

/**
 * @brief Ajoute une arme à l'inventaire du joueur
 * @param player Joueur cible
 * @param weapon Arme à ajouter
 */
void addWeaponToPlayer(t_joueur* player, t_arme* weapon);

/**
 * @brief Passe à l'arme suivante dans l'inventaire du joueur
 * @param player Joueur qui change d'arme
 */
void switchToNextWeapon(t_joueur* player);

/**
 * @brief Calcule l'échelle de l'arme en fonction des attributs du joueur
 * @param player Joueur dont l'arme est affichée
 * @return Facteur d'échelle pour le rendu de l'arme
 */
float calculateWeaponScale(t_joueur* player);

/**
 * @brief Affiche un cône de débogage pour visualiser la zone d'attaque
 * @param player Joueur dont l'attaque est visualisée
 */
void renderAttackConeDebug(t_joueur* player);

#endif