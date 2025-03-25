#include "./ui/affichage.h"

int main() {
    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_input* input = initInput(window_width, window_height);

    t_tileset* tileset = initTileset(renderer, 192, 240, 16, "./assets/imgs/tileMapDungeon.bmp");
    t_character* c = createCharactere(tileset, 98);
    t_item* item = malloc(sizeof(t_item));

    t_inventaire* inv = createInventaire();

    strcpy(item->name, "Marteau de thor");
    item->flags = ITEM_FLAG_STACKABLE;
    item->stats.attack.additive = 10;
    item->stats.defense.additive = 5;
    item->stats.health.additive = 50;
    item->stats.healthMax.additive = 500;
    item->stats.mana.additive = 15;
    item->stats.manaMax.additive = 20;
    item->stats.speed.additive = 60;
    strcpy(item->description, "\nça c'est de l'arme \nguts weapon\nbla bla bla\navec ça tu gagnes\nà coup sur");

    while (!input->quit) {
        updateInput(input);

        afficherInventaire(renderer, input, c, item);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}