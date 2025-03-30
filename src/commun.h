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

SDL_Rect creerRect(float x_ratio, float y_ratio, float w_ratio, float h_ratio);
void bouttonClickQuit(t_fonctionParam* fonction);
void renderTextWrapper(t_fonctionParam* f);
void changementAffichage(t_fonctionParam* fonction);
void renderButtonWrapper(t_fonctionParam* f);
void renderFPSDisplayWrapper(t_fonctionParam* f);
void renderGridWrapper(t_fonctionParam* f);
void renderMinimapWrapper(t_fonctionParam* f);
void renderViewportWrapper(t_fonctionParam* f);
void renderPlayerWrapper(t_fonctionParam* f);
void renderEnemyWrapper(t_fonctionParam* f);
void updateFPSDisplayWrapper(t_fonctionParam* f);
void updateMinimapWrapper(t_fonctionParam* f);
void cameraHandleZoomWrapper(t_fonctionParam* f);
void handleInputButtonWrapper(t_fonctionParam* f);
void handleInputPlayerWrapper(t_fonctionParam* f);
void setRenderTargetWrapper(t_fonctionParam* f);
void centerCameraOnWrapper(t_fonctionParam* f);
void resizeViewportWrapper(t_fonctionParam* f);
void resizeMinimapWrapper(t_fonctionParam* f);
void renderBarreVolummeWrapper(t_fonctionParam* f);
void handleInputButtonVolummeWrapper(t_fonctionParam* f);
void updatePlayerWrapper(t_fonctionParam* f);
void updateEnemyWrapper(t_fonctionParam* f);
void setSceneWrapper(t_fonctionParam* f);
void renderToucheWrapper(t_fonctionParam* f);
void handleInputToucheWrapper(t_fonctionParam* f);
void affichageWrapper(t_fonctionParam* f);
void handleInputMapWrapper(t_fonctionParam* f);
void inventoryUI_RenderWrapper(t_fonctionParam *f);
void inventoryUI_UpdateWrapper(t_fonctionParam *f);
void updateScrollWrapper(t_fonctionParam *f);
void updateWrapper(t_fonctionParam *f);
void getPrevuisSceneWrapper(t_fonctionParam* f);
void getPrevuisMenuWrapper(t_fonctionParam* f);

#endif