/**
 * @file frame.h
 * @brief Gestion des frames et du calcul FPS
 */

#ifndef FPS_H
#define FPS_H

#include "../../debug.h"
#include "../../utils/timer.h"

/**
 * @def TARGET_FPS
 * @brief Nombre d'images par seconde cible par défaut
 */
#define TARGET_FPS 60

/**
 * @def SECONDE_EN_MS
 * @brief Constante de conversion de secondes en millisecondes
 */
#define SECONDE_EN_MS 1000

/**
 * @struct t_frameData
 * @brief Structure de gestion des frames et du calcul de FPS
 *
 * Permet de contrôler la fréquence d'affichage, mesurer les performances
 * et calculer le temps delta entre les frames pour des animations fluides
 */
typedef struct {
    t_timer* frameTimer;       ///< Timer entre chaque frame
    t_timer* fpsTimer;         ///< Timer pour le calcul de fps
    Uint32 frameCount;         ///< Compteur de frame depuis la derniere mise a jour de FPS
    Uint32 fps;                ///< Valeur actuelle des FPS
    float deltaTime;           ///< Temps (en s) ecoulé depuis la derniere frame
    Uint32 lastFrameTime;      ///< Temps (en ms) de la derniere frame
    Uint32 targetFPS;          ///< FPS visé, 0 signifie pas de limite
    Uint32 fpsUpdateInterval;  ///< Intervalle de mise à jour des fps (en ms). Par défaut: 1 seconde
} t_frameData;

/**
 * @brief Initialise une structure de gestion des frames
 * @param targetFps Nombre d'images par seconde cible (0 = pas de limite)
 * @return Pointeur vers la structure initialisée
 *
 * @details
 * - Crée les timers nécessaires au calcul des FPS et deltaTime
 * - Configure l'intervalle de mise à jour des FPS à 1 seconde
 *
 * @warning La structure doit être libérée avec freeFrameData()
 */
t_frameData* initFrameData(int targetFps);

/**
 * @brief Démarre le calcul d'une nouvelle frame
 * @param frame Pointeur vers la structure de frame
 *
 * @details
 * - Réinitialise le timer frameTimer pour mesurer la durée de la frame
 * - Incrémente le compteur de frames
 * - Met à jour les FPS si l'intervalle est atteint
 */
void startFrame(t_frameData* frame);

/**
 * @brief Limite le nombre d'images par seconde au targetFPS configuré
 * @param frame Pointeur vers la structure de frame
 *
 * @details
 * - Calcule le temps écoulé pour la frame actuelle
 * - Met en pause le thread si nécessaire pour atteindre la durée cible
 * - Met à jour le deltaTime pour les animations
 *
 * @note N'a aucun effet si targetFPS est 0
 */
void capFrameRate(t_frameData* frame);

/**
 * @brief Met à jour le calcul des FPS
 * @param frame Pointeur vers la structure de frame
 *
 * @details
 * - Calcule les FPS en divisant le nombre de frames par le temps écoulé
 * - Réinitialise le compteur de frames et le timer FPS
 *
 * @note Cette fonction est appelée automatiquement par startFrame() selon l'intervalle configuré
 */
void updateFPS(t_frameData* frame);

/**
 * @brief Libère les ressources associées à une structure de frame
 * @param frame Pointeur vers la structure à libérer
 *
 * @details
 * - Libère les timers internes
 * - Libère la structure elle-même
 */
void freeFrameData(t_frameData* frame);

#endif