#include <SDL2/SDL.h>

#include "graphic/tilesManager.h"
#include "io/imageLoader.h"
#include "io/input.h"
#include "utils/objectManager.h"

#define LARGEUR 1260
#define HAUTEUR 620

void* GET_TYPED_OBJECT(t_objectManager* manager, size_t index) {
    if (manager == NULL || index >= manager->count) {
        return NULL;
    }

    t_typedObject* obj = manager->items[index];
    return obj->data;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR, HAUTEUR, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_tileset* tileset = initTileset(renderer, 192, 176, 16, "../assets/imgs/tileMapDungeon.bmp");
    t_grid* level = createGrid(12, 11, 2);

    appliquerTextureNiveau(level, 0, GET_TYPED_OBJECT(tileset->textureTiles, 1));
    appliquerTextureNiveau(level, 1, GET_TYPED_OBJECT(tileset->textureTiles, 98));

    t_input input;
    initInput(&input, LARGEUR, HAUTEUR);
    while (!input.quit) {
        updateInput(&input);
        dessinerGrille(renderer, level, input.windowWidth, input.windowHeight);
        SDL_RenderPresent(renderer);
    }

    freeGridTiles(level);
    freeTileset(tileset);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}
