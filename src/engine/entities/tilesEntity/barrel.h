/**
 * @file barrel.h
 * @brief Définition et comportement du baril destructible
 */

#ifndef BARREL_H
#define BARREL_H

#include "../../../context.h"
#include "../../../utils/timer.h"
#include "../components/health/health.h"
#include "../components/particules/particules.h"
#include "../tilesEntity.h"

/**
 * @struct t_barrel
 * @brief Entité baril destructible avec récompense en or
 */
typedef struct {
    t_tileEntity base;                   ///< Entité de base héritée
    t_healthSystem health;               ///< Système de santé du baril
    SDL_bool isExploding;                ///< Indique si le baril est en train d'exploser
    t_joueur* lastDamagedBy;             ///< Dernier joueur ayant endommagé le baril
    int goldReward;                      ///< Récompense en or à la destruction
    t_particleEmitter* particleEmitter;  ///< Émetteur de particules pour les effets visuels
} t_barrel;

/**
 * @brief Crée une nouvelle entité baril
 * @param tileset Tileset source pour les textures
 * @param scene Scène à laquelle appartient l'entité
 * @return Pointeur vers l'entité créée
 */
t_tileEntity* createBarrelEntity(t_tileset* tileset, t_scene* scene);

/**
 * @brief Met à jour l'état et le comportement du baril
 * @param entity Pointeur vers l'entité à mettre à jour
 * @param context Contexte du jeu
 * @param salle Salle où se trouve l'entité
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateBarrel(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);

/**
 * @brief Affiche le baril et ses effets de particules
 * @param entity Pointeur vers l'entité à afficher
 * @param context Contexte du jeu
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderBarrel(t_tileEntity* entity, t_context* context, t_camera* camera);

/**
 * @brief Inflige des dégâts au baril
 * @param entity Pointeur vers l'entité baril
 * @param damage Quantité de dégâts à appliquer
 * @param context Contexte du jeu
 */
void takeDamageBarrel(t_tileEntity* entity, float damage, t_context* context);

/**
 * @brief Fonction appelée lors de la destruction du baril
 * @param context Contexte du jeu
 * @param entity Pointeur vers l'entité baril
 */
void onBarrelDestroy(t_context* context, void* entity);

#endif