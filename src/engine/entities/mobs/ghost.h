/**
 * @file ghost.h
 * @brief Définition et comportement de l'ennemi fantôme
 */

#ifndef GHOST_H
#define GHOST_H

#include "../components/particules/particules.h"
#include "../enemy.h"

/**
 * @struct t_ghost
 * @brief Ennemi fantôme avec mouvement oscillatoire et capacité de boost
 */
typedef struct {
    t_enemy base;  ///< Entité de base héritée

    float moveSpeed;         ///< Vitesse de déplacement
    SDL_FPoint targetPos;    ///< Position cible pour le déplacement
    float targetChangeTime;  ///< Intervalle de changement de cible

    float verticalAmplitude;    ///< Amplitude du mouvement vertical
    float horizontalAmplitude;  ///< Amplitude du mouvement horizontal
    float verticalFrequency;    ///< Fréquence du mouvement vertical
    float horizontalFrequency;  ///< Fréquence du mouvement horizontal
    float verticalOffset;       ///< Décalage de phase vertical
    float horizontalOffset;     ///< Décalage de phase horizontal

    t_deltaTimer* timer;  ///< Timer pour les mouvements oscillatoires
    float elapsedTime;    ///< Temps écoulé pour les animations

    int transparency;     ///< Niveau de transparence (0-255)
    SDL_bool isBoosting;  ///< Indique si le fantôme est en mode boost
    float boostTimer;     ///< Temps restant en mode boost
    float boostCooldown;  ///< Temps de récupération entre deux boosts

    t_particleEmitter* particles;  // Added particle emitter
    SDL_Color particleColor;       // Added color for particles
} t_ghost;

/**
 * @brief Crée un nouvel ennemi de type fantôme
 * @param texture Texture à utiliser pour le rendu
 * @param rect Rectangle définissant la position et taille initiales
 * @param tileset Tileset pour les animations
 * @param scene Scène à laquelle appartient le fantôme
 * @return Pointeur vers l'ennemi créé
 */
t_enemy* createGhost(SDL_Texture* texture, SDL_Rect rect, t_tileset* tileset, t_scene* scene);

/**
 * @brief Met à jour l'état et le comportement du fantôme
 * @param enemy Pointeur vers l'ennemi à mettre à jour (cast en t_ghost)
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param grid Grille de collision du niveau
 * @param entities Gestionnaire des entités pour les interactions
 */
void updateGhost(t_enemy* enemy, float* deltaTime, t_grid* grid, t_objectManager* entities);

#endif