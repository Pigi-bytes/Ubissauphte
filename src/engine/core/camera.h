/**
 * @file camera.h
 * @brief Gestion de la caméra et du viewport
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "../../debug.h"

/**
 * @struct t_camera
 * @brief Représente une caméra 2D pour naviguer dans un niveau
 *
 * La camera définit une région rectangulaire visible dans l'espace du niveau
 * Utilisé pour le rendu partiel de grands niveaux
 */
typedef struct {
    int x, y;            ///< Coin supérieur gauche en coordonnées niveau (pixels)
    int w, h;            ///< Dimensions de la zone visible (pixels)
    int levelW, levelH;  ///< Dimensions totales du niveau (limites de déplacement)
} t_camera;

/**
 * @struct t_viewPort
 * @brief Lie une caméra à une texture de rendu et une zone d'affichage écran
 *
 * Le viewport permet de rendre une portion du niveau (définie par la caméra)
 * a une resolution natif avant de l'afficher a l'ecran
 */
typedef struct {
    SDL_Rect screenRect;        ///< Zone d'affichage à l'écran (position + taille)
    SDL_Texture* renderTarget;  ///< Texture SDL oo le niveau est rendu
    t_camera* camera;           ///< Caméra associée à ce viewport
} t_viewPort;

/**
 * @brief Crée une caméra avec des dimensions initiales
 * @param levelW Largeur totale du niveau en pixels
 * @param levelH Hauteur totale du niveau en pixels
 * @param camW Largeur initiale de la caméra en pixels
 * @param camH Hauteur initiale de la caméra en pixels
 * @return Pointeur vers la caméra initialisée, NULL en cas d'erreur
 *
 * @warning La caméra doit être libérée avec freeCamera()
 */
t_camera* createCamera(int levelW, int levelH, int camW, int camH);

/**
 * @brief Centre la caméra sur des coordonnées précises en ne sortant pas du niveau
 * @param cam Caméra à modifier
 * @param x Position X souhaitée (centre de la caméra)
 * @param y Position Y souhaitée (centre de la caméra)
 *
 * @details
 * @note Les coordonnées (x,y) sont relatives au niveau, pas à l'écran
 */
void centerCameraOn(t_camera* cam, int* x, int* y);

/**
 * @brief Applique un zoom relatif à la caméra via le viewport
 * @param vp Viewport contenant la caméra à zoomer
 * @param deltaZoom Variation du zoom (>0 = zoom avant, <0 = zoom arrière)
 *
 * @details
 * - Le zoom est calculé proportionnellement à la taille actuelle
 * - Les limites de zoom sont :
 *   - Zoom max avant : 10% de la taille de l'écran
 *   - Zoom max arrière : taille totale du niveau
 * - Met à jour w/h de la caméra
 */
void cameraHandleZoom(t_viewPort* vp, float deltaZoom);

/**
 * @brief Crée un viewport lié à une caméra
 * @param renderer Renderer SDL pour créer la texture
 * @param camera Caméra à associer
 * @param windowW Largeur de la fenêtre
 * @param windowH Hauteur de la fenêtre
 * @return Pointeur vers le viewport initialisé, NULL en cas d'erreur
 *
 * @details
 * - Crée une texture de la taille du niveau pour pré-rendre
 * - Initialise screenRect avec les dimensions de la fenêtre
 *
 * @note Le viewport doit être libérée avec freeViewport()
 */
t_viewPort* createViewport(SDL_Renderer* renderer, t_camera* camera, int windowW, int windowH);

/**
 * @brief Définit la texture natif pour le rendu du niveau
 * @param renderer Renderer SDL
 * @param vp Viewport contenant la texture cible
 *
 * @details
 * - Tous les dessins suivants seront sur renderTarget
 *
 * @note Il faut appelé renderViewport() pour redonner le renderer
 */
void setRenderTarget(SDL_Renderer* renderer, t_viewPort* vp);

/**
 * @brief Rend le viewport à l'écran en copiant la texture natif et en l'upscalant a la taille de l'ecran
 * @param renderer Renderer SDL
 * @param vp Viewport à afficher
 *
 */
void renderViewport(SDL_Renderer* renderer, t_viewPort* vp);

/**
 * @brief Met à jour les dimensions du viewport
 * @param vp Viewport à modifier
 * @param windowW Nouvelle largeur de fenêtre
 * @param windowH Nouvelle hauteur de fenêtre
 *
 * @note Doit être appelee a chaque fois que l'on redimensionne la fenetre
 */
void resizeViewport(t_viewPort* vp, int windowW, int windowH);

/**
 * @brief Libere la mémoire allouée pour le viewport
 * @param vp Pointeur sur le viewport à détruire
 */
void freeViewport(void* object);

/**
 * @brief Libere la mémoire allouée pour la camera
 * @param cam Pointeur sur la camera à détruire
 */
void freeCamera(void* object);

/**
 * @brief Vérifie si un rectangle est visible dans la caméra
 * @param rect Rectangle à vérifier (coordonnées niveau en pixels)
 * @param camera Caméra de référence
 * @return SDL_TRUE si au moins 1 pixel est visible, false sinon
 */
SDL_bool isRectOnCamera(SDL_Rect* rect, t_camera* camera);

#endif