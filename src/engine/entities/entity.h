/**
 * @file entity.h
 * @brief Définition de la structure de base pour toutes les entités du jeu
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

#include "../../debug.h"
#include "../core/camera.h"
#include "components/animation/animation.h"
#include "components/core.h"
#include "components/physic/physics.h"

/**
 * @struct s_entity
 * @brief Structure de base pour toutes les entités du jeu
 *
 * Contient les propriétés communes à toutes les entités: position, collision,
 * physique, rendu et animations.
 */
typedef struct s_entity {
    SDL_Rect displayRect;  ///< Position et taille pour l'affichage

    SDL_Rect collisionRect;       ///< Position et taille pour les collisions rectangulaires
    t_circle collisionCircle;     ///< Position et taille pour les collisions circulaires
    SDL_bool useCircleCollision;  ///< Indicateur pour utiliser le cercle de collision

    t_physics physics;  ///< Composant physique de l'entité

    SDL_Texture* texture;                        ///< Texture principale de l'entité
    t_animationController* animationController;  ///< Contrôleur d'animations
    SDL_RendererFlip flip;                       ///< Direction de rendu (retournement horizontal/vertical)
    SDL_bool debug;                              ///< Active/désactive les affichages de débogage

    struct t_scene* currentScene;  ///< Référence à la scène contenant l'entité
} t_entity;

/**
 * @brief Affiche une entité à l'écran
 * @param renderer Renderer SDL pour l'affichage
 * @param entity Entité à afficher
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderEntity(SDL_Renderer* renderer, t_entity* entity, t_camera* camera);

#endif