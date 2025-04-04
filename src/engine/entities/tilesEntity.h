/**
 * @file tilesEntity.h
 * @brief Système d'entités basées sur les tuiles
 */

#ifndef TILES_ENTITY_H
#define TILES_ENTITY_H

#include <SDL2/SDL.h>

#include "../../utils/fscene.h"
#include "../tileset.h"
#include "components/equipment/equipementsManager.h"
#include "components/physic/physics.h"
#include "entity.h"
#include "player.h"
#include "tiles.h"

/**
 * @struct s_tileEntity
 * @brief Entité spéciale placée sur une tuile avec des comportements spécifiques
 *
 * Utilisée pour les objets interactifs, décoratifs ou les obstacles du niveau.
 */
typedef struct s_tileEntity {
    t_entity entity;  ///< Entité de base héritée

    /// Fonction de mise à jour spécifique au type d'entité
    void (*update)(struct s_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);

    /// Fonction de rendu spécifique au type d'entité
    void (*render)(struct s_tileEntity* entity, t_context* context, t_camera* camera);

    SDL_bool isDestructible;  ///< Indique si l'entité peut être détruite
} t_tileEntity;

/**
 * @brief Initialise les propriétés de base d'une entité de tuile
 * @param base Pointeur vers l'entité à initialiser
 * @param texture Texture pour le rendu
 * @param rect Rectangle de position et taille
 * @param scene Scène à laquelle appartient l'entité
 */
void initTileEntityBase(t_tileEntity* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene);

/**
 * @brief Met à jour l'état d'une entité de tuile
 * @param tileEntity Entité à mettre à jour
 * @param context Contexte du jeu
 * @param grid Grille de collision du niveau
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateTileEntity(t_tileEntity* tileEntity, t_context* context, t_salle* grid, t_objectManager* entities);

/**
 * @brief Affiche une entité de tuile
 * @param tileEntity Entité à afficher
 * @param context Contexte du jeu
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderTileEntity(t_tileEntity* tileEntity, t_context* context, t_camera* camera);

/**
 * @brief Libère la mémoire utilisée par une entité de tuile
 * @param object Pointeur vers l'entité à libérer
 */
void freeTileEntity(void* object);

/**
 * @brief Remplace une tuile par une entité de tuile
 * @param tile Tuile à remplacer
 * @param x Position X dans la grille
 * @param y Position Y dans la grille
 * @param entity Entité de remplacement
 * @param entities Gestionnaire des entités
 * @param entityTypeId Identifiant du type d'entité
 * @param floorTexture Texture du sol à utiliser après remplacement
 */
void replaceTileWithEntity(t_tile* tile, int x, int y, t_tileEntity* entity, t_objectManager* entities, uint8_t entityTypeId, SDL_Texture* floorTexture);

/**
 * @brief Traite les tuiles spéciales d'une grille
 * @param grid Grille à traiter
 * @param tileset Tileset de référence
 * @param entities Gestionnaire des entités
 * @param entityTypeId Identifiant du type d'entité
 * @param scene Scène contenant la grille
 * @param context Contexte du jeu
 */
void processSpecialTiles(t_grid* grid, t_tileset* tileset, t_objectManager* entities, uint8_t entityTypeId, t_scene* scene, t_context* context);

/**
 * @brief Wrapper pour le rendu d'entité de tuile (compatible avec fonctionManager)
 * @param f Paramètres de fonction
 */
void renderTileEntityWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la mise à jour d'entité de tuile (compatible avec fonctionManager)
 * @param f Paramètres de fonction
 */
void updateTileEntityWrapper(t_fonctionParam* f);

#endif