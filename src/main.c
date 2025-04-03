#include "_scene/commandeMenu.h"
#include "_scene/fpsMenu.h"
#include "_scene/mainMenu.h"
#include "_scene/mainWorld.h"
#include "_scene/option2.h"
#include "_scene/optionMenu.h"
#include "_scene/sceneInventaire.h"
#include "context.h"
int main(int argc, char* argv[]) {
    if (initAudioSystem() != 0) {
        return 1;  // Arrêter si l'initialisation échoue
    }

    SDL_Init(SDL_INIT_VIDEO);
    initTextEngine();
    srand(time(NULL));

    t_context context;

    context.window = SDL_CreateWindow("Lo Poti Donjon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED);
    Debug_Init(context.renderer);

    context.input = initInput(WINDOW_WIDTH, WINDOW_HEIGHT);
    context.font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 24);
    context.gameFont = loadFont("assets/fonts/PressStart2P-vaV7.ttf", 7);

    int nb = 3;

    context.frameData = initFrameData(0);
    context.option = creeOption();
    context.sceneController = initSceneController();
    context.audioManager = initAudioManager();
    context.fpsDisplay = initFPSDisplay(context.renderer, context.font);
    context.currentLevel = NULL;
    context.nbLevel = &nb;

    context.control = malloc(sizeof(t_control));
    context.control->up = SDL_SCANCODE_W;
    context.control->down = SDL_SCANCODE_S;
    context.control->left = SDL_SCANCODE_A;
    context.control->right = SDL_SCANCODE_D;
    context.control->dash = SDL_SCANCODE_SPACE;
    context.control->escape = SDL_SCANCODE_ESCAPE;
    context.control->map = SDL_SCANCODE_P;
    context.control->interact = SDL_SCANCODE_E;
    context.control->inventaire = SDL_SCANCODE_I;

    t_tileset* tileset = initTileset(context.renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* playerTileSet = initTileset(context.renderer, 32, 32, 16, "assets/imgs/chevaliervisiereouverteidle12run34.bmp");

    t_joueur* player = createPlayer(context.control, (SDL_Texture*)getObject(tileset->textureTiles, 98), (SDL_Rect){60, 60, 16, 16}, playerTileSet);

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

    t_arme* hache = malloc(sizeof(t_arme));
    *hache = (t_arme){
        .mass = 15.0f,            // Lourde
        .damage = 50000.0f,       // Dégâts élevés
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

    player->currentWeapon = dague;

    player->indexCurrentRoom = 0;

    t_scene* scene = createMainMenu(&context, &player);
    t_scene* scene0 = createOptionMenu(&context);
    t_scene* scene2 = createCommandeMenu(&context);
    t_scene* scene3 = createFpsMenu(&context);
    t_scene* scene4 = createMainInv(&context, player);
    t_scene* scene5 = createOption2Menu(&context);
    t_scene* scene6 = attente(&context);

    addScene(context.sceneController, scene);
    addScene(context.sceneController, scene0);
    addScene(context.sceneController, scene2);
    addScene(context.sceneController, scene3);
    addScene(context.sceneController, scene4);
    addScene(context.sceneController, scene5);
    addScene(context.sceneController, scene6);
    setScene(context.sceneController, "menuPrincipal");

    while (!context.input->quit) {
        startFrame(context.frameData);
        t_scene* currentScene = getCurrentScene(context.sceneController);

        executeSceneFunctions(currentScene, HANDLE_INPUT);
        updateInput(context.input);

        executeSceneFunctions(currentScene, UPDATE);

        updateFPS(context.frameData);

        if (context.input->resized) {
            executeSceneFunctions(currentScene, HANDLE_RESIZE);
        }

        SDL_RenderClear(context.renderer);
        executeSceneFunctions(currentScene, SET_BUFFER);
        executeSceneFunctions(currentScene, RENDER_GAME);

        Debug_RenderAll();
        executeSceneFunctions(currentScene, RENDER_BUFFER);
        executeSceneFunctions(currentScene, RENDER_UI);

        SDL_RenderPresent(context.renderer);

        capFrameRate(context.frameData);
    }

    freeOption(context.option);
    freeFrameData(context.frameData);
    freeSceneController(&context.sceneController);
    free(context.control);
    freeLevel(context.currentLevel);

    TTF_CloseFont(context.font);
    freeInput(context.input);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    return EXIT_SUCCESS;
}