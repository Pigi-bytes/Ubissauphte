#include "./sceneInventaire.h"

t_scene *createMainInv(t_context *context) {
    t_typeRegistry *registre = createTypeRegistry();
    t_fichier *fichier = chargerFichier("src/test.txt");
    t_tileset *tileset = initTileset(context->renderer, 192, 240, 16, "./assets/imgs/tileMapDungeon.bmp");

    t_item **itemListe = item_load(fichier, tileset);

    t_character *c = createCharactere(tileset, 98);

    for (int i = 0; i < 40; i++) {
        inventaireAjoutObjet(c->inventaire, itemListe[i], 1);
    }

    InventoryUI *ui = inventoryUI_Init(NULL, context->renderer, c, context->input);
    const uint8_t INVENTORY_TYPE = registerType(registre, freeInv, "inventory");

    t_scene *scene = createScene(initObjectManager(registre), "mainInv");

    // InventoryUI *ui = inventoryUI_Init(NULL, context->renderer, c, context->input);

    ADD_OBJECT_TO_SCENE(scene, ui, INVENTORY_TYPE);

    sceneRegisterFunction(scene, INVENTORY_TYPE, HANDLE_INPUT, updateWrapper, 1, FONCTION_PARAMS(context->input));
    sceneRegisterFunction(scene, INVENTORY_TYPE, UPDATE, updateScrollWrapper, 0, FONCTION_PARAMS(context->input));
    sceneRegisterFunction(scene, INVENTORY_TYPE, UPDATE, inventoryUI_UpdateWrapper, 0, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, INVENTORY_TYPE, RENDER_UI, inventoryUI_RenderWrapper, 0, FONCTION_PARAMS(context));

    return scene;
}
// int main() {
//     SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//     t_input *input = initInput(window_width, window_height);

//     t_fichier *fichier = chargerFichier("src/test.txt");
//     printf("yes\n");
//     t_tileset *tileset = initTileset(renderer, 192, 240, 16, "./assets/imgs/tileMapDungeon.bmp");

//     t_item **itemListe = item_load(fichier, tileset);

//     t_character *c = createCharactere(tileset, 98);

//     for (int i = 0; i < 40; i++) {
//         inventaireAjoutObjet(c->inventaire, itemListe[i], 1);

//     }

//     InventoryUI *ui = inventoryUI_Init(NULL, renderer, c, input);
//     ;
//     /* const uint8 Case_TYpe = (registre,null,"case")

//     init (ui,) */

//     while (!input->quit) {
//         update(input, ui);
//         updateInput(input);

//         updateScroll(ui, input);

//         inventoryUI_Update(ui, renderer, input);

//         inventoryUI_Render(ui, renderer, input);

//         SDL_RenderPresent(renderer);
//     }

//     SDL_DestroyWindow(window);
//     SDL_DestroyRenderer(renderer);
//     SDL_Quit();
//     return EXIT_SUCCESS;
// }