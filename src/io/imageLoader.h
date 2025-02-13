/**
 * @file image_loader.h
 * @brief Chargement d'images dans des textures SDL
 */

#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "../debug.h"

/**
 * @brief Charge une image depuis un fichier dans une texture SDL
 * @param filename Chemin vers le fichier image (forcement .bmp)
 * @param renderer Pointeur vers le renderer SDL pour créer la texture
 * @return Pointeur vers la texture SDL chargée, NULL en cas d'erreur
 *
 * @warning La texture doit être détruite avec SDL_DestroyTexture()
 */
SDL_Texture* loadImage(char* filename, SDL_Renderer* renderer);

#endif