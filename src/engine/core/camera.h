/**
 * @file camera.h
 * @brief Gestion de la caméra et du viewport
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "../../debug.h"
#include "../../utils/timer.h"

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

    float shakeIntensity;  ///< Force actuelle du shake (0 = aucun)
    float baseShakeIntensity;
    t_deltaTimer* shakeTimer;  // Timer pour la durée du shake
    SDL_Point shakeOffset;     ///< Décalage aléatoire actuel
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
void centerCameraOn(t_camera* cam, int* x, int* y, float*);

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
void cameraHandleZoom(t_viewPort* vp, int* deltaZoom);

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
void resizeViewport(t_viewPort* vp, int* windowW, int* windowH);

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

/**
 * @brief Convertit des coordonnées souris (écran) en coordonnées monde (niveau)
 * @param vp Viewport contenant la caméra de référence
 * @param mouseX Position X de la souris en pixels écran
 * @param mouseY Position Y de la souris en pixels écran
 * @param worldX Pointeur pour stocker la coordonnée X monde résultante
 * @param worldY Pointeur pour stocker la coordonnée Y monde résultante
 *
 * @details
 * - Prend en compte le zoom et la position de la caméra
 * - Utilise les proportions du viewport pour la conversion
 */
void convertMouseToWorld(t_viewPort* vp, int mouseX, int mouseY, float* worldX, float* worldY);

/**
 * @brief Ajoute un effet de tremblement à la caméra
 * @param cam Caméra à faire trembler
 * @param intensity Intensité du tremblement (pixels de déplacement max)
 * @param duration Durée du tremblement en secondes
 *
 * @details
 * - L'intensité diminue progressivement jusqu'à zéro
 * - Le tremblement est aléatoire sur les axes X et Y
 */
void cameraAddShake(t_camera* cam, float intensity, float duration);

/**
 * @brief Met à jour l'effet de tremblement de la caméra
 * @param cam Caméra à mettre à jour
 * @param deltaTime Temps écoulé depuis la dernière mise à jour en secondes
 *
 * @details
 * - Génère de nouveaux déplacements aléatoires
 * - Diminue l'intensité en fonction du temps écoulé
 * - Désactive le tremblement quand l'intensité atteint zéro
 */
void cameraUpdateShake(t_camera* cam, float* deltaTime);

#endif