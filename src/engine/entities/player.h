/**
 * @file player.h
 * @brief Définition et comportement du joueur principal
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "../../context.h"
#include "../../debug.h"
#include "../../io/input.h"
#include "components/equipment/equipementsManager.h"
#include "components/health/health.h"
#include "components/particules/particules.h"
#include "components/physic/physics.h"
#include "entity.h"
#include "items/weapon.h"
#include "tiles.h"

/**
 * @def DASH_TRAIL_COUNT
 * @brief Nombre maximal de positions pour l'effet de traînée pendant un dash
 */
#define DASH_TRAIL_COUNT 50

/**
 * @struct t_attack
 * @brief Gère l'état et les propriétés d'une attaque du joueur
 *
 * Phases d'une attaque : Anticipation > Contact > Récupération
 * @see https://www.youtube.com/watch?v=8X4fx-YncqA
 */
typedef struct {
    SDL_bool isActive;   ///< Indique si une attaque est en cours
    float cooldown;      ///< Temps de récupération entre deux attaques
    float progress;      ///< Progression de l'attaque (0.0 à 1.0)
    t_sector hitBox;     ///< Secteur d'attaque (origine, angles, portée)
    int nbHits;          ///< Nombre d'entités touchées par l'attaque en cours
    float hit_distance;  ///< Distance du dernier hit pour l'effet de tremblement d'écran

    float timeSlowFactor;     ///< Facteur de ralentissement du temps lors d'un hit
    float timeSlowDuration;   ///< Durée du ralentissement du temps
    float timeSlowRemaining;  ///< Temps restant de ralentissement
} t_attack;

/**
 * @struct t_dash
 * @brief Gère l'état et les propriétés d'un dash (esquive rapide)
 */
typedef struct {
    SDL_bool isActive;       ///< Indique si le dash est actif
    float duration;          ///< Durée du dash en secondes
    float speedMultiplier;   ///< Multiplicateur de vitesse pendant le dash
    t_timer* cooldownTimer;  ///< Timer pour gérer le temps de récupération
    Uint32 cooldownTime;     ///< Temps de récupération en millisecondes

    /**
     * @struct trail
     * @brief Définit une position de traînée pour l'effet visuel du dash
     */
    struct {
        SDL_Point position;      ///< Position d'une image résiduelle
        SDL_bool active;         ///< Indique si cette image est active
        float opacity;           ///< Opacité de l'image (0.0 à 1.0)
    } trails[DASH_TRAIL_COUNT];  ///< Tableau des positions de traînée

    float trailSpawnInterval;  ///< Intervalle de temps entre les captures de position
    float timeSinceLastTrail;  ///< Temps écoulé depuis la dernière capture
} t_dash;

/**
 * @struct s_joueur
 * @brief Représente le joueur contrôlé par l'utilisateur
 *
 * Contient toutes les propriétés du joueur: entité de base, contrôles,
 * capacités spéciales, santé, inventaire, statistiques et équipement.
 */
typedef struct s_joueur {
    t_entity entity;                     ///< Entité de base héritée
    t_control* control;                  ///< Configuration des contrôles
    t_dash dash;                         ///< Système de dash (esquive)
    t_healthSystem health;               ///< Système de gestion de la santé
    t_particleEmitter* particleEmitter;  ///< Émetteur de particules

    t_stats baseStats;        ///< Statistiques de base
    t_stats calculatedStats;  ///< Statistiques calculées avec équipement

    t_equipementSlotType equipement[TOTAL_EQUIPMENT_SLOTS];  ///< Équipement actuel
    t_inventaire* inventaire;                                ///< Inventaire du joueur

    int level;          ///< Niveau actuel du joueur
    int xp;             ///< Points d'expérience actuels
    int xpToNextLevel;  ///< XP requis pour le prochain niveau
    int gold;           ///< Or possédé

    t_arme* currentWeapon;  ///< Arme actuellement équipée
    int indexCurrentRoom;   ///< Index de la salle actuelle

    t_attack attack;  ///< État de l'attaque en cours
    float aimAngle;   ///< Angle de visée en radians
} t_joueur;

/**
 * @brief Crée un nouveau joueur
 * @param control Configuration des contrôles
 * @param texture Texture pour le rendu
 * @param rect Rectangle définissant la position et taille initiales
 * @param tileset Tileset pour les animations
 * @return Pointeur vers le joueur créé
 */
t_joueur* createPlayer(t_control* control, SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset);

/**
 * @brief Met à jour l'état et le comportement du joueur
 * @param player Joueur à mettre à jour
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param salle Salle où se trouve le joueur
 * @param entities Gestionnaire des entités pour les interactions
 * @param context Contexte du jeu
 */
void updatePlayer(t_joueur* player, float* deltaTime, t_salle* salle, t_objectManager* entities, t_context* context);

/**
 * @brief Affiche le joueur à l'écran
 * @param renderer Renderer SDL pour l'affichage
 * @param player Joueur à afficher
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderPlayer(SDL_Renderer* renderer, t_joueur* player, t_camera* camera);

/**
 * @brief Traite les entrées utilisateur pour le joueur
 * @param input État des entrées
 * @param player Joueur à contrôler
 * @param grid Grille de collision du niveau
 * @param vp Viewport actif
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param sceneController Contrôleur de scène pour les transitions
 */
void handleInputPlayer(t_input* input, t_joueur* player, t_grid* grid, t_viewPort* vp, float* deltaTime, t_sceneController* sceneController);

/**
 * @brief Libère la mémoire utilisée par un joueur
 * @param object Pointeur vers le joueur à libérer
 */
void freePlayer(void* object);

/**
 * @brief Ajoute des points d'expérience au joueur
 * @param player Joueur qui reçoit l'XP
 * @param xpAmount Quantité d'XP à ajouter
 */
void addPlayerXP(t_joueur* player, int xpAmount);

/**
 * @brief Calcule la progression vers le niveau suivant
 * @param player Joueur dont on veut connaître la progression
 * @return Ratio (0.0-1.0) de progression vers le niveau suivant
 */
float getPlayerXPProgress(t_joueur* player);

/**
 * @brief Vérifie si le joueur peut monter de niveau et applique les améliorations
 * @param player Joueur à vérifier
 * @return SDL_TRUE si le joueur a monté de niveau, SDL_FALSE sinon
 */
SDL_bool checkAndProcessLevelUp(t_joueur* player);

#endif