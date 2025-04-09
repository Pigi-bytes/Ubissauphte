/**
 * @file commun.h
 * @brief Gestion du contenu utile pour le fonctionnement des différentes scènes
 *
 * Ce fichier centralise les inclusions communes, les constantes globales
 * et les déclarations de wrappers utilisés dans le projet.
 * Il sert de point d'entrée unique pour les fonctionnalités de base lors
 * de la création d'une scène.
 *
 * @author Ubissauphte
 * @date 2025
 * @version 1.0
 */

#ifndef COMMUN_H
#define COMMUN_H

#include "color.h"
#include "context.h"
#include "debug.h"
#include "engine/core/camera.h"
#include "engine/core/frame.h"
#include "engine/entities/enemy.h"
#include "engine/entities/mobs/slime.h"
#include "engine/entities/player.h"
#include "engine/entities/tiles.h"
#include "engine/world/generationSalle/geneRoom.h"
#include "engine/world/genmap.h"
#include "io/audioManager.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "ui/affichage.h"
#include "ui/button.h"
#include "ui/fpsDisplay.h"
#include "ui/mapview.h"
#include "ui/minimap.h"
#include "ui/slider.h"
#include "ui/text.h"
#include "ui/touche.h"
#include "utils/fonctionManager.h"
#include "utils/fscene.h"
#include "utils/objectManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

/**
 * @brief fonction de création d'un SDL_Rectangle
 * @param  x_ratio coordonnées x du rectangle
 * @param  y_ratio coordonnées y du rectangle
 * @param  w_ratio longueur du rectangle
 * @param h_ratio largueur du rectangle
 *
 * @return un SDL_rect
 */
SDL_Rect creerRect(float x_ratio, float y_ratio, float w_ratio, float h_ratio);

/**
 * @brief fonction de ferme le programe si le boutton l'utilisant est cliqué
 * @param  fonction les paramètres utile à l'execution du programme
 */
void bouttonClickQuit(t_fonctionParam* fonction);

/**
 * @brief fonction de changer la résolution de l'écran si le boutton l'utilisant est cliqué
 * @param  fonction les paramètres utile à l'execution du programme
 */
void changementAffichage(t_fonctionParam* fonction);

/*=== Wrappers d'affichage ===*/

/**
 * @brief Wrapper pour le rendu de texte
 * @param f Paramètres de fonction encapsulés
 */
void renderTextWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu des boutons UI
 * @param f Contient le contexte et les données du bouton
 */
void renderButtonWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour l'affichage du compteur FPS
 * @param f Contient le contexte et le composant FPS
 */
void renderFPSDisplayWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu de la grille de debug
 * @param f Contient le contexte et les paramètres de grille
 */
void renderGridWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu de la minimap
 * @param f Contient le contexte et la configuration de la minimap
 */
void renderMinimapWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu du viewport principal
 * @param f Contient le contexte et la configuration du viewport
 */
void renderViewportWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu du joueur
 * @param f Contient le contexte et l'entité joueur
 */
void renderPlayerWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu des contrôles affichés
 * @param f Contient le contexte et la configuration des touches
 */
void renderToucheWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu de la barre de volume
 * @param f Contient le contexte et le composant volume
 */
void renderBarreVolummeWrapper(t_fonctionParam* f);

/*=== Wrappers de mise à jour ===*/

/**
 * @brief Wrapper pour la mise à jour du compteur FPS
 * @param f Contient le contexte et le composant FPS
 */
void updateFPSDisplayWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la mise à jour de la minimap
 * @param f Contient le contexte et la configuration de la minimap
 */
void updateMinimapWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la mise à jour du joueur
 * @param f Contient le contexte et l'entité joueur
 */
void updatePlayerWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la gestion du scrolling
 * @param f Contient le contexte et les paramètres de scroll
 */
void updateScrollWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper générique de mise à jour
 * @param f Contient le contexte et les données à mettre à jour
 */
void updateWrapper(t_fonctionParam* f);

/*=== Wrappers de gestion d'entrées ===*/

/**
 * @brief Wrapper pour la gestion des entrées boutons
 * @param f Contient le contexte et le bouton cible
 */
void handleInputButtonWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la gestion des entrées joueur
 * @param f Contient le contexte et l'entité joueur
 */
void handleInputPlayerWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la gestion du volume
 * @param f Contient le contexte et le composant volume
 */
void handleInputButtonVolummeWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la gestion des contrôles affichés
 * @param f Contient le contexte et la configuration des touches
 */
void handleInputToucheWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la gestion de la carte
 * @param f Contient le contexte et la configuration de la map
 */
void handleInputMapWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la gestion de l'inventaire
 * @param f Contient le contexte et l'état de l'inventaire
 */
void handleInputInventaireWrapper(t_fonctionParam* f);

/*=== Wrappers système ===*/

/**
 * @brief Wrapper pour la gestion du zoom de la caméra
 * @param f Contient le contexte et la configuration de la caméra
 */
void cameraHandleZoomWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour définir la cible de rendu
 * @param f Contient le contexte et la texture cible
 */
void setRenderTargetWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour centrer la caméra
 * @param f Contient le contexte et la position cible
 */
void centerCameraOnWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour redimensionner le viewport
 * @param f Contient le contexte et les nouvelles dimensions
 */
void resizeViewportWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour redimensionner la minimap
 * @param f Contient le contexte et les nouvelles dimensions
 */
void resizeMinimapWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour changer de scène
 * @param f Contient le contexte et l'identifiant de scène
 */
void setSceneWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour revenir à la scène précédente
 * @param f Contient le contexte actuel
 */
void getPrevuisSceneWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour revenir au menu précédent
 * @param f Contient le contexte actuel
 */
void getPrevuisMenuWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper générique d'affichage
 * @param f Contient le contexte et les données à afficher
 */
void affichageWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour le rendu de l'UI d'inventaire
 * @param f Contient le contexte et l'état de l'inventaire
 */
void inventoryUI_RenderWrapper(t_fonctionParam* f);

/**
 * @brief Wrapper pour la mise à jour de l'UI d'inventaire
 * @param f Contient le contexte et l'état de l'inventaire
 */
void inventoryUI_UpdateWrapper(t_fonctionParam* f);

void jouerMusiqueWrapper(t_fonctionParam* fonction);

#endif