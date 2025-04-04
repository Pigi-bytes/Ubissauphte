/**
 * @file enemy.h
 * @brief Définition de la structure de base pour les ennemis
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "../../context.h"
#include "../../utils/fscene.h"
#include "../../utils/timer.h"
#include "components/health/health.h"
#include "components/physic/physics.h"
#include "entity.h"
#include "tiles.h"

// Forward declaration pour éviter les dépendances circulaires
typedef struct s_joueur t_joueur;

/**
 * @struct t_enemy
 * @brief Structure de base pour tous les ennemis
 *
 * Étend l'entité de base avec des systèmes de santé, de récompense
 * et des fonctions de comportement spécifiques.
 */
typedef struct t_enemy {
    t_entity entity;        ///< Entité de base héritée
    t_healthSystem health;  ///< Système de gestion de la santé

    int xpReward;             ///< Points d'expérience donnés à la mort
    t_joueur* lastDamagedBy;  ///< Dernier joueur ayant infligé des dégâts

    /// Fonction de mise à jour spécifique au type d'ennemi
    void (*update)(struct t_enemy*, float*, t_grid*, t_objectManager*);

    /// Fonction de rendu spécifique au type d'ennemi
    void (*render)(SDL_Renderer*, struct t_enemy*, t_camera*);

    /// Fonction pour gérer les dégâts reçus
    void (*takeDamage)(struct t_enemy*, int, t_joueur*, t_context*, SDL_FPoint);
} t_enemy;

/**
 * @brief Initialise les propriétés de base d'un ennemi
 * @param base Pointeur vers l'ennemi à initialiser
 * @param texture Texture pour le rendu
 * @param rect Rectangle de position et taille
 * @param scene Scène à laquelle appartient l'ennemi
 */
void initEnemyBase(t_enemy* base, SDL_Texture* texture, SDL_Rect rect, t_scene* scene);

/**
 * @brief Affiche un ennemi à l'écran
 * @param renderer Renderer SDL pour l'affichage
 * @param enemy Ennemi à afficher
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderEnemy(SDL_Renderer* renderer, t_enemy* enemy, t_camera* camera);

/**
 * @brief Met à jour l'état d'un ennemi
 * @param enemy Ennemi à mettre à jour
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param grid Grille de collision du niveau
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateEnemy(t_enemy* enemy, float* deltaTime, t_salle* grid, t_objectManager* entities);

/**
 * @brief Libère la mémoire utilisée par un ennemi
 * @param object Pointeur vers l'ennemi à libérer
 */
void freeEnemy(void* object);

/**
 * @brief Applique des dégâts à un ennemi
 * @param enemy Ennemi cible
 * @param damage Quantité de dégâts à infliger
 * @param player Joueur qui inflige les dégâts
 * @param context Contexte du jeu
 * @param hitDirection Direction de l'impact
 */
void takeDamageFromPlayer(t_enemy* enemy, int damage, t_joueur* player, t_context* context, SDL_FPoint hitDirection);

/**
 * @brief Wrapper pour le rendu d'ennemi (compatible avec fonctionManager)
 * @param f Paramètres de fonction
 */
void renderEnemyWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la mise à jour d'ennemi (compatible avec fonctionManager)
 * @param f Paramètres de fonction
 */
void updateEnemyWrapper(t_fonctionParam* f);

#endif