#include "camera.h"

void renderMinimap(SDL_Renderer* renderer, t_camera* camera, int screenWidth, int screenHeight) {
    SDL_Rect minimapArea = {
        screenWidth - MINIMAP_SIZE - MINIMAP_MARGIN,
        screenHeight - MINIMAP_SIZE - MINIMAP_MARGIN,
        MINIMAP_SIZE,
        MINIMAP_SIZE};

    SDL_SetRenderDrawColor(renderer, MINIMAP_BG_COLOR);
    SDL_RenderFillRect(renderer, &minimapArea);

    SDL_SetRenderDrawColor(renderer, MINIMAP_BORDER_COLOR);
    SDL_RenderDrawRect(renderer, &minimapArea);

    float scaleX = (float)MINIMAP_SIZE / camera->worldWidth;
    float scaleY = (float)MINIMAP_SIZE / camera->worldHeight;

    // Calcul de la zone visible de la caméra
    const SDL_Rect cameraView = {
        minimapArea.x + (int)(camera->centerCameraX * scaleX),
        minimapArea.y + (int)(camera->centerCameraY * scaleY),
        (int)(camera->camWidth * scaleX),
        (int)(camera->camHeight * scaleY)};

    SDL_SetRenderDrawColor(renderer, CAMERA_VIEW_COLOR);
    SDL_RenderDrawRect(renderer, &cameraView);
}

void cameraHandleResize(t_camera* camera, int newWidth, int newHeight) {
    camera->windowWidth = newWidth;
    camera->windowHeight = newHeight;

    // Calcul du zoom pour adapter la vue monde a l'ecran
    // Zoom horizontal = largeur écran / largeur vue monde
    // Zoom vertical = hauteur écran / hauteur vue monde
    camera->scaleX = (float)newWidth / camera->camWidth;
    camera->scaleY = (float)newHeight / camera->camHeight;
}

void cameraHandleZoom(t_camera* camera, float deltaZoom, int screenWidth, int screenHeight) {
    // Calcul du facteur de zoom multiplicatif
    // deltaZoom = (-1, +1), permet de savoir si on zoom avant ou arriere
    // 1.0f = permet de gardé la valeur de base quand on multiplie
    float zoom = 1.0f + deltaZoom * ZOOM_STEP;

    // Multiplication par le nouveau facteur (avec 1.0 on s'assure de modifier l'ancienne valeur)
    camera->camWidth *= zoom;
    camera->camHeight *= zoom;

    // Limite du zoom
    float minViewW = screenWidth * 0.1f;   // Zoom avant max = 10% ecran
    float minViewH = screenHeight * 0.1f;  // Meme chose pour la hauteur
    float maxViewW = camera->worldWidth;   // Zoom arriere max = taille totale monde
    float maxViewH = camera->worldHeight;  // Idem pour la hauteur

    // Clamping des valeurs entre les limites min/max
    camera->camWidth = fmaxf(minViewW, fminf(camera->camWidth, maxViewW));
    camera->camHeight = fmaxf(minViewH, fminf(camera->camHeight, maxViewH));

    // Mettre à jour les zooms X/Y
    // Calcul inverse base sur la nouvelle zone visible
    camera->scaleX = (float)screenWidth / camera->camWidth;
    camera->scaleY = (float)screenHeight / camera->camHeight;

    DEBUG_PRINT("Zoom : viewWidth = %.2f, viewHeight = %.2f\n", camera->camWidth, camera->camHeight);
}

void cameraCenterOnRect(t_camera* camera, SDL_Rect* target) {
    // Position X = centre de la cible - demi largeur ecran (en coordonnees monde)
    // Position Y = centre de la cible - demi hauteur ecran (en coordonnees monde)
    camera->centerCameraX = (target->x + target->w / 2) - (camera->windowWidth / (2 * camera->scaleX));
    camera->centerCameraY = (target->y + target->h / 2) - (camera->windowHeight / (2 * camera->scaleY));

    // Calcul des limites maximales pour pas sortir du monde
    // Max X = largeur monde - largeur vue camera
    // Max Y = hauteur monde - hauteur vue camera
    float maxX = camera->worldWidth - (camera->windowWidth / camera->scaleX);
    float maxY = camera->worldHeight - (camera->windowHeight / camera->scaleY);

    camera->centerCameraX = fmaxf(0, fminf(camera->centerCameraX, maxX));  // X entre 0 et maxX
    camera->centerCameraY = fmaxf(0, fminf(camera->centerCameraY, maxY));  // Y entre 0 et maxY
}

SDL_Rect cameraTransformWorld2Camera(t_camera* camera, SDL_Rect* rectWorld) {
    return (SDL_Rect){
        // Position ecran = (position monde - position camera) * zoom
        .x = (rectWorld->x - camera->centerCameraX) * camera->scaleX,
        .y = (rectWorld->y - camera->centerCameraY) * camera->scaleY,

        // Taille ecran = taille monde * zoom
        .w = rectWorld->w * camera->scaleX,
        .h = rectWorld->h * camera->scaleY};
}

SDL_bool entityOnCamera(t_camera* camera, SDL_Rect* rectWorld) {
    // Calcul des bords de la camera en coordonne monde
    float camRight = camera->centerCameraX + (camera->windowWidth / camera->scaleX);
    float camBottom = camera->centerCameraY + (camera->windowHeight / camera->scaleY);

    return !(rectWorld->x + rectWorld->w < camera->centerCameraX ||  // Trop a gauche
             rectWorld->x > camRight ||                              // Trop a droite
             rectWorld->y + rectWorld->h < camera->centerCameraY ||  // Trop en haut
             rectWorld->y > camBottom);                              // Trop en bas
}

void renderEntity(SDL_Renderer* renderer, t_camera* camera, t_entity* entity) {
    // Verification pour savoir si l'entité est sur l'ecran
    if (!entityOnCamera(camera, &entity->rect)) {
        return;
    }

    SDL_Rect screen_rect = cameraTransformWorld2Camera(camera, &entity->rect);
    if (entity->texture) {
        SDL_RenderCopy(renderer, entity->texture, NULL, &screen_rect);
    }
}
