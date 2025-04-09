#include "./sceneInventaire.h"

t_scene *createMainInv(t_context *context, t_joueur *player) {
    t_typeRegistry *registre = createTypeRegistry();
    t_tileset *tileset = initTileset(context->renderer, 192, 240, 16, "./assets/imgs/tileMapDungeon.bmp");

   

    inventaireAjoutObjet(player->inventaire, context->itemListe[2], 1);
    equiperEquipement(&player, 0, SLOT_ARME);

    InventoryUI *ui = inventoryUI_Init(NULL, player, context);
    const uint8_t INVENTORY_TYPE = registerType(registre, freeInv, "inventory");

    t_scene *scene = createScene(initObjectManager(registre), "mainInv");

    ADD_OBJECT_TO_SCENE(scene, ui, INVENTORY_TYPE);

    sceneRegisterFunction(scene, INVENTORY_TYPE, HANDLE_INPUT, updateWrapper, 1, FONCTION_PARAMS(context->input));
    sceneRegisterFunction(scene, INVENTORY_TYPE, UPDATE, updateScrollWrapper, 0, FONCTION_PARAMS(context->input));
    sceneRegisterFunction(scene, INVENTORY_TYPE, UPDATE, inventoryUI_UpdateWrapper, 0, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, INVENTORY_TYPE, RENDER_UI, inventoryUI_RenderWrapper, 0, FONCTION_PARAMS(context));
    sceneRegisterFunction(scene, INVENTORY_TYPE, HANDLE_INPUT, handleInputInventaireWrapper, -1, FONCTION_PARAMS(context->input, player, context->sceneController));

    return scene;
}