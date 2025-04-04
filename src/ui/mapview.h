/**
 * @file mapview.h
 * @brief Gestion de l'affichage de la carte du niveau
 *
 * Ce module fournit des fonctions pour générer et afficher
 * une vue schématique des salles et leurs connexions.
 */

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <SDL2/SDL.h>

#include "../engine/entities/player.h"
#include "../engine/world/genmap.h"

/**
 * @struct Line
 * @brief Représente une ligne connectant deux salles sur la carte
 */
typedef struct {
    int x1, y1;  ///< Coordonnées du point de départ
    int x2, y2;  ///< Coordonnées du point d'arrivée
} Line;

/**
 * @struct t_mapAffichage
 * @brief Structure contenant les éléments d'affichage de la carte
 */
typedef struct {
    SDL_Rect *rooms;  ///< Tableau des rectangles représentant les salles
    int numRooms;     ///< Nombre de salles
    Line *lines;      ///< Tableau des lignes connectant les salles
    int numLines;     ///< Nombre de lignes
} t_mapAffichage;

/**
 * @brief Détermine les limites min/max des coordonnées des salles
 * @param zonneAff Tableau recevant les coordonnées min/max [xMin, xMax, yMin, yMax]
 * @param roomCoords Tableau des coordonnées des salles
 * @param numberRoom Nombre de salles
 */
void XYMinMax(int *zonneAff, SDL_Rect *roomCoords, int numberRoom);

/**
 * @brief Génère la représentation visuelle de la carte
 * @param roomCoords Coordonnées des salles
 * @param listeRoom Liste des salles du niveau
 * @param numberRoom Nombre de salles
 * @param map Structure recevant les données d'affichage
 * @param WW Largeur de la fenêtre
 * @param WH Hauteur de la fenêtre
 */
void generateMap(SDL_Rect *roomCoords, t_salle **listeRoom, int numberRoom, t_mapAffichage *map, int WW, int WH);

/**
 * @brief Affiche la carte à l'écran
 * @param renderer Renderer SDL utilisé pour le rendu
 * @param map Structure contenant les données d'affichage
 * @param player Joueur (pour indiquer sa position)
 */
void affichage(SDL_Renderer *renderer, t_mapAffichage *map, t_joueur *player);

/**
 * @brief Gère les entrées utilisateur dans la vue carte
 * @param input Gestionnaire d'entrées
 * @param player Joueur
 * @param sceneController Contrôleur de scènes
 */
void handleInputMap(t_input *input, t_joueur *player, t_sceneController *sceneController);

/**
 * @brief Libère la mémoire utilisée par la structure d'affichage de carte
 * @param elt Pointeur vers la structure à libérer
 */
void freeMapAffiche(void *elt);

#endif