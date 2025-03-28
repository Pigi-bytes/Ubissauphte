#include "camera.h"

t_camera* createCamera(int levelW, int levelH, int camW, int camH) {
    t_camera* cam = malloc(sizeof(t_camera));
    if (!cam) {
        fprintf(stderr, "Erreur: Impossible d'allouer la mémoire pour la camera.\n");
        return NULL;
    }
    cam->shakeOffset.x = 0;
    cam->shakeOffset.y = 0;

    cam->shakeIntensity = 0.0;
    cam->shakeTimer = initDeltaTimer();
    startDeltaTimer(cam->shakeTimer);

    cam->x = 0;
    cam->y = 0;
    cam->w = camW;
    cam->h = camH;
    cam->levelW = levelW;
    cam->levelH = levelH;
    return cam;
}

void centerCameraOn(t_camera* cam, int* x, int* y, float* deltaTime) {
    cameraUpdateShake(cam, deltaTime);

    // X = position cible - demi-largeur de la vue camera
    // Y = position cible - demi-hauteur de la vue camera
    int newX = *x - (cam->w / 2);
    int newY = *y - (cam->h / 2);

    newX += cam->shakeOffset.x;
    newY += cam->shakeOffset.y;

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

void cameraHandleZoom(t_viewPort* vp, int* deltaZoom) {
    const float zoomSpeed = 0.05f;
    // Calcul du facteur de zoom multiplicatif
    // deltaZoom = (-1, +1), permet de savoir si on zoom avant ou arriere
    // 1.0f = permet de gardé la valeur de base quand on multiplie

    float zoom = 1.0f + *deltaZoom * zoomSpeed;

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
    if (vp->camera->shakeIntensity > 0) {
        SDL_RenderCopy(renderer, vp->renderTarget, &srcRect, &vp->screenRect);

        SDL_Texture* temp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, vp->screenRect.w, vp->screenRect.h);

        SDL_SetRenderTarget(renderer, temp);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, vp->renderTarget, &srcRect, NULL);

        SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_BLEND);
        Uint8 alpha = 150 - (vp->camera->shakeIntensity * 30);
        alpha = alpha > 255 ? 255 : (alpha < 50 ? 50 : alpha);  // Clamp [50-255]
        SDL_SetTextureAlphaMod(temp, alpha);

        SDL_SetRenderTarget(renderer, NULL);
        for (int i = 0; i < 3; i++) {
            SDL_Rect offsetRect = {vp->screenRect.x + (rand() % 5 - 2), vp->screenRect.y + (rand() % 5 - 2), vp->screenRect.w, vp->screenRect.h};
            SDL_RenderCopy(renderer, temp, NULL, &offsetRect);
        }
        SDL_DestroyTexture(temp);
    } else {
        SDL_RenderCopy(renderer, vp->renderTarget, &srcRect, &vp->screenRect);
    }
}

void setRenderTarget(SDL_Renderer* renderer, t_viewPort* vp) {
    SDL_SetRenderTarget(renderer, vp->renderTarget);
}

void resizeViewport(t_viewPort* vp, int* windowW, int* windowH) {
    vp->screenRect.w = *windowW;
    vp->screenRect.h = *windowH;
}

void freeViewport(void* object) {
    t_viewPort* vp = (t_viewPort*)object;
    if (vp) {
        SDL_DestroyTexture(vp->renderTarget);
        free(vp);
    }
}

void freeCamera(void* object) {
    t_camera* cam = (t_camera*)object;
    if (cam) {
        free(cam);
    }
}

SDL_bool isRectOnCamera(SDL_Rect* rect, t_camera* camera) {
    return !((rect->x + rect->w <= camera->x) ||    // Trop a gauche
             (rect->x >= camera->x + camera->w) ||  // Trop a droite
             (rect->y + rect->h <= camera->y) ||    // Trop en haut
             (rect->y >= camera->y + camera->h));   // Trop en bas
}

void convertMouseToWorld(t_viewPort* vp, int mouseX, int mouseY, float* worldX, float* worldY) {
    // Conversion viewport => espace normalisé [0-1]
    float nx = (mouseX - vp->screenRect.x) / (float)vp->screenRect.w;
    float ny = (mouseY - vp->screenRect.y) / (float)vp->screenRect.h;

    // Application du zoom et du decalage de la caméra
    *worldX = vp->camera->x + (nx * vp->camera->w);
    *worldY = vp->camera->y + (ny * vp->camera->h);
}

void cameraUpdateShake(t_camera* cam, float* deltaTime) {
    if (cam->shakeTimer->accumulatedTime <= 0.0f) return;

    updateDeltaTimer(cam->shakeTimer, *deltaTime);

    float currentTime = getDeltaTimer(cam->shakeTimer);
    float progress = currentTime / cam->shakeTimer->accumulatedTime;

    float currentIntensity = cam->shakeIntensity * (1.0f - progress * progress);

    cam->shakeIntensity = currentIntensity;

    cam->shakeOffset.x = (rand() % (int)(currentIntensity * 2 + 1)) - currentIntensity;
    cam->shakeOffset.y = (rand() % (int)(currentIntensity * 2 + 1)) - currentIntensity;

    if (currentTime >= cam->shakeTimer->accumulatedTime) {
        resetDeltaTimer(cam->shakeTimer);
        cam->shakeOffset.x = 0;
        cam->shakeOffset.y = 0;
        cam->shakeIntensity = 0.0f;
    }
}

void cameraAddShake(t_camera* cam, float intensity, float duration) {
    if (intensity > cam->shakeIntensity) {
        cam->shakeIntensity = intensity;
        cam->shakeTimer->accumulatedTime = duration;
        cam->shakeTimer->isStarted = SDL_FALSE;
    }
}
