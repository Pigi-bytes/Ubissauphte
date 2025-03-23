#include "mainWorld.h"

t_scene* createMainWord(t_context* context) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t GRID_TYPE = registerType(registre, freeGrid, "Grid");
    const uint8_t PLAYER_TYPE = registerType(registre, freePlayer, "player");
    const uint8_t FRAME_DISPLAY_TYPE = registerType(registre, freeFPSDisplay, "frameData");
    const uint8_t CAMERA_TYPE = registerType(registre, freeCamera, "camera");
    const uint8_t VIEWPORT_TYPE = registerType(registre, freeViewport, "viewport");
    const uint8_t MINIMAP_TYPE = registerType(registre, freeMinimap, "minimap");
    const uint8_t ENEMY_TYPE = registerType(registre, freeEnemy, "enemy");
    const uint8_t TILE_ENTITY = registerType(registre, freeTileEntity, "ENTITY");

    t_scene* scene = createScene(initObjectManager(registre), "main");

    t_tileset* tileset = initTileset(context->renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* playerTileSet = initTileset(context->renderer, 32, 32, 16, "assets/imgs/chevaliervisiereouverteidle12run34.bmp");
    // t_tileset* fantomTileSet = initTileset(context->renderer, 48, 16, 16, "assets/imgs/fantomeidle23un1232.bmp");
    t_tileset* slimeTileSet = initTileset(context->renderer, 48, 16, 16, "assets/imgs/slimeidle12run1213.bmp");
    // t_tileset* crabeTileSet = initTileset(context->renderer, 80, 16, 16, "assets/imgs/crabeidle123232run416.bmp");

    SDL_Rect* rectcord = malloc(sizeof(SDL_Rect) * 10);
    for (int i = 0; i < 10; i++) {
        rectcord[i].x = 1;
        rectcord[i].y = 1;
        rectcord[i].w = 1;
        rectcord[i].h = 1;
    }
    t_salle** salle = genMap(10, rectcord);
    // addScene(context->sceneController, createMapWord(context->renderer, salle, rectcord));

    t_grille* grille[10];
    for (int i = 0; i < 10; i++) {
        grille[i] = geneRoom(salle[i]);
    }

    t_grid* level = loadMap(grille[0]->nom, tileset);
    for (int i = 0; i < 10; i++) {
        freeGrille(grille[i]);
    }

    int levelWidth = level->width * tileset->tileSize;
    int levelHeight = level->height * tileset->tileSize;

    t_objectManager* entities = initObjectManager(createTypeRegistry());
    const uint8_t ENTITY = registerType(entities->registry, NULL, "ENTITY");
    registerType(entities->registry, NULL, "TILE_ENTITY");

    t_joueur* joueur = createPlayer(context->control, (SDL_Texture*)getObject(tileset->textureTiles, 98), (SDL_Rect){60, 60, 16, 16}, playerTileSet);

    joueur->weaponCount = 0;
    joueur->currentWeaponIndex = 0;

    addScene(context->sceneController, createMapWord(context->renderer, salle, rectcord, context->input, joueur, context->sceneController));

    // Création des armes avec statistiques équilibrées
    t_arme* dague = malloc(sizeof(t_arme));
    *dague = (t_arme){
        .mass = 1.5f,             // Très légère pour des coups rapides
        .damage = 12.0f,          // Dégâts modérés
        .range = 20.0f,           // Courte portée
        .angleAttack = M_PI / 4,  // Arc d'attaque étroit (45°)
        .attackDuration = 0.15f,  // Animation très rapide
        .attackCooldown = 0.35f   // Récupération très rapide
    };
    dague->texture = getObject(tileset->textureTiles, 104);
    dague->displayRect = (SDL_Rect){0, 0, 16, 16};

    t_arme* epee = malloc(sizeof(t_arme));
    *epee = (t_arme){
        .mass = 3.0f,             // Masse moyenne
        .damage = 20.0f,          // Dégâts moyens
        .range = 30.0f,           // Portée moyenne
        .angleAttack = M_PI / 2,  // Arc d'attaque de 90°
        .attackDuration = 0.28f,  // Animation moyenne
        .attackCooldown = 0.7f    // Récupération moyenne
    };
    epee->texture = getObject(tileset->textureTiles, 105);
    epee->displayRect = (SDL_Rect){0, 0, 16, 16};

    t_arme* hache = malloc(sizeof(t_arme));
    *hache = (t_arme){
        .mass = 15.0f,            // Lourde
        .damage = 30.0f,          // Dégâts élevés
        .range = 28.0f,           // Portée moyenne
        .angleAttack = 2 * M_PI,  // Large arc d'attaque (120°)
        .attackDuration = 0.45f,  // Animation lente
        .attackCooldown = 1.2f    // Longue récupération
    };
    hache->texture = getObject(tileset->textureTiles, 119);
    hache->displayRect = (SDL_Rect){0, 0, 16, 16};

    t_arme* lance = malloc(sizeof(t_arme));
    *lance = (t_arme){
        .mass = 5.0f,             // Masse moyenne-légère
        .damage = 18.0f,          // Dégâts moyens-faibles
        .range = 40.0f,           // Longue portée
        .angleAttack = M_PI / 5,  // Arc d'attaque très étroit (36°)
        .attackDuration = 0.3f,   // Animation moyenne
        .attackCooldown = 0.65f   // Récupération moyenne
    };
    lance->texture = getObject(tileset->textureTiles, 106);
    lance->displayRect = (SDL_Rect){0, 0, 16, 16};

    t_arme* marteau = malloc(sizeof(t_arme));
    *marteau = (t_arme){
        .mass = 8.0f,               // Très lourd
        .damage = 40.0f,            // Dégâts très élevés
        .range = 40.0f,             // Portée moyenne-courte
        .angleAttack = M_PI / 1.5,  // Arc d'attaque de 90°
        .attackDuration = 0.3f,     // Animation très lente
        .attackCooldown = 1.5f      // Récupération très lente
    };
    marteau->texture = getObject(tileset->textureTiles, 132);
    marteau->displayRect = (SDL_Rect){0, 0, 16, 16};

    // Ajouter les armes à l'inventaire du joueur
    joueur->weapons[0] = epee;     // Première arme (indice 0)
    joueur->weapons[1] = dague;    // Deuxième arme (indice 1)
    joueur->weapons[2] = hache;    // Troisième arme (indice 2)
    joueur->weapons[3] = lance;    // Quatrième arme (indice 3)
    joueur->weapons[4] = marteau;  // Cinquième arme (indice 4)

    joueur->weaponCount = 5;  // Nombre total d'armes

    // Définir l'arme actuelle
    joueur->currentWeaponIndex = 0;
    joueur->currentWeapon = joueur->weapons[joueur->currentWeaponIndex];

    joueur->indexCurrentRoom = 0;

    addObject(entities, &joueur->entity, ENTITY);
    placeOnRandomTile(level, &joueur->entity, entities);

    t_enemy* enemy;
    for (int i = 0; i < 20; i++) {
        enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 16, 16}, slimeTileSet, scene);
        addObject(entities, &enemy->entity, ENTITY);
        placeOnRandomTile(level, &enemy->entity, entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

    for (int i = 0; i < 20; i++) {
        enemy = createSlime((SDL_Texture*)getObject(tileset->textureTiles, 109), (SDL_Rect){100, 100, 32, 32}, slimeTileSet, scene);
        enemy->entity.physics.mass = 10;
        enemy->maxHealth = 300;
        enemy->health = 300;

        addObject(entities, &enemy->entity, ENTITY);
        placeOnRandomTile(level, &enemy->entity, entities);
        ADD_OBJECT_TO_SCENE(scene, enemy, ENEMY_TYPE);
    }

    t_camera* camera = createCamera(levelWidth, levelHeight, 300, 300);
    t_viewPort* viewport = createViewport(context->renderer, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    t_minimap* minimap = createMinimap(context->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    ADD_OBJECT_TO_SCENE(scene, joueur, PLAYER_TYPE);
    ADD_OBJECT_TO_SCENE(scene, level, GRID_TYPE);
    ADD_OBJECT_TO_SCENE(scene, camera, CAMERA_TYPE);
    ADD_OBJECT_TO_SCENE(scene, viewport, VIEWPORT_TYPE);
    ADD_OBJECT_TO_SCENE(scene, minimap, MINIMAP_TYPE);

    processSpecialTiles(level, tileset, entities, TILE_ENTITY, scene);

    sceneRegisterFunction(scene, PLAYER_TYPE, HANDLE_INPUT, handleInputPlayerWrapper, 1, FONCTION_PARAMS(context->input, level, viewport, &context->frameData->deltaTime, context->sceneController));
    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_INPUT, cameraHandleZoomWrapper, 0, FONCTION_PARAMS(&context->input->mouseYWheel));

    sceneRegisterFunction(scene, PLAYER_TYPE, UPDATE, updatePlayerWrapper, 0, FONCTION_PARAMS(&context->frameData->deltaTime, level, entities));
    sceneRegisterFunction(scene, ENEMY_TYPE, UPDATE, updateEnemyWrapper, 0, FONCTION_PARAMS(&context->frameData->deltaTime, level, entities));
    sceneRegisterFunction(scene, TILE_ENTITY, UPDATE, updateTileEntityWrapper, 1, FONCTION_PARAMS(&context->frameData->deltaTime, level, entities, context->input));

    sceneRegisterFunction(scene, FRAME_DISPLAY_TYPE, RENDER_UI, renderFPSDisplayWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, MINIMAP_TYPE, UPDATE, updateMinimapWrapper, 0, FONCTION_PARAMS(camera, context->renderer));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.displayRect.x, &joueur->entity.displayRect.y, &context->frameData->deltaTime));
    sceneRegisterFunction(scene, CAMERA_TYPE, UPDATE, centerCameraOnWrapper, 0, FONCTION_PARAMS(&joueur->entity.displayRect.x, &joueur->entity.displayRect.y));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, HANDLE_RESIZE, resizeViewportWrapper, 0, FONCTION_PARAMS(&context->input->windowWidth, &context->input->windowHeight));
    sceneRegisterFunction(scene, MINIMAP_TYPE, HANDLE_RESIZE, resizeMinimapWrapper, 1, FONCTION_PARAMS(context->renderer, &context->input->windowWidth, &context->input->windowHeight));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, SET_BUFFER, setRenderTargetWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, GRID_TYPE, RENDER_GAME, renderGridWrapper, 1, FONCTION_PARAMS(context->renderer, camera));
    sceneRegisterFunction(scene, TILE_ENTITY, RENDER_GAME, renderTileEntityWrapper, 1, FONCTION_PARAMS(context->renderer, camera));
    sceneRegisterFunction(scene, PLAYER_TYPE, RENDER_GAME, renderPlayerWrapper, 1, FONCTION_PARAMS(context->renderer, camera));
    sceneRegisterFunction(scene, ENEMY_TYPE, RENDER_GAME, renderEnemyWrapper, 1, FONCTION_PARAMS(context->renderer, camera));

    sceneRegisterFunction(scene, VIEWPORT_TYPE, RENDER_BUFFER, renderViewportWrapper, 1, FONCTION_PARAMS(context->renderer));

    sceneRegisterFunction(scene, MINIMAP_TYPE, RENDER_UI, renderMinimapWrapper, 1, FONCTION_PARAMS(context->renderer));

    return scene;
}

t_scene* createMapWord(SDL_Renderer* renderer, t_salle** salle, SDL_Rect* rectcord, t_input* input, t_joueur* player, t_sceneController* scenecontroler) {
    t_typeRegistry* registre = createTypeRegistry();
    const uint8_t MAP_TYPE = registerType(registre, freeMapAffiche, "mapView");

    t_scene* scene = createScene(initObjectManager(registre), "carte");

    t_mapAffichage* map = malloc(sizeof(t_mapAffichage));

    generateMap(rectcord, salle, 10, map, WINDOW_WIDTH, WINDOW_HEIGHT);

    ADD_OBJECT_TO_SCENE(scene, map, MAP_TYPE);

    sceneRegisterFunction(scene, MAP_TYPE, RENDER_UI, affichageWrapper, 1, FONCTION_PARAMS(renderer, player));
    sceneRegisterFunction(scene, MAP_TYPE, HANDLE_INPUT, handleInputMapWrapper, -1, FONCTION_PARAMS(input, player, scenecontroler));

    return scene;
}