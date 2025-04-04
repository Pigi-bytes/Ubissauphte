/**
 * @file blacksmith.h
 * @brief Définition et comportement du forgeron
 */

#ifndef BLACKSMITH_H
#define BLACKSMITH_H

#include "../../../ui/text.h"
#include "../tilesEntity.h"

/**
 * @struct t_blacksmith
 * @brief Entité forgeron qui vend des objets rares
 */
typedef struct {
    t_tileEntity base;            ///< Entité de base héritée
    t_circle detectionRange;      ///< Zone de détection du joueur
    SDL_bool playerInRange;       ///< Indique si le joueur est dans la zone d'interaction
    SDL_bool interactKeyPressed;  ///< État de la touche d'interaction
    SDL_Keycode lastInteractKey;  ///< Dernière touche d'interaction utilisée
    t_item* randomItem;           ///< Objet proposé à la vente
    t_text *interactText, *item;  ///< Textes d'interface utilisateur
    int prix;                     ///< Prix de l'objet en or
} t_blacksmith;

/**
 * @brief Met à jour l'état et le comportement du forgeron
 * @param entity Pointeur vers l'entité à mettre à jour
 * @param context Contexte du jeu
 * @param salle Salle où se trouve l'entité
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateBlacksmith(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);

/**
 * @brief Affiche le forgeron et son interface
 * @param entity Pointeur vers l'entité à afficher
 * @param context Contexte du jeu
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderBlacksmith(t_tileEntity* entity, t_context* context, t_camera* camera);

/**
 * @brief Crée une nouvelle entité forgeron
 * @param tileset Tileset source pour les textures
 * @param scene Scène à laquelle appartient l'entité
 * @param context Contexte du jeu pour accéder aux ressources
 * @return Pointeur vers l'entité créée
 */
t_tileEntity* createBlacksmithEntity(t_tileset* tileset, t_scene* scene, t_context* context);

#endif