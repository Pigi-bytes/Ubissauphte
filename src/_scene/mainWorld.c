#include "mainWorld.h"

void generateSalle(t_salle** salle, int numSalle, t_context* context, t_joueur** joueur) {
    for (int i = 0; i < numSalle; i++) {
        if (salle[i]->ID >= -4 && salle[i]->ID <= -1)
            salle[i]->scene = createMarchantMap(context, salle[i], joueur, &(salle[i]->grille));
        else if (salle[i]->ID == -5) {
            salle[i]->scene = createBossMap(context, salle[i], joueur, &(salle[i]->grille));
        } else {
            salle[i]->scene = createMainWord(context, salle[i], joueur, &(salle[i]->grille));
        }
        addScene(context->sceneController, salle[i]->scene);
    }

    placeOnRandomTile(salle[0]->grille, &(*joueur)->entity, salle[0]->entities);
}

void CreateNiveau(t_context* context, int nbSalle, t_joueur** joueur) {
    SDL_Rect* rectcord = malloc(sizeof(SDL_Rect) * nbSalle);
    for (int i = 0; i < nbSalle; i++) {
        rectcord[i].x = 1;
        rectcord[i].y = 1;
        rectcord[i].w = 1;
        rectcord[i].h = 1;
    }
    t_salle** salle = genMap(nbSalle, rectcord);
    generateSalle(salle, nbSalle, context, joueur);
    addScene(context->sceneController, createMapWord(context, salle, rectcord, *joueur, nbSalle));
}

t_scene* createMainWord(t_context* context, t_salle* salle, t_joueur** player, t_grid** level) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t GRID_TYPE = registerType(registre, freeGrid, "Grid");
    const uint8_t PLAYER_TYPE = registerType(registre, freePlayer, "player");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t CAMERA_TYPE = registerType(registre, freeCamera, "camera");
    const uint8_t VIEWPORT_TYPE = registerType(registre, freeViewport, "viewport");
    const uint8_t MINIMAP_TYPE = registerType(registre, freeMinimap, "minimap");
    const uint8_t ENEMY_TYPE = registerType(registre, freeEnemy, "enemy");
    const uint8_t TILE_ENTITY = registerType(registre, freeTileEntity, "tile_entity");
    const uint8_t HUD_TYPE = registerType(registre, freeHUD, "hud");

    t_scene* scene = createScene(initObjectManager(registre), "main");
    t_tileset* tileset = initTileset(context->renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* playerTileSet = initTileset(context->renderer, 32, 32, 16, "assets/imgs/chevaliervisiereouverteidle12run34.bmp");
    // t_tileset* fantomTileSet = initTileset(context->renderer, 48, 16, 16, "assets/imgs/fantomeidle23un1232.bmp");
    t_tileset* slimeTileSet = initTileset(context->renderer, 48, 16, 16, "assets/imgs/slimeidle12run1213.bmp");
    // t_tileset* crabeTileSet = initTileset(context->renderer, 80, 16, 16, "assets/imgs/crabeidle123232run416.bmp");

    // addScene(context->sceneController, createMapWord(context->renderer, salle, rectcord));

    salle->entities = initObjectManager(createTypeRegistry());
    const uint8_t PLAYER = registerType(salle->entities->registry, NULL, "PLAYER");
    const uint8_t ENEMY = registerType(salle->entities->registry, NULL, "ENEMY");
    registerType(salle->entities->registry, NULL, "CHEST");
    registerType(salle->entities->registry, NULL, "SPIKE");
    registerType(salle->entities->registry, NULL, "BARREL");

    t_grille* grille = geneRoom(salle);
    (*level) = loadMap(grille->nom, tileset);
    int levelWidth = (*level)->width * tileset->tileSize;
    int levelHeight = (*level)->height * tileset->tileSize;
    free(grille);

    addObject(salle->entities, &(*player)->entity, PLAYER);

    t_enemy* enemy;
    for (int i = 0; i < 0; i++) {
        enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 16, 16}, slimeTileSet, scene);
        addObject(salle->entities, &enemy->entity, ENEMY);
        placeOnRandomTile(*level, &enemy->entity, salle->entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

    for (int i = 0; i < 0; i++) {
        enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 32, 32}, slimeTileSet, scene);
        enemy->entity.physics.mass = 10;
        enemy->health.currentHealth = 300;
        enemy->health.maxHealth = 300;

        addObject(salle->entities, &enemy->entity, ENEMY);
        placeOnRandomTile(*level, &enemy->entity, salle->entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

    t_camera* camera = createCamera(levelWidth, levelHeight, 300, 300);
    t_viewPort* viewport = createViewport(context->renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(context->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_hud* playerHUD = createHUD(context->renderer, loadFont("assets/fonts/PressStart2P-vaV7.ttf", 16), tileset);

    ADD_OBJECT_TO_SCENE(scene, playerHUD, HUD_TYPE);

    ADD_OBJECT_TO_SCENE(scene, NULL, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, *level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    processSpecialTiles(*level, tileset, salle->entities, TILE_ENTITY, scene);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, -1, FONCTION_PARAMS(context->input, *player, *level, viewport, &context->frameData->deltaTime, context->sceneController));
    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_INPUT, cameraHandleZoomWrapper, 0, FONCTION_PARAMS(&context->input->mouseYWheel));

    sceneRegisterFunction(scene, PLAYER_TYPE, UPDATE, updatePlayerWrapper, -1, FONCTION_PARAMS((*player), &context->frameData->deltaTime, salle, salle->entities));
    sceneRegisterFunction(scene, ENEMY_TYPE, UPDATE, updateEnemyWrapper, 0, FONCTION_PARAMS(&context->frameData->deltaTime, salle, salle->entities));
    sceneRegisterFunction(scene, TILE_ENTITY, UPDATE, updateTileEntityWrapper, 1, FONCTION_PARAMS(context, salle, salle->entities));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));

    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, context->renderer, salle->entities, *level));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&(*player)->entity.displayRect.x, &(*player)->entity.displayRect.y, &context->frameData->deltaTime));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&(*player)->entity.displayRect.x, &(*player)->entity.displayRect.y));
    sceneRegisterFunction(scene, HUD_TYPE, UPDATE, updateHUDWrapper, 0, FONCTION_PARAMS(context->renderer, *player));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&context->input->windowWidth, &context->input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(context->renderer, &context->input->windowWidth, &context->input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(context->renderer, camera));
    sceneRegisterFunction(scene, TILE_ENTITY, RENDER_GAME, renderTileEntityWrapper, 1, FONCTION_PARAMS(context, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, -1, FONCTION_PARAMS(context->renderer, (*player), camera));
    sceneRegisterFunction(scene, ENEMY_TYPE, RENDER_GAME, renderEnemyWrapper, 1, FONCTION_PARAMS(context->renderer, camera));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, HUD_TYPE, RENDER_UI, renderHUDWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(context->renderer));

    return scene;
}

