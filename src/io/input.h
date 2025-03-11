/**
 * @file input.h
 * @brief Implémentation de la gestion des entrées
 */

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "../debug.h"
/**
 * @struct t_input
 * @brief Contient l'état actuel du clavier, de la souris et de la fenêtre
 */
typedef struct {
    int key[SDL_NUM_SCANCODES];    ///< Touches clavier à l'index SDL_SCANCODE
    SDL_bool quit;                 ///< Flag de fermeture de la fenetre
    SDL_bool resized;              ///< Flag de redimensionnement de la fenetre
    int mouseX, mouseY;            ///< Position X/Y de la souris en pixels écran
    int mouseXWheel, mouseYWheel;  ///< Deplacement de la molette souris (-1, 0, +1)
    SDL_bool mouseButtons[6];      ///< Etat des boutons souris (0-5 pour les 6 boutons SDL)
    int windowWidth;               ///< Largeur actuelle de la fenetre
    int windowHeight;              ///< Hauteur actuelle de la fenetre
} t_input;

/**
 * @brief Met a jour l'etat des entrees en lisant la file d'event
 * @param input Pointeur vers la structure d'input à mettre à jour
 */
void updateInput(t_input* input);

/**
 * @brief Initialise la structure d'entrées avec des valeurs par défaut
 * @param windowWidth Largeur initiale de la fenêtre
 * @param windowHeight Hauteur initiale de la fenêtre
 * @return input Pointeur vers la structure initialiser
 */
t_input* initInput(int windowWidth, int windowHeight);

/**
 * @brief Détecte un appui unique sur une touche malgrés les frames
 * @param input Pointeur vers la structure d'entrées
 * @param scancode Code SDL_Scancode de la touche à vérifier
 * @return SDL_TRUE si la touche vient d'être pressée, SDL_FALSE sinon
 *
 * @note Utilise un tableau static pour suivre l'état des touches entre les appels
 */
SDL_bool keyPressOnce(t_input* input, SDL_Scancode scancode);

/**
 * @brief Libere la mémoire allouée pour l'input
 * @param input Pointeur sur l'input à détruire
 */
void freeInput(t_input* input);

int indiceToucheCliquer(t_input* input);

#endif  // INPUT_H
