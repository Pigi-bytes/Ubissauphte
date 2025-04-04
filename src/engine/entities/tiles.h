/**
 * @file tiles.h
 * @brief Système de gestion des tuiles et des grilles de niveau
 */

#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "../../debug.h"
#include "../core/camera.h"
#include "../tileset.h"
#include "components/animation/animation.h"
#include "entity.h"

/**
 * @def MAX_LINE_LENGTH
 * @brief Longueur maximale d'une ligne lors de la lecture de fichiers de carte
 */
#define MAX_LINE_LENGTH 2048

/**
 * @struct t_tile
 * @brief Représente une tuile individuelle dans la grille
 */
typedef struct {
    SDL_bool solide;  ///< Propriété de collision (SDL_TRUE = solide, SDL_FALSE = traversable)
    t_entity entity;  ///< Entité associée pour le rendu
} t_tile;

/**
 * @struct s_grid
 * @brief Représente une grille 3D de tuiles pour un niveau
 */
typedef struct s_grid {
    int width;        ///< Largeur de la grille en nombre de tuiles
    int height;       ///< Hauteur de la grille en nombre de tuiles
    int depth;        ///< Profondeur de la grille (nombre de couches)
    t_tile*** tiles;  ///< Tableau à 3 dimensions [profondeur][hauteur][largeur]
} t_grid;

/**
 * @brief Récupère une tuile à des coordonnées spécifiques
 * @param grid Grille source
 * @param x Position X dans la grille
 * @param y Position Y dans la grille
 * @param z Couche (profondeur) dans la grille
 * @return Pointeur vers la tuile, ou NULL si les coordonnées sont invalides
 */
t_tile* getTile(t_grid* grid, int x, int y, int z);

/**
 * @brief Crée une nouvelle grille aux dimensions spécifiées
 * @param width Largeur de la grille en tuiles
 * @param height Hauteur de la grille en tuiles
 * @param depth Nombre de couches de la grille
 * @return Pointeur vers la grille créée
 */
t_grid* createGrid(int width, int height, int depth);

/**
 * @brief Charge une carte depuis un fichier
 * @param filename Chemin vers le fichier de carte
 * @param tileset Tileset contenant les textures des tuiles
 * @return Pointeur vers la grille chargée
 */
t_grid* loadMap(char* filename, t_tileset* tileset);

/**
 * @brief Affiche une grille de tuiles
 * @param renderer Renderer SDL pour l'affichage
 * @param grid Grille à afficher
 * @param camera Caméra active pour la conversion de coordonnées
 */
void renderGrid(SDL_Renderer* renderer, t_grid* grid, t_camera* camera);

/**
 * @brief Libère la mémoire utilisée par une grille
 * @param object Pointeur vers la grille à libérer
 */
void freeGrid(void* object);

/**
 * @brief Place une entité sur une tuile libre aléatoire
 * @param grid Grille de niveau
 * @param entity Entité à placer
 * @param entities Gestionnaire des entités
 */
void placeOnRandomTile(t_grid* grid, t_entity* entity, t_objectManager* entities);

/**
 * @brief Wrapper pour SDL_DestroyTexture (compatible avec objectManager)
 * @param object Pointeur vers la texture SDL à détruire
 */
void SDL_DestroyTextureWrapper(void* object);

#endif