// #include "./ui/affichage.h"

// int main() {
//     SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//     t_input *input = initInput(window_width, window_height);

//     t_tileset *tileset = initTileset(renderer, 192, 240, 16, "./assets/imgs/tileMapDungeon.bmp");
//     t_character *c = createCharactere(tileset, 98);
//     t_item *item = malloc(sizeof(t_item));
//     t_item *itemListe[40];
//     t_inventaire *inv = createInventaire();

//     strcpy(item->name, "Marteau de thor");
//     item->flags = ITEM_FLAG_STACKABLE;
//     item->stats.attack.additive = 0;
//     item->stats.attack.multiplicative = 10;
//     item->stats.defense.additive = 0;
//     item->stats.defense.multiplicative = 1;
//     item->stats.health.additive = 50;
//     item->stats.health.multiplicative = 5;
//     item->stats.healthMax.additive = 500;
//     item->stats.healthMax.multiplicative = -25;
//     item->stats.mana.additive = 15;
//     item->stats.mana.multiplicative = 1.2;
//     item->stats.manaMax.additive = 20;
//     item->stats.manaMax.multiplicative = 3;
//     item->stats.speed.additive = 60;
//     item->stats.speed.multiplicative = 0;
//     item->indiceTexture = 102;
//     item->texture = (SDL_Texture *)getObject(tileset->textureTiles, item->indiceTexture);
//     strcpy(item->description, "\nça c'est de l'arme \nguts weapon\nbla bla bla\navec ça tu gagnes\nà coup sur");

//     for (int i = 0; i < 40; i++) {
//         itemListe[i] = item;
//     }

//     InventoryUI ui;
//     /* const uint8 Case_TYpe = (registre,null,"case")

//     init (ui,) */
//     inventoryUI_Init(&ui, renderer, c, itemListe, input, 40);

//     while (!input->quit) {
//         int width = input->windowWidth;
//         int height = input->windowHeight;
//         updateInput(input);

//         updateScroll(&ui, input);

//         if (input->windowWidth != width || input->windowHeight != height) {
//             inventoryUI_Update(&ui, renderer, input);
//         }
//         inventoryUI_Render(&ui, renderer, input);

//         SDL_RenderPresent(renderer);
//     }

//     SDL_DestroyWindow(window);
//     SDL_DestroyRenderer(renderer);
//     SDL_Quit();
//     return EXIT_SUCCESS;
// }