t_scene* createMarchantMap(t_context* context, t_salle* salle, t_joueur** player, t_grid** level) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t GRID_TYPE = registerType(registre, freeGrid, "Grid");
    const uint8_t PLAYER_TYPE = registerType(registre, freePlayer, "player");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t CAMERA_TYPE = registerType(registre, freeCamera, "camera");
    const uint8_t VIEWPORT_TYPE = registerType(registre, freeViewport, "viewport");
    const uint8_t MINIMAP_TYPE = registerType(registre, freeMinimap, "minimap");
    const uint8_t TILE_ENTITY = registerType(registre, freeTileEntity, "tile_entity");
    const uint8_t HUD_TYPE = registerType(registre, freeHUD, "hud");

    t_scene* scene = createScene(initObjectManager(registre), "main");
    t_tileset* tileset = initTileset(context->renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");

    char* chemin;

    switch (salle->ID) {
        case -1:
            chemin = "assets/mapSpecial/mapMarchant/1marchant30x30.txt";
            break;
        case -2:
            chemin = "assets/mapSpecial/mapMarchant/2marchant35x35.txt";
            break;
        case -3:
            chemin = "assets/mapSpecial/mapMarchant/3marchant20x20.txt";
            break;
        case -4:
            chemin = "assets/mapSpecial/mapMarchant/4marchant20x20.txt";
    }

    (*level) = loadMap(chemin, tileset);
    int levelWidth = (*level)->width * tileset->tileSize;
    int levelHeight = (*level)->height * tileset->tileSize;

    salle->entities = initObjectManager(createTypeRegistry());
    const uint8_t PLAYER = registerType(salle->entities->registry, NULL, "PLAYER");
    registerType(salle->entities->registry, NULL, "BARREL");
    registerType(salle->entities->registry, NULL, "SPIKE");
    registerType(salle->entities->registry, NULL, "BLACKSMITH");
    registerType(salle->entities->registry, NULL, "MERCHANT");
    registerType(salle->entities->registry, NULL, "WIZARD");

    addObject(salle->entities, &(*player)->entity, PLAYER);

    t_camera* camera = createCamera(levelWidth, levelHeight, 300, 300);
    t_viewPort* viewport = createViewport(context->renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(context->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_hud* playerHUD = createHUD(context->renderer, loadFont("assets/fonts/PressStart2P-vaV7.ttf", 16), tileset);

    ADD_OBJECT_TO_SCENE(scene, playerHUD, HUD_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, *level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    processSpecialTiles(*level, tileset, salle->entities, TILE_ENTITY, scene);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, -1, FONCTION_PARAMS(context->input, *player, *level, viewport, &context->frameData->deltaTime, context->sceneController));
    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_INPUT, cameraHandleZoomWrapper, 0, FONCTION_PARAMS(&context->input->mouseYWheel));

    sceneRegisterFunction(scene, PLAYER_TYPE, UPDATE, updatePlayerWrapper, -1, FONCTION_PARAMS((*player), &context->frameData->deltaTime, salle, salle->entities));
    sceneRegisterFunction(scene, TILE_ENTITY, UPDATE, updateTileEntityWrapper, 1, FONCTION_PARAMS(context, salle, salle->entities));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));

    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, context->renderer, salle->entities, *level));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&(*player)->entity.displayRect.x, &(*player)->entity.displayRect.y, &context->frameData->deltaTime));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&(*player)->entity.displayRect.x, &(*player)->entity.displayRect.y));
    sceneRegisterFunction(scene, HUD_TYPE, UPDATE, updateHUDWrapper, 0, FONCTION_PARAMS(context->renderer, *player));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&context->input->windowWidth, &context->input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(context->renderer, &context->input->windowWidth, &context->input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(context->renderer, camera));
    sceneRegisterFunction(scene, TILE_ENTITY, RENDER_GAME, renderTileEntityWrapper, 1, FONCTION_PARAMS(context, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, -1, FONCTION_PARAMS(context->renderer, (*player), camera));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, HUD_TYPE, RENDER_UI, renderHUDWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(context->renderer));

    return scene;
}

