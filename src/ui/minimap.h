/**
 * @file minimap.h
 * @brief Gestion de la minimap (affichage et mise a jour)
 */

#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL2/SDL.h>

#include "../tileEngine/camera.h"

/**
 * @def MINIMAP_SIZE
 * @brief Taille d'un coté de la minimap en pixels
 */
#define MINIMAP_SIZE 200

/**
 * @def MINIMAP_MARGIN
 * @brief Marge entre la minimap et le bord de l'écran en pixels
 */
#define MINIMAP_MARGIN 10

/**
 * @struct t_minimap
 * @brief Structure contenant tous les éléments relatifs a la minimap
 */
typedef struct {
    SDL_Texture* texture;  ///< Texture SDL rendue de la minimap
    int size;              ///< Taille d'un de la minimap (doit être carrée)
    int margin;            ///< Marge avec les bords de l'écran
    SDL_Rect area;         ///< Rectangle d'affichage (position ET taille)
} t_minimap;

/**
 * @brief Crée et initialise une nouvelle minimap
 * @param renderer Renderer SDL pour la création de textures
 * @param windowW Largeur de la fenêtre
 * @param windowH Hauteur de la fenêtre
 * @return Pointeur sur la minimap allouée, NULL en cas d'erreur
 * @warning La minimap doit être libérée avec freeMinimap()
 */
t_minimap* createMinimap(SDL_Renderer* renderer, int windowW, int windowH);

/**
 * @brief Met a jour le contenu de la minimap
 * @param renderer Renderer SDL pour le rendu intermédiaire
 * @param minimap Pointeur sur la minimap à mettre à jour
 * @param camera Pointeur vers la caméra
 * @note Doit être appelee a chaque changement de la caméra
 */
void updateMinimap(SDL_Renderer* renderer, t_minimap* minimap, t_camera* camera);

/**
 * @brief Dessine la minimap a l'écran
 * @param renderer Renderer SDL pour l'affichage
 * @param minimap Pointeur sur la minimap à afficher
 */
void renderMinimap(SDL_Renderer* renderer, t_minimap* minimap);

/**
 * @brief Libere la mémoire allouée pour la minimap
 * @param minimap Pointeur sur la minimap à détruire
 */
void freeMinimap(t_minimap* minimap);

/**
 * @brief Change la position de la minimap selon la taille de l'eccran
 * @param renderer Renderer SDL pour la création de textures
 * @param minimap Pointeur sur la minimap à mettre à jour
 * @param windowW Largeur de la fenêtre
 * @param windowH Hauteur de la fenêtre
 */
void resizeMinimap(SDL_Renderer* renderer, t_minimap* minimap, int windowW, int windowH);

#endif