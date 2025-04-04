/**
 * @file animation.h
 * @brief Système d'animation pour les entités
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

#include "../../../../utils/objectManager.h"
#include "../../../../utils/timer.h"
#include "../../../tileset.h"
#include "string.h"

/**
 * @struct t_animation
 * @brief Représente une séquence d'animation avec ses paramètres
 */
typedef struct {
    char* name;            ///< Nom de l'animation
    SDL_Texture** frames;  ///< Textures de l'animation
    int numFrames;         ///< Nombre total de frames
    int currentFrame;      ///< Frame actuelle
    int animationSpeed;    ///< Vitesse en ms entre les frames
    t_timer* frameTimer;   ///< Timer pour le changement de frame
    SDL_bool isLooping;    ///< Animation en boucle
    SDL_bool isActive;     ///< État de l'animation
} t_animation;

/**
 * @struct t_animationController
 * @brief Gère plusieurs animations pour une entité
 */
typedef struct {
    t_objectManager* animations;  ///< Gestionnaire des animations disponibles
    int currentAnim;              ///< Index de l'animation actuelle
    SDL_bool haveAnimation;       ///< Indique si des animations sont disponibles
} t_animationController;

/**
 * @brief Crée une nouvelle animation
 * @param tileset Tileset source des frames
 * @param frameIndices Tableau des indices de tile à utiliser
 * @param numFrames Nombre total de frames
 * @param speed Vitesse de l'animation en ms par frame
 * @param looping Animation en boucle (SDL_TRUE) ou unique (SDL_FALSE)
 * @param name Nom identifiant l'animation
 * @return Pointeur vers l'animation créée
 */
t_animation* createAnimation(t_tileset* tileset, int* frameIndices, int numFrames, int speed, SDL_bool looping, char* name);

/**
 * @brief Démarre une animation
 * @param animation Animation à démarrer
 */
void startAnimation(t_animation* animation);

/**
 * @brief Arrête une animation
 * @param animation Animation à arrêter
 */
void stopAnimation(t_animation* animation);

/**
 * @brief Met à jour l'état d'une animation (passe à la frame suivante si nécessaire)
 * @param animation Animation à mettre à jour
 */
void updateAnimation(t_animation* animation);

/**
 * @brief Initialise un contrôleur d'animations
 * @return Pointeur vers le contrôleur créé
 */
t_animationController* initAnimationController();

/**
 * @brief Ajoute une animation au contrôleur
 * @param controller Contrôleur cible
 * @param animation Animation à ajouter
 */
void addAnimation(t_animationController* controller, t_animation* animation);

/**
 * @brief Change l'animation active
 * @param controller Contrôleur cible
 * @param name Nom de l'animation à activer
 */
void setAnimation(t_animationController* controller, char* name);

/**
 * @brief Récupère l'animation active
 * @param controller Contrôleur source
 * @return Pointeur vers l'animation active ou NULL
 */
t_animation* getCurrentAnimation(t_animationController* controller);

/**
 * @brief Met à jour l'animation active
 * @param controller Contrôleur à mettre à jour
 */
void updateController(t_animationController* controller);

#endif