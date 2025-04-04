/**
 * @file spike.h
 * @brief Définition et comportement des pièges à pointes
 */

#ifndef SPIKE_H
#define SPIKE_H

#include "../../../context.h"
#include "../tilesEntity.h"

/**
 * @struct t_spike
 * @brief Entité piège à pointes avec fonction de téléportation
 */
typedef struct {
    t_tileEntity base;        ///< Entité de base héritée
    float damage;             ///< Dégâts infligés lorsque le piège est actif
    SDL_bool isActive;        ///< Indique si le piège est actif (inflige des dégâts)
    SDL_bool messageShown;    ///< Indique si le message de téléportation a été affiché
    SDL_bool playerTouching;  ///< Indique si le joueur touche actuellement le piège
    int direction;            ///< Direction du piège (pour l'animation)
    t_salle* linkedRoom;      ///< Salle liée pour la téléportation
} t_spike;

/**
 * @brief Crée une nouvelle entité piège à pointes
 * @param tileset Tileset source pour les textures
 * @param scene Scène à laquelle appartient l'entité
 * @return Pointeur vers l'entité créée
 */
t_tileEntity* createSpikeEntity(t_tileset* tileset, t_scene* scene);

/**
 * @brief Met à jour l'état et le comportement du piège à pointes
 * @param entity Pointeur vers l'entité à mettre à jour
 * @param context Contexte du jeu
 * @param sale Salle où se trouve l'entité
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateSpike(t_tileEntity* entity, t_context* context, t_salle* sale, t_objectManager* entities);

/**
 * @brief Affiche le piège à pointes
 * @param entity Pointeur vers l'entité à afficher
 * @param context Contexte du jeu
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderSpike(t_tileEntity* entity, t_context* context, t_camera* camera);

#endif