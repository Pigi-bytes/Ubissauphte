#include "mainWorld.h"

t_scene* createMainWord(SDL_Renderer* renderer, t_input* input, TTF_Font* font, t_frameData* frameData, t_control* contr) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t GRID_TYPE = registerType(registre, freeGrid, "Grid");
    const uint8_t PLAYER_TYPE = registerType(registre, freePlayer, "player");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t CAMERA_TYPE = registerType(registre, freeCamera, "camera");
    const uint8_t VIEWPORT_TYPE = registerType(registre, freeViewport, "viewport");
    const uint8_t MINIMAP_TYPE = registerType(registre, freeMinimap, "minimap");
    const uint8_t ENEMY_TYPE = registerType(registre, freeEnemy, "enemy");
    t_scene* scene = createScene(initObjectManager(registre), "main");

    t_tileset* tileset = initTileset(renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* playerTileSet = initTileset(renderer, 32, 32, 16, "assets/imgs/chevaliervisiereouverteidle12run34.bmp");
    t_tileset* fantomTileSet = initTileset(renderer, 48, 16, 16, "assets/imgs/fantomeidle23un1232.bmp");
    t_tileset* slimeTileSet = initTileset(renderer, 48, 16, 16, "assets/imgs/slimeidle12run1213.bmp");
    t_tileset* crabeTileSet = initTileset(renderer, 80, 16, 16, "assets/imgs/crabeidle123232run416.bmp");

    SDL_Rect* rectcord = malloc(sizeof(SDL_Rect) * 150);
    for (int i = 0; i < 150; i++) {
        rectcord[i].x = 1;
        rectcord[i].y = 1;
        rectcord[i].w = 1;
    }
    t_salle** salle = genMap(150, rectcord);
    printf("%p\n", salle[1]->droite);
    t_grille* grille = geneRoom(salle[1]);

    t_grid* level = loadMap(grille->nom, tileset);
    freeGrille(grille);

    int levelWidth = level->width * tileset->tileSize;
    int levelHeight = level->height * tileset->tileSize;

    t_objectManager* entities = initObjectManager(createTypeRegistry());
    const uint8_t ENTITY = registerType(entities->registry, NULL, "ENTITY");

    t_joueur* joueur = createPlayer(contr, (SDL_Texture*)getObject(tileset->textureTiles, 98), (SDL_Rect){60, 60, 16, 16}, playerTileSet);
    t_arme* hache = malloc(sizeof(t_arme));
    t_arme* epee = malloc(sizeof(t_arme));
    t_arme* dague = malloc(sizeof(t_arme));

    *hache = (t_arme){.mass = 8.0f, .damage = 30.0f, .range = 15.0f, .angleAttack = M_PI * 2, .attackDuration = 1.0f, .attackCooldown = 2.0f};
    hache->texture = getObject(tileset->textureTiles, 119);
    hache->displayRect = (SDL_Rect){0, 0, 16, 16};
    *epee = (t_arme){.mass = 3.0f, .damage = 20.0f, .range = 35.0f, .angleAttack = M_PI / 2, .attackDuration = 0.3f, .attackCooldown = 1.0f};
    epee->texture = getObject(tileset->textureTiles, 105);
    epee->displayRect = (SDL_Rect){0, 0, 16, 16};
    *dague = (t_arme){.mass = 2.0f, .damage = 15.0f, .range = 20.0f, .angleAttack = M_PI / 3, .attackDuration = 0.2f, .attackCooldown = 0.4f};
    dague->texture = getObject(tileset->textureTiles, 104);
    dague->displayRect = (SDL_Rect){0, 0, 16, 16};

    joueur->currentWeapon = epee;

    addObject(entities, &joueur->entity, ENTITY);
    placeOnRandomTile(level, &joueur->entity, entities);

    t_enemy* enemy;
    for (int i = 0; i < 1; i++) {
        enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 16, 16}, slimeTileSet, scene);
        addObject(entities, &enemy->entity, ENTITY);
        placeOnRandomTile(level, &enemy->entity, entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

    for (int i = 0; i < 0; i++) {
        enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 32, 32}, slimeTileSet, scene);
        enemy->entity.physics.mass = 10;
        enemy->maxHealth = 300;
        enemy->health = 300;

        addObject(entities, &enemy->entity, ENTITY);
        placeOnRandomTile(level, &enemy->entity, entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

    t_camera* camera = createCamera(levelWidth, levelHeight, 300, 300);
    t_viewPort* viewport = createViewport(renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    ADD_OBJECT_TO_SCENE(scene, joueur, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, 1, FONCTION_PARAMS(input, level, viewport, &frameData->deltaTime));
    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_INPUT, cameraHandleZoomWrapper, 0, FONCTION_PARAMS(&input->mouseYWheel));

    sceneRegisterFunction(scene, PLAYER_TYPE, UPDATE, updatePlayerWrapper, 0, FONCTION_PARAMS(&frameData->deltaTime, level, entities));
    sceneRegisterFunction(scene, ENEMY_TYPE, UPDATE, updateEnemyWrapper, 0, FONCTION_PARAMS(&frameData->deltaTime, level, entities));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, renderer));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.displayRect.x, &joueur->entity.displayRect.y, &frameData->deltaTime));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.displayRect.x, &joueur->entity.displayRect.y));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&input->windowWidth, &input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(renderer, &input->windowWidth, &input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(renderer, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, 1, FONCTION_PARAMS(renderer, camera));
    sceneRegisterFunction(scene, ENEMY_TYPE, RENDER_GAME, renderEnemyWrapper, 1, FONCTION_PARAMS(renderer, camera));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(renderer));

    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(renderer));

    return scene;
}