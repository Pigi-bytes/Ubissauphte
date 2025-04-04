/**
 * @file particules.h
 * @brief Système de particules pour les entités
 */

#ifndef ENTITY_PARTICLES_H
#define ENTITY_PARTICLES_H

#include <SDL2/SDL.h>

#include "../core.h"

/**
 * @def MAX_ENTITY_PARTICLES
 * @brief Nombre maximum de particules par émetteur
 */
#define MAX_ENTITY_PARTICLES 500  // Particules maximales par entité

/**
 * @struct s_entityParticle
 * @brief Représente une particule individuelle
 */
typedef struct s_entityParticle {
    SDL_bool active;      ///< Indique si la particule est active
    SDL_FPoint position;  ///< Position en x,y 
    SDL_FPoint velocity;  ///< Vitesse en x,y 
    float lifetime;       ///< Durée de vie restante
    float maxLifetime;    ///< Durée de vie totale
    float size;           ///< Taille de la particule
    SDL_Color color;      ///< Couleur de la particule
    int nextFree;         ///< Index de la prochaine particule libre (pour liste chaînée)
    int shape;            ///< Forme: 0 = carré, 1 = rond
} t_entityParticle;

/**
 * @struct s_particleEmitter
 * @brief Émetteur gérant un ensemble de particules
 */
typedef struct s_particleEmitter {
    t_entityParticle particles[MAX_ENTITY_PARTICLES];  ///< Tableau de particules
    int count;                                         ///< Nombre de particules actives
    int firstFree;                                     ///< Index de la première particule libre
} t_particleEmitter;

/**
 * @brief Crée un nouvel émetteur de particules
 * @return Pointeur vers l'émetteur créé
 */
t_particleEmitter* createParticleEmitter();

/**
 * @brief Réinitialise un émetteur de particules
 * @param emitter Émetteur à réinitialiser
 */
void resetParticleEmitter(t_particleEmitter* emitter);

/**
 * @brief Libère la mémoire d'un émetteur de particules
 * @param emitter Émetteur à libérer
 */
void freeParticleEmitter(t_particleEmitter* emitter);

/**
 * @brief Obtient la prochaine particule libre de l'émetteur
 * @param emitter Émetteur source
 * @return Pointeur vers la particule libre ou NULL si aucune disponible
 */
t_entityParticle* getNextFreeParticle(t_particleEmitter* emitter);

/**
 * @brief Émet des particules pour un changement de phase
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 */
void emitPhaseChangeParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);

/**
 * @brief Émet des particules pour un coup au sol
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 * @param phaseProgress Progression de la phase (0.0-1.0)
 */
void emitGroundPoundParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor, float phaseProgress);

/**
 * @brief Émet des particules pour une attaque chargée
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 * @param phaseProgress Progression de la phase (0.0-1.0)
 * @param chargeDirection Direction de la charge
 */
void emitChargeAttackParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor, float phaseProgress, SDL_FPoint chargeDirection);

/**
 * @brief Émet des particules génériques
 * @param emitter Émetteur à utiliser
 * @param x Position X centrale
 * @param y Position Y centrale
 * @param color Couleur des particules
 * @param count Nombre de particules à émettre
 * @param sizeMin Taille minimale
 * @param sizeMax Taille maximale
 * @param speedMin Vitesse minimale
 * @param speedMax Vitesse maximale
 * @param lifetimeMin Durée de vie minimale
 * @param lifetimeMax Durée de vie maximale
 */
void emitParticles(t_particleEmitter* emitter, float x, float y, SDL_Color color, int count, float sizeMin, float sizeMax, float speedMin, float speedMax, float lifetimeMin, float lifetimeMax);

/**
 * @brief Émet des particules de mouvement
 * @param emitter Émetteur à utiliser
 * @param x Position X centrale
 * @param y Position Y centrale
 * @param color Couleur des particules
 */
void emitMovementParticles(t_particleEmitter* emitter, float x, float y, SDL_Color color);

/**
 * @brief Émet des particules de mouvement pour un boss
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 */
void emitBossMovementParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);

/**
 * @brief Met à jour toutes les particules actives
 * @param emitter Émetteur contenant les particules
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 */
void updateEntityParticles(t_particleEmitter* emitter, float deltaTime);

/**
 * @brief Affiche toutes les particules actives
 * @param renderer Renderer SDL pour l'affichage
 * @param emitter Émetteur contenant les particules
 */
void renderEntityParticles(SDL_Renderer* renderer, t_particleEmitter* emitter);

/**
 * @brief Émet des particules d'atterrissage
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 */
void emitLandingParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);

/**
 * @brief Émet des particules d'impact
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param hitDirection Direction de l'impact
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 */
void emitImpactParticles(t_particleEmitter* emitter, SDL_FPoint position, SDL_FPoint hitDirection, float radius, SDL_Color baseColor);

/**
 * @brief Émet des particules de mort
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 * @param baseColor Couleur de base des particules
 */
void emitDeathParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius, SDL_Color baseColor);

/**
 * @brief Émet des particules pour un baril touché
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param hitDirection Direction de l'impact
 * @param radius Rayon d'émission
 */
void emitBarrelHitParticles(t_particleEmitter* emitter, SDL_FPoint position, SDL_FPoint hitDirection, float radius);

/**
 * @brief Émet des particules pour l'explosion d'un baril
 * @param emitter Émetteur à utiliser
 * @param position Position centrale
 * @param radius Rayon d'émission
 */
void emitBarrelExplosionParticles(t_particleEmitter* emitter, SDL_FPoint position, float radius);

/**
 * @brief Génère un nombre aléatoire dans une plage
 * @param min Valeur minimale
 * @param max Valeur maximale
 * @return Nombre flottant aléatoire entre min et max
 */
float randomRangeF(float min, float max);

#endif