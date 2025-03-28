#include "_scene/commandeMenu.h"
#include "_scene/fpsMenu.h"
#include "_scene/mainMenu.h"
#include "_scene/mainWorld.h"
#include "_scene/optionMenu.h"
#include "_scene/sceneInventaire.h"
#include "context.h"

int main(int argc, char* argv[]) {
    if (initAudioSystem() != 0) {
        return 1;  // Arrêter si l'initialisation échoue
    }

    SDL_Init(SDL_INIT_VIDEO);
    initTextEngine();

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

    context.control = malloc(sizeof(t_control));
    context.control->up = SDL_SCANCODE_W;
    context.control->down = SDL_SCANCODE_S;
    context.control->left = SDL_SCANCODE_A;
    context.control->right = SDL_SCANCODE_D;
    context.control->dash = SDL_SCANCODE_SPACE;
    context.control->escape = SDL_SCANCODE_ESCAPE;
    context.control->map = SDL_SCANCODE_P;
    context.control->interact = SDL_SCANCODE_E;

    t_tileset* tileset = initTileset(context.renderer, 192, 240, 16, "./assets/imgs/tileMapDungeon.bmp");
    t_character* c = createCharactere(tileset, 98);

    t_item* item = malloc(sizeof(t_item));
    t_item* item1 = malloc(sizeof(t_item));
    t_item* item2 = malloc(sizeof(t_item));

    t_item** itemListe = malloc(sizeof(t_item*) * 40);

    strcpy(item->name, "Marteau de thor");
    item->flags = ITEM_FLAG_STACKABLE;
    item->stats.attack.additive = 0;
    item->stats.attack.multiplicative = 10;
    item->stats.defense.additive = 0;
    item->stats.defense.multiplicative = 1;
    item->stats.health.additive = 50;
    item->stats.health.multiplicative = 5;
    item->stats.healthMax.additive = 500;
    item->stats.healthMax.multiplicative = -25;
    item->stats.mana.additive = 15;
    item->stats.mana.multiplicative = 1.2;
    item->stats.manaMax.additive = 20;
    item->stats.manaMax.multiplicative = 3;
    item->stats.speed.additive = 60;
    item->stats.speed.multiplicative = 0;
    item->indiceTexture = 102;
    item->texture = (SDL_Texture*)getObject(tileset->textureTiles, item->indiceTexture);
    strcpy(item->description, "\nça c'est de l'arme \nguts weapon\nbla bla bla\navec ça tu gagnes\nà coup sur");

    strcpy(item1->name, "de thor");
    item1->flags = ITEM_FLAG_STACKABLE;
    item1->stats.attack.additive = 0;
    item1->stats.attack.multiplicative = 10;
    item1->stats.defense.additive = 0;
    item1->stats.defense.multiplicative = 1;
    item1->stats.health.additive = 50;
    item1->stats.health.multiplicative = 5;
    item1->stats.healthMax.additive = 500;
    item1->stats.healthMax.multiplicative = -25;
    item1->stats.mana.additive = 15;
    item1->stats.mana.multiplicative = 1.2;
    item1->stats.manaMax.additive = 20;
    item1->stats.manaMax.multiplicative = 3;
    item1->stats.speed.additive = 60;
    item1->stats.speed.multiplicative = 0;
    item1->indiceTexture = 103;
    item1->texture = (SDL_Texture*)getObject(tileset->textureTiles, item1->indiceTexture);
    strcpy(item1->description, "\nça c'est de l'arme \nazdefddsc sqdqff\nbla bla bla\navec ça tu gagnes\nà coup sur");

    strcpy(item2->name, "Marteau");
    item2->flags = ITEM_FLAG_STACKABLE;
    item2->stats.attack.additive = 0;
    item2->stats.attack.multiplicative = 10;
    item2->stats.defense.additive = 0;
    item2->stats.defense.multiplicative = 1;
    item2->stats.health.additive = 50;
    item2->stats.health.multiplicative = 5;
    item2->stats.healthMax.additive = 500;
    item2->stats.healthMax.multiplicative = -25;
    item2->stats.mana.additive = 15;
    item2->stats.mana.multiplicative = 1.2;
    item2->stats.manaMax.additive = 20;
    item2->stats.manaMax.multiplicative = 3;
    item2->stats.speed.additive = 60;
    item2->stats.speed.multiplicative = 0;
    item2->indiceTexture = 101;
    item2->texture = (SDL_Texture*)getObject(tileset->textureTiles, item2->indiceTexture);
    strcpy(item2->description, "\nqzzehbtb hj \nguts weapon\nbla bla bla\navec ça tu gagnes\nà coup sur");

    for (int i = 0; i < 15; i++) {
        itemListe[i] = item;
    }
    for (int i = 15; i < 30; i++) {
        itemListe[i] = item1;
    }
    for (int i = 30; i < 40; i++) {
        itemListe[i] = item2;
    }

    t_scene* scene = createMainMenu(&context);
    t_scene* scene0 = createOptionMenu(&context);
    t_scene* scene1 = createMainWord(&context);
    t_scene* scene2 = createCommandeMenu(&context);
    t_scene* scene3 = createFpsMenu(&context);
    t_scene* scene4 = createMainInv(&context, c, tileset, itemListe);

    addScene(context.sceneController, scene);
    addScene(context.sceneController, scene0);
    addScene(context.sceneController, scene1);
    addScene(context.sceneController, scene2);
    addScene(context.sceneController, scene3);
    addScene(context.sceneController, scene4);
    setScene(context.sceneController, "mainInv");

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
    freeObjectManager(context.sceneController->scene);
    free(context.control);

    TTF_CloseFont(context.font);
    freeInput(context.input);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    return EXIT_SUCCESS;
}