/**
 * @file tileset.h
 * @brief Gestion des ensembles de tuiles (tilesets) pour les niveaux
 */

#ifndef TILESET_H
#define TILESET_H

#include <SDL2/SDL.h>

#include "../io/imageLoader.h"
#include "../utils/objectManager.h"

/**
 * @struct t_tileset
 * @brief Représente un ensemble de tuiles utilisées pour construire les niveaux
 */
typedef struct {
    int width;                      ///< Largeur du tileset en nombre de tuiles
    int height;                     ///< Hauteur du tileset en nombre de tuiles
    int tileSize;                   ///< Taille d'une tuile individuelle en pixels
    t_objectManager* textureTiles;  ///< Gestionnaire des textures des tuiles
} t_tileset;

/**
 * @brief Initialise un nouveau tileset à partir d'une image
 * @param renderer Renderer SDL pour créer les textures
 * @param width Largeur du tileset en nombre de tuiles
 * @param height Hauteur du tileset en nombre de tuiles
 * @param tileSize Taille d'une tuile individuelle en pixels
 * @param filename Chemin vers l'image du tileset
 * @return Pointeur vers le tileset initialisé
 */
t_tileset* initTileset(SDL_Renderer* renderer, int width, int height, int tileSize, char* filename);

/**
 * @brief Libère la mémoire utilisée par un tileset
 * @param tileset Pointeur vers le tileset à libérer
 */
void freeTileset(t_tileset* tileset);

/**
 * @brief Wrapper pour SDL_DestroyTexture (compatible avec objectManager)
 * @param object Pointeur vers la texture SDL à détruire
 */
void SDL_DestroyTextureWrapper(void* object);

#endif