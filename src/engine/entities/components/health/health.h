/**
 * @file health.h
 * @brief Système de gestion de la santé des entités
 */

#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include <SDL2/SDL.h>

#include "../../../../context.h"
#include "../../../../utils/timer.h"
#include "../../../core/camera.h"

/**
 * @struct s_healthSystem
 * @brief Gère la santé, les dégâts et les effets visuels associés
 */
typedef struct s_healthSystem {
    int currentHealth;  ///< Points de vie actuels
    int maxHealth;      ///< Points de vie maximum

    float displayedHealthRatio;  ///< Ratio de santé affiché (animation)
    float targetHealthRatio;     ///< Ratio de santé cible

    SDL_bool showHealthBar;        ///< Indique si la barre de vie est visible
    t_deltaTimer* healthBarTimer;  ///< Timer pour l'affichage de la barre de vie
    float healthBarDuration;       ///< Durée d'affichage de la barre de vie

    SDL_bool isFlashing;       ///< Indique si l'entité clignote (effet de dégât)
    t_deltaTimer* flashTimer;  ///< Timer pour l'effet de clignotement
    float flashDuration;       ///< Durée du clignotement

    SDL_bool isInvincible;             ///< Indique si l'entité est invincible
    t_deltaTimer* invincibilityTimer;  ///< Timer pour l'invincibilité
    float invincibilityDuration;       ///< Durée de l'invincibilité

    SDL_bool isDead;                                                                       ///< Indique si l'entité est morte
    void (*onDeathCallback)(t_context* context, void* entity);                             ///< Fonction appelée à la mort
    void (*healthBareRender)(SDL_Renderer*, struct s_healthSystem*, SDL_Rect, t_camera*);  ///< Fonction de rendu de la barre de vie
} t_healthSystem;

/**
 * @brief Initialise un système de santé
 * @param health Pointeur vers le système à initialiser
 * @param maxHealth Points de vie maximum
 */
void initHealthSystem(t_healthSystem* health, int maxHealth);

/**
 * @brief Applique des dégâts à une entité
 * @param health Système de santé cible
 * @param damage Quantité de dégâts à appliquer
 * @param entity Pointeur vers l'entité affectée
 * @param context Contexte du jeu
 */
void applyDamage(t_healthSystem* health, int damage, void* entity, t_context* context);

/**
 * @brief Met à jour le système de santé
 * @param health Système à mettre à jour
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 */
void updateHealthSystem(t_healthSystem* health, float deltaTime);

/**
 * @brief Libère les ressources du système de santé
 * @param health Système à libérer
 */
void freeHealthSystem(t_healthSystem* health);

/**
 * @brief Affiche une barre de vie standard
 * @param renderer Renderer SDL pour l'affichage
 * @param health Système de santé source
 * @param entityRect Rectangle de l'entité
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderStandardHealthBar(SDL_Renderer* renderer, t_healthSystem* health, SDL_Rect entityRect, t_camera* camera);

#endif