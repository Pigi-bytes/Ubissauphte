/**
 * @file slime.h
 * @brief Définition et comportement de l'ennemi slime
 */

#include "../../../debug.h"
#include "../components/particules/particules.h"
#include "../enemy.h"

/**
 * @enum e_slime_state
 * @brief États possibles du slime
 */
typedef enum {
    SLIME_IDLE,              ///< Immobile
    SLIME_CHASE_PLAYER,      ///< Poursuit le joueur activement
    SLIME_CHASE_LAST_KNOWN,  ///< Se dirige vers la dernière position connue du joueur
    SLIME_PATROL             ///< Patrouille aléatoirement
} e_slime_state;

/**
 * @struct t_slime
 * @brief Ennemi slime avec système de détection et déplacement par sauts
 */
typedef struct {
    t_enemy base;         ///< Entité de base héritée
    e_slime_state state;  ///< État actuel du slime

    t_circle detectionRange;         ///< Zone de détection du joueur
    float baseDetectionRange;        ///< Rayon de base de la zone de détection
    float jumpCooldownDuration;      ///< Temps entre deux sauts
    float idleDurationBeforePatrol;  ///< Temps d'inactivité avant de passer en mode patrouille
    float jumpForce;                 ///< Force du saut
    int patrolMovesBaseValue;        ///< Nombre de mouvements en mode patrouille

    SDL_bool playerInDetection;     ///< Indique si le joueur est dans la zone de détection
    SDL_bool playerInSight;         ///< Indique si le joueur est visible (pas d'obstacles)
    SDL_FPoint lastKnownPlayerPos;  ///< Dernière position connue du joueur

    int patrolMovesLeft;              ///< Nombre de mouvements restants en patrouille
    t_deltaTimer* idleTimer;          ///< Timer pour l'état inactif
    t_deltaTimer* jumpCooldownTimer;  ///< Timer pour le cooldown des sauts

    t_particleEmitter* particles;  ///< Émetteur de particules
    SDL_Color particleColor;       ///< Couleur des particules
} t_slime;

/**
 * @brief Met à jour l'état et le comportement du slime
 * @param enemy Pointeur vers l'ennemi à mettre à jour (cast en t_slime)
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param grid Grille de collision du niveau
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);

/**
 * @brief Crée un nouvel ennemi de type slime
 * @param texture Texture à utiliser pour le rendu
 * @param rect Rectangle définissant la position et taille initiales
 * @param tileset Tileset pour les animations
 * @param scene Scène à laquelle appartient le slime
 * @return Pointeur vers l'ennemi créé
 */
t_enemy* createSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);

/**
 * @brief Inflige des dégâts au joueur lors d'une collision
 * @param slime Slime qui inflige les dégâts
 * @param player Joueur qui reçoit les dégâts
 * @param context Contexte du jeu
 */
void slimeDealDamageToPlayer(t_slime* slime, t_joueur* player, t_context* context);

/**
 * @brief Fonction appelée lors de la mort d'un slime
 * @param context Contexte du jeu
 * @param entity Entité slime qui meurt
 */
void onSlimeDeath(t_context* context, void* entity);