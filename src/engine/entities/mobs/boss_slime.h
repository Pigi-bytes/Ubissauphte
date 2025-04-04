/**
 * @file boss_slime.h
 * @brief Définition et comportement du boss slime
 */

#ifndef BOSS_SLIME_H
#define BOSS_SLIME_H

#include "../../../_scene/mainWorld.h"
#include "../../../debug.h"
#include "../../../ui/text.h"
#include "../enemy.h"

/**
 * @enum e_boss_slime_state
 * @brief États possibles du boss slime
 */
typedef enum {
    BOSS_SLIME_IDLE,              ///< Immobile
    BOSS_SLIME_CHASE_PLAYER,      ///< Poursuit le joueur activement
    BOSS_SLIME_CHASE_LAST_KNOWN,  ///< Se dirige vers la dernière position connue du joueur
    BOSS_SLIME_ENRAGED,           ///< Phase 2 - plus rapide et agressif
    BOSS_SLIME_DESPERATE,         ///< Phase 3 - attaques désespérées
    BOSS_SLIME_GROUND_POUND,      ///< Attaque spéciale 1 - saut puis impact
    BOSS_SLIME_CHARGE_ATTACK      ///< Attaque spéciale 2 - charge rapide
} e_boss_slime_state;

/**
 * @struct t_boss_slime
 * @brief Boss slime avec plusieurs phases et attaques spéciales
 */
typedef struct {
    t_enemy base;              ///< Entité de base héritée
    e_boss_slime_state state;  ///< État actuel du boss

    t_circle detectionRange;   ///< Zone de détection du joueur
    float baseDetectionRange;  ///< Rayon de base de la zone de détection

    float jumpCooldownDuration;    ///< Temps entre deux sauts
    float idleDurationBeforeMove;  ///< Temps d'inactivité avant mouvement
    float jumpForce;               ///< Force du saut
    int movesBaseValue;            ///< Nombre de mouvements de base

    float groundPoundForce;       ///< Force de l'attaque "Ground Pound"
    float chargeForce;            ///< Force de l'attaque "Charge"
    float specialAttackCooldown;  ///< Temps entre deux attaques spéciales

    SDL_bool playerInDetection;     ///< Indique si le joueur est dans la zone de détection
    SDL_bool playerInSight;         ///< Indique si le joueur est visible
    SDL_FPoint lastKnownPlayerPos;  ///< Dernière position connue du joueur

    int movesLeft;                       ///< Nombre de mouvements restants
    int currentPhase;                    ///< Phase actuelle du combat
    float phase2HealthThreshold;         ///< Seuil de santé pour passer à la phase 2
    float phase3HealthThreshold;         ///< Seuil de santé pour passer à la phase 3
    float invulnerabilityPhaseDuration;  ///< Durée d'invulnérabilité lors du changement de phase
    SDL_bool isPhaseTransition;          ///< Indique si le boss est en transition de phase

    t_deltaTimer* idleTimer;             ///< Timer pour l'état inactif
    t_deltaTimer* jumpCooldownTimer;     ///< Timer pour le cooldown des sauts
    t_deltaTimer* specialAttackTimer;    ///< Timer pour le cooldown des attaques spéciales
    t_deltaTimer* phaseTransitionTimer;  ///< Timer pour les transitions de phase

    t_particleEmitter* particles;  ///< Émetteur de particules
    SDL_Color particleColor;       ///< Couleur des particules
} t_boss_slime;

/**
 * @struct t_bossHealthBar
 * @brief Barre de vie du boss avec affichage spécial
 */
typedef struct {
    SDL_bool isActive;                   ///< Indique si la barre est active
    TTF_Font* font;                      ///< Police pour le texte
    t_enemy* boss;                       ///< Référence au boss
    char* bossName;                      ///< Nom du boss
    float displayedHealthRatio;          ///< Ratio de santé affiché (animation)
    float targetHealthRatio;             ///< Ratio de santé cible
    SDL_Color phaseColors[3];            ///< Couleurs pour chaque phase
    int currentPhase;                    ///< Phase actuelle
    t_deltaTimer* phaseTransitionTimer;  ///< Timer pour les transitions
    float lastDamageTime;                ///< Moment du dernier dégât reçu
    t_text* nameText;                    ///< Texte pour le nom du boss
} t_bossHealthBar;

/**
 * @brief Met à jour l'état et le comportement du boss slime
 * @param enemy Pointeur vers l'ennemi à mettre à jour (cast en t_boss_slime)
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param grid Grille de collision du niveau
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateBossSlime(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);

/**
 * @brief Crée un nouveau boss slime
 * @param texture Texture à utiliser pour le rendu
 * @param rect Rectangle définissant la position et taille initiales
 * @param tileset Tileset pour les animations
 * @param scene Scène à laquelle appartient le boss
 * @return Pointeur vers l'ennemi créé
 */
t_enemy* createBossSlime(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);

/**
 * @brief Crée une barre de vie pour le boss
 * @param font Police pour le texte
 * @param boss Boss associé à la barre de vie
 * @return Pointeur vers la barre de vie
 */
t_bossHealthBar* createBossHealthBar(TTF_Font* font, t_enemy* boss);

/**
 * @brief Met à jour la barre de vie du boss
 * @param bar Barre de vie à mettre à jour
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 */
void updateBossHealthBar(t_bossHealthBar* bar, float deltaTime);

/**
 * @brief Affiche la barre de vie du boss
 * @param renderer Renderer SDL
 * @param bar Barre de vie à afficher
 * @param windowWidth Largeur de la fenêtre
 * @param windowHeight Hauteur de la fenêtre
 */
void renderBossHealthBar(SDL_Renderer* renderer, t_bossHealthBar* bar, int windowWidth, int windowHeight);

/**
 * @brief Libère les ressources associées à la barre de vie
 * @param obj Pointeur vers la barre de vie à libérer
 */
void freeBossHealthBar(void* obj);

/**
 * @brief Wrapper pour la mise à jour de la barre de vie (compatible avec fonctionManager)
 * @param f Paramètres de fonction
 */
void updateBossHealthBarWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu de la barre de vie (compatible avec fonctionManager)
 * @param f Paramètres de fonction
 */
void renderBossHealthBarWrapper(t_fonctionParam* f);

#endif