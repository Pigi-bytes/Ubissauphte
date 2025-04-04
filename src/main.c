#include "_scene/commandeMenu.h"
#include "_scene/fpsMenu.h"
#include "_scene/gameOver.h"
#include "_scene/mainMenu.h"
#include "_scene/mainWorld.h"
#include "_scene/option2.h"
#include "_scene/optionMenu.h"
#include "_scene/sceneInventaire.h"
#include "context.h"

#ifdef _WIN32
#include <direct.h>  // Windows : _mkdir()
#define CREATE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>  // Linux/macOS : mkdir()
#include <sys/types.h>
#define CREATE_DIR(path) mkdir(path, 0777)
#endif

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
        CREATE_DIR("./assets/map/");
        perror("création du répertoire map");
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

    context.frameData = initFrameData(0);
    context.option = creeOption();
    context.sceneController = initSceneController();
    context.audioManager = initAudioManager();
    context.fpsDisplay = initFPSDisplay(context.renderer, context.font);
    context.currentLevel = NULL;
    context.nbLevel = 3;
    context.difficulty = EASY;

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
    context.control->activable1 = SDL_SCANCODE_1;
    t_tileset* tileset = initTileset(context.renderer, 192, 240, 16, "assets/imgs/tileMapDungeon.bmp");
    t_tileset* playerTileSet = initTileset(context.renderer, 32, 32, 16, "assets/imgs/chevaliervisiereouverteidle12run34.bmp");

    t_joueur* player = createPlayer(context.control, (SDL_Texture*)getObject(tileset->textureTiles, 98), (SDL_Rect){60, 60, 16, 16}, playerTileSet);

    t_item* epee = malloc(sizeof(t_item));
    epee->arme = malloc(sizeof(t_arme));
    strcpy(epee->name, "HOMARD");
    epee->arme->mass = 3.0f;             // Masse moyenne
    epee->arme->damage = 20.0f;          // Dégâts moyens
    epee->arme->range = 30.0f;           // Portée moyenne
    epee->arme->angleAttack = M_PI / 2;  // Arc d'attaque de 90°
    epee->arme->attackDuration = 0.28f;  // Animation moyenne
    epee->arme->attackCooldown = 0.7f;   // Récupération moyenne
    strcpy(epee->description, "\nazdezdef\nsssqs");
    epee->stats.attack.additive = 5;
    epee->stats.attack.multiplicative = 1;
    epee->stats.defense.additive = 581;
    epee->stats.defense.multiplicative = 1;
    epee->stats.healthMax.additive = 8162;
    epee->stats.healthMax.multiplicative = 1;
    epee->stats.speed.additive = 645;
    epee->stats.speed.multiplicative = 1;

    epee->arme->texture = getObject(tileset->textureTiles, 111);
    epee->texture = getObject(tileset->textureTiles, 111);

    epee->arme->displayRect = (SDL_Rect){0, 0, 16, 16};
    epee->arme->displayRect = (SDL_Rect){0, 0, 16, 16};
    epee->validSlot[0] = SLOT_ARME;
    epee->flags = 6;
    epee->id = 9;
    epee->arme->indice = 10;
    epee->arme->onEquipe = creerFonction(peutEquiperWrapper, FONCTION_PARAMS(&player->currentWeapon, epee->arme));

    equipementRecalculerStats(&player);

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

        if (keyPressOnce(context.input, SDL_SCANCODE_U)) {
            printf("Ajout Objet \n");
            inventaireAjoutObjet(player->inventaire, epee, 1);
        }

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