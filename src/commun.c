#include "commun.h"

void renderTextWrapper(t_fonctionParam* f) {
    renderText(((SDL_Renderer*)(f)->param[0]), ((t_text*)(f)->param[1]));
}
void renderButtonWrapper(t_fonctionParam* f) {
    renderButton(((SDL_Renderer*)(f)->param[0]), ((t_button*)(f)->param[1]));
}

void renderFPSDisplayWrapper(t_fonctionParam* f) {
    renderFPSDisplay(((SDL_Renderer*)(f)->param[0]), ((t_fpsDisplay*)(f)->param[1]));
}

void renderGridWrapper(t_fonctionParam* f) {
    renderGrid(((SDL_Renderer*)(f)->param[0]), ((t_grid*)(f)->param[1]), ((t_camera*)(f)->param[2]));
}

void renderMinimapWrapper(t_fonctionParam* f) {
    renderMinimap(((SDL_Renderer*)(f)->param[0]), ((t_minimap*)(f)->param[1]));
}

void renderViewportWrapper(t_fonctionParam* f) {
    renderViewport(((SDL_Renderer*)(f)->param[0]), ((t_viewPort*)(f)->param[1]));
}

void renderPlayerWrapper(t_fonctionParam* f) {
    renderPlayer(((SDL_Renderer*)(f)->param[0]), ((t_joueur*)(f)->param[1]), ((t_camera*)(f)->param[2]));
}

void renderEnemyWrapper(t_fonctionParam* f) {
    renderEnemy(((SDL_Renderer*)(f)->param[0]), ((t_enemy*)(f)->param[1]), ((t_camera*)(f)->param[2]));
}

void updateFPSDisplayWrapper(t_fonctionParam* f) {
    updateFPSDisplay(((t_fpsDisplay*)(f)->param[0]), ((t_frameData*)(f)->param[1]), ((SDL_Renderer*)(f)->param[2]));
}

void updateMinimapWrapper(t_fonctionParam* f) {
    updateMinimap(((t_minimap*)(f)->param[0]), ((t_camera*)(f)->param[1]), ((SDL_Renderer*)(f)->param[2]));
}

void cameraHandleZoomWrapper(t_fonctionParam* f) {
    cameraHandleZoom(((t_viewPort*)(f)->param[0]), ((int*)(f)->param[1]));
}

void handleInputButtonWrapper(t_fonctionParam* f) {
    handleInputButton(((t_input*)(f)->param[0]), ((t_button*)(f)->param[1]));
}

void handleInputPlayerWrapper(t_fonctionParam* f) {
    handleInputPlayer(((t_input*)(f)->param[0]), ((t_joueur*)(f)->param[1]), ((t_grid*)(f)->param[2]), ((t_viewPort*)(f)->param[3]), ((float*)(f)->param[4]), ((t_sceneController*)(f)->param[5]));
}

void setRenderTargetWrapper(t_fonctionParam* f) {
    setRenderTarget(((SDL_Renderer*)(f)->param[0]), ((t_viewPort*)(f)->param[1]));
}

void centerCameraOnWrapper(t_fonctionParam* f) {
    centerCameraOn(((t_camera*)(f)->param[0]), ((int*)(f)->param[1]), ((int*)(f)->param[2]), ((float*)(f)->param[3]));
}

void resizeViewportWrapper(t_fonctionParam* f) {
    resizeViewport(((t_viewPort*)(f)->param[0]), ((int*)(f)->param[1]), ((int*)(f)->param[2]));
}

void resizeMinimapWrapper(t_fonctionParam* f) {
    resizeMinimap(((SDL_Renderer*)(f)->param[0]), ((t_minimap*)(f)->param[1]), ((int*)(f)->param[2]), ((int*)(f)->param[3]));
}

void renderBarreVolummeWrapper(t_fonctionParam* f) {
    renderBarreVolumme(((SDL_Renderer*)(f)->param[0]), ((t_barreVolumme*)(f)->param[1]));
}

void handleInputButtonVolummeWrapper(t_fonctionParam* f) {
    handleInputButtonVolumme(((t_input*)(f)->param[0]), ((t_barreVolumme*)(f)->param[1]));
}

void updatePlayerWrapper(t_fonctionParam* f) {
    updatePlayer(((t_joueur*)(f)->param[0]), ((float*)(f)->param[1]), ((t_grid*)(f)->param[2]), ((t_objectManager*)(f)->param[3]));
}

void updateEnemyWrapper(t_fonctionParam* f) {
    updateEnemy(((t_enemy*)(f)->param[0]), ((float*)(f)->param[1]), ((t_grid*)(f)->param[2]), ((t_objectManager*)(f)->param[3]));
}

void setSceneWrapper(t_fonctionParam* f) {
    setScene(((t_sceneController*)(f)->param[0]), ((char*)(f)->param[1]));
}

void renderToucheWrapper(t_fonctionParam* f) {
    renderTouche(((SDL_Renderer*)f->param[0]), (t_touche*)f->param[1]);
}

void handleInputToucheWrapper(t_fonctionParam* f) {
    handleInputTouche((t_input*)f->param[0], (t_touche*)f->param[1], (SDL_Renderer*)f->param[2]);
}

void affichageWrapper(t_fonctionParam* fonction) {
    affichage((SDL_Renderer*)fonction->param[0], (t_mapAffichage*)fonction->param[1]);
}

SDL_Rect
creerRect(float x_ratio, float y_ratio, float w_ratio, float h_ratio) {
    return (SDL_Rect){WINDOW_WIDTH * x_ratio, WINDOW_HEIGHT * y_ratio, WINDOW_WIDTH * w_ratio, WINDOW_HEIGHT * h_ratio};
}

void bouttonClickQuit(t_fonctionParam* fonction) {
    t_input* input = GET_PTR(fonction, 0, t_input*);
    input->quit = SDL_TRUE;
}
