#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "../debug.h"

#define ZOOM_STEP 0.05f
#define MINIMAP_SIZE 200
#define MINIMAP_MARGIN 10
#define MINIMAP_BG_COLOR 50, 50, 50, 255
#define MINIMAP_BORDER_COLOR 100, 100, 100, 255
#define CAMERA_VIEW_COLOR 255, 0, 0, 255

typedef struct {
    int centerCameraX;  // [Unite Monde] Centre de la camera X dans le monde
    int centerCameraY;  // [Unite Monde] Centre de la camera Y dans le monde

    float camWidth;    // [Unite Monde] Largeur visible par la camera
    float camHeight;   // [Unite Monde] Hauteur visible par la camera
    int windowWidth;   // [Pixels] Largeur de la fenêtre
    int windowHeight;  // [Pixels] Hauteur de la fenêtre

    float scaleX;  // [Pixels/Unite Monde] Zoom Largeur
    float scaleY;  // [Pixels/Unite Monde] Zoom Hauteur

    int worldWidth;   // [Unite Monde] Total largeur monde
    int worldHeight;  // [Unite Monde] Total hauteur monde
} t_camera;

typedef struct {
    SDL_Rect rect;  // Position et taille dans le monde
    SDL_Texture* texture;
} t_entity;

void cameraHandleResize(t_camera* camera, int newWidth, int newHeight);
void cameraHandleZoom(t_camera* camera, float deltaZoom, int screenWidth, int screenHeight);

void cameraCenterOnRect(t_camera* camera, SDL_Rect* target);

SDL_Rect cameraTransformWorld2Camera(t_camera* camera, SDL_Rect* rectWordCoord);

SDL_bool entityOnCamera(t_camera* camera, SDL_Rect* rectWordCoord);
void renderEntity(SDL_Renderer* renderer, t_camera* camera, t_entity* entity);
void renderMinimap(SDL_Renderer* renderer, t_camera* camera, int screenWidth, int screenHeight);

#endif