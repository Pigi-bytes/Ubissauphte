/**
 * @file chest.h
 * @brief Définition et comportement du coffre à trésor
 */

#ifndef CHEST_H
#define CHEST_H

#include "../../../ui/text.h"
#include "../tilesEntity.h"

/**
 * @struct t_chest
 * @brief Entité coffre qui contient des objets ou de l'or
 */
typedef struct {
    t_tileEntity base;            ///< Entité de base héritée
    t_circle detectionRange;      ///< Zone de détection du joueur
    SDL_bool playerInRange;       ///< Indique si le joueur est dans la zone d'interaction
    SDL_bool interactKeyPressed;  ///< État de la touche d'interaction
    SDL_Keycode lastInteractKey;  ///< Dernière touche d'interaction utilisée
    SDL_bool isOpen;              ///< Indique si le coffre a déjà été ouvert
    t_item* containedItem;        ///< Objet contenu dans le coffre
    t_text* interactText;         ///< Texte d'interface utilisateur
    int goldReward;               ///< Quantité d'or contenue dans le coffre
} t_chest;

/**
 * @brief Met à jour l'état et le comportement du coffre
 * @param entity Pointeur vers l'entité à mettre à jour
 * @param context Contexte du jeu
 * @param salle Salle où se trouve l'entité
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateChest(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);

/**
 * @brief Affiche le coffre et son interface
 * @param entity Pointeur vers l'entité à afficher
 * @param context Contexte du jeu
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderChest(t_tileEntity* entity, t_context* context, t_camera* camera);

/**
 * @brief Crée une nouvelle entité coffre
 * @param tileset Tileset source pour les textures
 * @param scene Scène à laquelle appartient l'entité
 * @param context Contexte du jeu pour accéder aux ressources
 * @return Pointeur vers l'entité créée
 */
t_tileEntity* createChestEntity(t_tileset* tileset, t_scene* scene, t_context* context);

#endif