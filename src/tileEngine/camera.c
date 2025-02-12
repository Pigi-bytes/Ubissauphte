#include "camera.h"

t_camera* createCamera(int levelW, int levelH, int camW, int camH) {
    t_camera* cam = malloc(sizeof(t_camera));
    if (!cam) {
        fprintf(stderr, "Erreur: Impossible d'allouer la mémoire pour la camera.\n");
        return NULL;
    }
    cam->x = 0;
    cam->y = 0;
    cam->w = camW;
    cam->h = camH;
    cam->levelW = levelW;
    cam->levelH = levelH;
    return cam;
}

void centerCameraOn(t_camera* cam, int x, int y) {
    // X = position cible - demi-largeur de la vue camera
    // Y = position cible - demi-hauteur de la vue camera
    int newX = x - (cam->w / 2);
    int newY = y - (cam->h / 2);

    // Calcul des limites maximales autorisées
    // Max X = taille niveau - largeur camera (pour éviter sortie droite/bas)
    // Max Y = taille niveau - hauteur camera
    if (newX < 0) newX = 0;
    if (newY < 0) newY = 0;
    if (newX + cam->w > cam->levelW) newX = cam->levelW - cam->w;
    if (newY + cam->h > cam->levelH) newY = cam->levelH - cam->h;

    cam->x = newX;
    cam->y = newY;
}

void cameraHandleZoom(t_viewPort* vp, float deltaZoom) {
    const float zoomSpeed = 0.05f;
    // Calcul du facteur de zoom multiplicatif
    // deltaZoom = (-1, +1), permet de savoir si on zoom avant ou arriere
    // 1.0f = permet de gardé la valeur de base quand on multiplie

    float zoom = 1.0f + deltaZoom * zoomSpeed;

    // Multiplication par le nouveau facteur (avec 1.0 on s'assure de modifier l'ancienne valeur)
    vp->camera->w *= zoom;
    vp->camera->h *= zoom;

    float minViewW = vp->screenRect.w * 0.1f;  // Zoom avant max = 10% ecran
    float minViewH = vp->screenRect.h * 0.1f;  // Meme chose pour la hauteur
    float maxViewW = vp->camera->levelW;       // Zoom arriere max = taille totale monde
    float maxViewH = vp->camera->levelH;       // Idem pour la hauteur

    // Clamping des valeurs entre les limites min/max
    vp->camera->w = fmaxf(minViewW, fminf(vp->camera->w, maxViewW));
    vp->camera->h = fmaxf(minViewH, fminf(vp->camera->h, maxViewH));
}

t_viewPort* createViewport(SDL_Renderer* renderer, t_camera* camera, int windowW, int windowH) {
    t_viewPort* vp = malloc(sizeof(t_viewPort));
    if (!vp) {
        fprintf(stderr, "Erreur: Impossible d'allouer la mémoire pour le viewport.\n");
        return NULL;
    }

    vp->screenRect = (SDL_Rect){0, 0, windowW, windowH};
    vp->camera = camera;
    vp->renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, camera->levelW, camera->levelH);

    return vp;
}

void renderViewport(SDL_Renderer* renderer, t_viewPort* vp) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect srcRect = {vp->camera->x, vp->camera->y, vp->camera->w, vp->camera->h};
    SDL_RenderCopy(renderer, vp->renderTarget, &srcRect, &vp->screenRect);
}

void setRenderTarget(SDL_Renderer* renderer, t_viewPort* vp) {
    SDL_SetRenderTarget(renderer, vp->renderTarget);
}

void updateViewport(t_viewPort* vp, int windowW, int windowH) {
    vp->screenRect.w = windowW;
    vp->screenRect.h = windowH;
}

void freeViewport(t_viewPort* vp) {
    if (vp) {
        SDL_DestroyTexture(vp->renderTarget);
        free(vp);
    }
}

void freeCamera(t_camera* cam) {
    if (cam) {
        free(cam);
    }
}