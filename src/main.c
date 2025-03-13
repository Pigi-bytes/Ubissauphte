#include "./io/equipementsManager.h"

int main() {
    t_fichier* fichier = chargerFichier("./src/test.txt");
    t_fichier* fichier1 = (t_fichier*)malloc(sizeof(t_fichier));
    t_fichier* fichier2 = (t_fichier*)malloc(sizeof(t_fichier));

    t_item** item = item_load(fichier);

    t_character* c1 = createCharactere();
    t_inventaire* inv;

    item_save(item, fichier1, fichier->blockManager->count);
    saveFichier(fichier1, "nouv.txt");

    equipment_print(c1);
    printf("\n");

    inventaireAjoutObjet((*c1).inventaire, item[0], 5);
    t_itemsStack* itemStack = (t_itemsStack*)getObject((*c1).inventaire->itemsStack, 0);
    printf("%d\n", itemStack->quantite);
    inventaireAjoutObjet((*c1).inventaire, item[5], 2);
    itemStack = (t_itemsStack*)getObject((*c1).inventaire->itemsStack, 1);
    printf("%d\n", itemStack->quantite);

    equiperEquipement(&c1, 0, SLOT_ARME);
    if (c1->equipement[SLOT_ARME].stack != NULL)
        printf("%s\n", c1->equipement[SLOT_ARME].stack->definition->name);

    equiperEquipement(&c1, 1, SLOT_ACTIVABLE1);
    if (c1->equipement[SLOT_ACTIVABLE1].stack != NULL)
        printf("%s\n", c1->equipement[SLOT_ACTIVABLE1].stack->definition->name);

    equipment_print(c1);
    printf("\n");

    inventory_save(c1->inventaire, fichier2);
    saveFichier(fichier2, "inventaire");

    inv = inventory_load(fichier2, item, fichier->blockManager->count);
    inventory_print(inv);

    
    free_item(item, fichier->blockManager->count);
    charactereFree(c1);
    itemFree(inv);
    freeFichier(fichier);
    freeFichier(fichier1);
    freeFichier(fichier2);
    return 0;
}