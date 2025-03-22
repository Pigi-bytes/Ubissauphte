#include "_scene/commandeMenu.h"
#include "_scene/fpsMenu.h"
#include "_scene/mainMenu.h"
#include "_scene/mainWorld.h"
#include "_scene/optionMenu.h"

int main(int argc, char* argv[]) {
    if (initAudioSystem() != 0) {
        return 1;  // Arrêter si l'initialisation échoue
    }

    SDL_Init(SDL_INIT_VIDEO);
    initTextEngine();
    SDL_Window* window = SDL_CreateWindow("Lo Poti Donjon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Debug_Init(renderer);

    t_input* input = initInput(WINDOW_WIDTH, WINDOW_HEIGHT);
    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 24);
    t_frameData* frameData = initFrameData(0);
    t_option* option = creeOption();
    t_sceneController* sceneController = initSceneController();
    t_fpsDisplay* fpsDisplay = initFPSDisplay(renderer, font);

    t_control* contr = malloc(sizeof(t_control));
    contr->up = SDL_SCANCODE_W;
    contr->down = SDL_SCANCODE_S;
    contr->left = SDL_SCANCODE_A;
    contr->right = SDL_SCANCODE_D;
    contr->dash = SDL_SCANCODE_SPACE;
    contr->escape = SDL_SCANCODE_ESCAPE;
    contr->map = SDL_SCANCODE_P;
    t_scene* scene = createMainMenu(renderer, input, font, frameData, sceneController);
    t_scene* scene0 = createOptionMenu(renderer, input, font, frameData, window, option, contr, sceneController);
    t_scene* scene1 = createMainWord(renderer, input, font, frameData, contr, sceneController);
    t_scene* scene2 = createCommandeMenu(renderer, input, font, window, option, contr, sceneController);
    t_scene* scene3 = createFpsMenu(renderer, input, font, frameData, fpsDisplay, window, option, sceneController);

    addScene(sceneController, scene);
    addScene(sceneController, scene0);
    addScene(sceneController, scene1);
    addScene(sceneController, scene2);
    addScene(sceneController, scene3);

    setScene(sceneController, "menuPrincipal");
    while (!input->quit) {
        startFrame(frameData);
        t_scene* currentScene = getCurrentScene(sceneController);

        executeSceneFunctions(currentScene, HANDLE_INPUT);
        updateInput(input);

        executeSceneFunctions(currentScene, UPDATE);

        updateFPS(frameData);
        updateFPSDisplay(fpsDisplay, frameData, renderer);

        if (input->resized) {
            executeSceneFunctions(currentScene, HANDLE_RESIZE);
        }

        SDL_RenderClear(renderer);
        executeSceneFunctions(currentScene, SET_BUFFER);
        executeSceneFunctions(currentScene, RENDER_GAME);

        Debug_RenderAll();
        executeSceneFunctions(currentScene, RENDER_BUFFER);
        executeSceneFunctions(currentScene, RENDER_UI);
        renderFPSDisplay(renderer, fpsDisplay);

        SDL_RenderPresent(renderer);

        capFrameRate(frameData);
    }

    freeOption(option);
    freeFrameData(frameData);
    freeObjectManager(sceneController->scene);
    free(contr);

    TTF_CloseFont(font);
    freeInput(input);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}