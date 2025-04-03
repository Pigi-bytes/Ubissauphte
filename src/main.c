#include "_scene/commandeMenu.h"
#include "_scene/fpsMenu.h"
#include "_scene/gameOver.h"
#include "_scene/mainMenu.h"
#include "_scene/mainWorld.h"
#include "_scene/option2.h"
#include "_scene/optionMenu.h"
#include "_scene/sceneInventaire.h"
#include "context.h"

void supprimerAllfichier() {
    DIR* d;
    struct dirent* dir;
    char path[1024];

    d = opendir("./assets/map/");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".txt") != NULL) {
                snprintf(path, sizeof(path), "./assets/map/%s", dir->d_name);

                if (remove(path) == 0) {
                    printf("Fichier supprimé: %s\n", path);
                } else {
                    perror("Erreur lors de la suppression du fichier");
                }
            }
        }
        closedir(d);
    } else {
        perror("Erreur lors de l'ouverture du répertoire");
    }
}

int main(int argc, char* argv[]) {
    supprimerAllfichier();
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

    player->currentWeapon = dague;

    player->indexCurrentRoom = 0;

    t_scene* scene = createMainMenu(&context, &player);
    t_scene* scene0 = createOptionMenu(&context);
    t_scene* scene2 = createCommandeMenu(&context);
    t_scene* scene3 = createFpsMenu(&context);
    t_scene* scene4 = createMainInv(&context, player);
    t_scene* scene5 = createOption2Menu(&context);
    t_scene* scene6 = attente(&context);
    t_scene* scene7 = CreateGameOver(&context);

    addScene(context.sceneController, scene);
    addScene(context.sceneController, scene0);
    addScene(context.sceneController, scene2);
    addScene(context.sceneController, scene3);
    addScene(context.sceneController, scene4);
    addScene(context.sceneController, scene5);
    addScene(context.sceneController, scene6);
    addScene(context.sceneController, scene7);
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