t_scene* createBossMap(t_context* context, t_salle* salle, t_joueur** player, t_grid** level) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t GRID_TYPE = registerType(registre, freeGrid, "Grid");
    const uint8_t PLAYER_TYPE = registerType(registre, freePlayer, "player");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t CAMERA_TYPE = registerType(registre, freeCamera, "camera");
    const uint8_t VIEWPORT_TYPE = registerType(registre, freeViewport, "viewport");
    const uint8_t MINIMAP_TYPE = registerType(registre, freeMinimap, "minimap");
    const uint8_t TILE_ENTITY = registerType(registre, freeTileEntity, "tile_entity");
    const uint8_t HUD_TYPE = registerType(registre, freeHUD, "hud");
    const uint8_t ENEMY_TYPE = registerType(registre, freeEnemy, "enemy");

    t_scene* scene = createScene(initObjectManager(registre), "main");
    t_tileset* tileset = initTileset(context->renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* slimeTileSet = initTileset(context->renderer, 48, 16, 16, "assets/imgs/slimeidle12run1213.bmp");

    (*level) = loadMap("assets/mapSpecial/mapBoss/1boss80x80.txt", tileset);
    int levelWidth = (*level)->width * tileset->tileSize;
    int levelHeight = (*level)->height * tileset->tileSize;

    salle->entities = initObjectManager(createTypeRegistry());
    const uint8_t PLAYER = registerType(salle->entities->registry, NULL, "PLAYER");
    const uint8_t ENEMY = registerType(salle->entities->registry, NULL, "ENEMY");
    registerType(salle->entities->registry, NULL, "BARREL");
    registerType(salle->entities->registry, NULL, "SPIKE");
    registerType(salle->entities->registry, NULL, "BLACKSMITH");
    registerType(salle->entities->registry, NULL, "MERCHANT");
    registerType(salle->entities->registry, NULL, "WIZARD");

    addObject(salle->entities, &(*player)->entity, PLAYER);

    t_camera* camera = createCamera(levelWidth, levelHeight, 300, 300);
    t_viewPort* viewport = createViewport(context->renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(context->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_hud* playerHUD = createHUD(context->renderer, loadFont("assets/fonts/PressStart2P-vaV7.ttf", 16), tileset);

    t_enemy* enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 128, 128}, slimeTileSet, scene);
    enemy->entity.physics.mass = 200;
    enemy->health.currentHealth = 100000;
    enemy->health.maxHealth = 100000;

    addObject(salle->entities, &enemy->entity, ENEMY);
    placeOnRandomTile(*level, &enemy->entity, salle->entities);
    ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);

    ADD_OBJECT_TO_SCENE(scene, playerHUD, HUD_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, *level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);
    ADD_OBJECT_TO_SCENE(scene, NULL, FRAME_DISPLAY_TYPE);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, -1, FONCTION_PARAMS(context->input, *player, *level, viewport, &context->frameData->deltaTime, context->sceneController));
    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_INPUT, cameraHandleZoomWrapper, 0, FONCTION_PARAMS(&context->input->mouseYWheel));

    sceneRegisterFunction(scene, PLAYER_TYPE, UPDATE, updatePlayerWrapper, -1, FONCTION_PARAMS((*player), &context->frameData->deltaTime, salle, salle->entities));
    sceneRegisterFunction(scene, ENEMY_TYPE, UPDATE, updateEnemyWrapper, 0, FONCTION_PARAMS(&context->frameData->deltaTime, salle, salle->entities));
    sceneRegisterFunction(scene, TILE_ENTITY, UPDATE, updateTileEntityWrapper, 1, FONCTION_PARAMS(context, salle, salle->entities));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, UPDATE, updateFPSDisplayWrapper, -1, FONCTION_PARAMS(context->fpsDisplay, context->frameData, context->renderer));
    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, -1, FONCTION_PARAMS(context->renderer, context->fpsDisplay));

    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, context->renderer, salle->entities, *level));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&(*player)->entity.displayRect.x, &(*player)->entity.displayRect.y, &context->frameData->deltaTime));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&(*player)->entity.displayRect.x, &(*player)->entity.displayRect.y));
    sceneRegisterFunction(scene, HUD_TYPE, UPDATE, updateHUDWrapper, 0, FONCTION_PARAMS(context->renderer, *player));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&context->input->windowWidth, &context->input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(context->renderer, &context->input->windowWidth, &context->input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(context->renderer, camera));
    sceneRegisterFunction(scene, TILE_ENTITY, RENDER_GAME, renderTileEntityWrapper, 1, FONCTION_PARAMS(context, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, -1, FONCTION_PARAMS(context->renderer, (*player), camera));
    sceneRegisterFunction(scene, ENEMY_TYPE, RENDER_GAME, renderEnemyWrapper, 1, FONCTION_PARAMS(context->renderer, camera));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, HUD_TYPE, RENDER_UI, renderHUDWrapper, 1, FONCTION_PARAMS(context->renderer));
    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(context->renderer));

    return scene;
}

t_scene* createMapWord(t_context* context, t_salle** salle, SDL_Rect* rectcord, t_joueur* player, int nbSalle) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t MAP_TYPE = registerType(registre, freeMapAffiche, "mapView");

    t_scene* scene = createScene(initObjectManager(registre), "carte");

    t_mapAffichage* map = malloc(sizeof(t_mapAffichage));

    generateMap(rectcord, salle, nbSalle, map, WINDOW_WIDTH, WINDOW_HEIGHT);

    ADD_OBJECT_TO_SCENE(scene, map, MAP_TYPE);

    sceneRegisterFunction(scene, MAP_TYPE, RENDER_UI, affichageWrapper, 1, FONCTION_PARAMS(context->renderer, player));
    sceneRegisterFunction(scene, MAP_TYPE, HANDLE_INPUT, handleInputMapWrapper, -1, FONCTION_PARAMS(context->input, player, context->sceneController));

    return scene;
}