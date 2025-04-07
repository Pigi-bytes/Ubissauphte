#ifndef WIZARD_H
#define WIZARD_H

#include <math.h>

#include "../components/particules/particules.h"
#include "../enemy.h"

/**
 * @struct t_wizard
 * @brief Ennemi de type sorcier, capable de soigner les autres ennemis dans un rayon.
 */
typedef struct {
    t_enemy base;  ///< Structure de base de l'ennemi

    float healRadius;         ///< Rayon dans lequel le sorcier peut soigner ses alliés
    int healAmount;           ///< Points de vie soignés
    float healCooldown;       ///< Intervalle (secondes) entre deux soins
    t_deltaTimer* healTimer;  ///< Timer pour gérer le cooldown de soin

    t_particleEmitter* particles;  ///< Système de particules
    SDL_Color particleColor;       ///< Couleur des particules du sorcier

    // États de comportement
    float detectionRadius;       ///< Rayon de détection du joueur
    float fleeRadius;            ///< Rayon dans lequel le sorcier fuit le joueur
    t_circle detectionRange;     ///< Cercle de détection
    SDL_bool playerInDetection;  ///< Joueur dans le rayon de détection
    SDL_bool playerInSight;      ///< Joueur visible (pas d'obstacles)

    SDL_FPoint targetHealPos;  ///< Position cible pour aller soigner un allié
    SDL_bool hasHealTarget;    ///< Si le sorcier a une cible de soin
    float moveSpeed;           ///< Vitesse de déplacement du sorcier

    // Système de patrouille pour chercher des alliés à soigner
    SDL_FPoint patrolTarget;    ///< Position cible pour la patrouille
    t_deltaTimer* patrolTimer;  ///< Timer pour gérer la durée de patrouille
    SDL_bool isPatrolling;      ///< Si le sorcier est en patrouille
} t_wizard;

/**
 * @brief Crée et initialise un sorcier.
 * @param texture Texture du sorcier
 * @param rect Rectangle définissant position et taille
 * @param tileset Tileset pour les animations
 * @param scene Scène à laquelle appartient le sorcier
 */
t_enemy* createWizard(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);
void wizardTakeDamage(t_enemy* enemy, int damage, t_joueur* player, t_context* context, SDL_FPoint hitDirection);
#endif