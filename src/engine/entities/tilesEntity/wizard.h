/**
 * @file wizard.h
 * @brief Définition et comportement du sorcier
 */

#ifndef WIZARD_H
#define WIZARD_H

#include "../../../ui/text.h"
#include "../tilesEntity.h"

/**
 * @struct t_wizard
 * @brief Entité sorcier qui vend des objets épiques
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
} t_wizard;

/**
 * @brief Met à jour l'état et le comportement du sorcier
 * @param entity Pointeur vers l'entité à mettre à jour
 * @param context Contexte du jeu
 * @param salle Salle où se trouve l'entité
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateWizard(t_tileEntity* entity, t_context* context, t_salle* salle, t_objectManager* entities);

/**
 * @brief Affiche le sorcier et son interface
 * @param entity Pointeur vers l'entité à afficher
 * @param context Contexte du jeu
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderWizard(t_tileEntity* entity, t_context* context, t_camera* camera);

/**
 * @brief Crée une nouvelle entité sorcier
 * @param tileset Tileset source pour les textures
 * @param scene Scène à laquelle appartient l'entité
 * @param context Contexte du jeu pour accéder aux ressources
 * @return Pointeur vers l'entité créée
 */
t_tileEntity* createWizardEntity(t_tileset* tileset, t_scene* scene, t_context* context);

#endif