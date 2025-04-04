/**
 * @file physics.h
 * @brief Système de physique pour les entités
 */

#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL2/SDL.h>

#include "../core.h"

// Forward declarations using 'struct' to avoid name conflicts
struct s_entity;
struct s_grid;
struct s_objectManager;

/**
 * @struct t_physics
 * @brief Composant de physique pour les entités
 */
typedef struct {
    SDL_FPoint velocity;  ///< Vélocité actuelle (x,y)
    float mass;           ///< Masse de l'entité (0 = infinité/statique)
    float friction;       ///< Frottement (pour ralentir l'entité)
    float restitution;    ///< Elasticité des collisions (0.0 = pas élastique, 1.0 = parfaitement élastique)
} t_physics;

/**
 * @brief Détecte collision entre un cercle et un rectangle
 * @param circle Cercle à tester
 * @param rect Rectangle à tester
 * @param out Données de collision (point d'impact, etc.)
 * @return SDL_TRUE si collision, SDL_FALSE sinon
 */
SDL_bool checkCircleRectCollision(t_circle* circle, SDL_Rect* rect, t_collisionData* out);

/**
 * @brief Détecte collision entre deux cercles
 * @param a Premier cercle
 * @param b Deuxième cercle
 * @param out Données de collision (point d'impact, etc.)
 * @return SDL_TRUE si collision, SDL_FALSE sinon
 */
SDL_bool checkCircleCircleCollision(t_circle* a, t_circle* b, t_collisionData* out);

/**
 * @brief Vérifie si un cercle est dans un secteur angulaire
 * @param target Centre du cercle cible
 * @param target_radius Rayon du cercle cible
 * @param origin Origine du secteur
 * @param currentAngle Angle central du secteur (radians)
 * @param range Distance maximale du secteur
 * @param arc Ouverture angulaire du secteur (radians)
 * @return SDL_TRUE si le cercle est dans le secteur, SDL_FALSE sinon
 */
SDL_bool cercleInSector(SDL_FPoint target, float target_radius, SDL_FPoint origin, float currentAngle, float range, float arc);

/**
 * @brief Met à jour la physique d'une entité
 * @param entity Entité à mettre à jour
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param grid Grille pour les collisions avec le terrain
 * @param entities Gestionnaire d'entités pour les collisions entre entités
 */
void updatePhysicEntity(struct s_entity* entity, float* deltaTime, struct s_grid* grid, struct s_objectManager* entities);

/**
 * @brief Résout les collisions d'une entité
 * @param entity Entité à traiter
 * @param grid Grille pour les collisions
 * @param entities Gestionnaire d'entités pour les collisions entre entités
 */
void resolveCollision(struct s_entity* entity, struct s_grid* grid, struct s_objectManager* entities);

/**
 * @brief Lance un rayon à travers la grille et détecte la première obstruction
 * @param grid Grille à tester
 * @param start Point de départ du rayon
 * @param end Point d'arrivée du rayon
 * @param tileSize Taille des tuiles de la grille
 * @param obstructionPoint Point d'obstruction (résultat)
 * @return SDL_TRUE si obstruction trouvée, SDL_FALSE sinon
 */
SDL_bool gridRaycast(struct s_grid* grid, SDL_FPoint start, SDL_FPoint end, int tileSize, SDL_FPoint* obstructionPoint);

#endif