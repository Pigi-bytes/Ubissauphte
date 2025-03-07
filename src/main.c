#include "./io/equipementsManager.h"

int main() {
    t_fichier* fichier = chargerFichier("./src/test.txt");
    t_fichier* fichier1 = (t_fichier*)malloc(sizeof(t_fichier));

    t_item** item = item_load(fichier);
    t_inventaire* inv = createInventaire();

    t_character c1;

    item_save(item, fichier1, fichier->blockManager->count);
    saveFichier(fichier1, "nouv.txt");

    inventaireAjoutObjet(inv, item[0], 5);
    t_itemsStack* itemStack = (t_itemsStack*)getObject(inv->itemsStack, 0);
    printf("%d\n", itemStack->quantite);
    inventaireAjoutObjet(inv, item[5], 2);
    itemStack = (t_itemsStack*)getObject(inv->itemsStack, 1);
    printf("%d\n", itemStack->quantite);

    c1.inventaire = inv;
    c1.equipement[SLOT_ARME].stack = NULL;
    c1.equipement[SLOT_ARMURE].stack = NULL;
    c1.equipement[SLOT_ACTIVABLE1].stack = NULL;
    c1.equipement[SLOT_ACTIVABLE2].stack = NULL;

    equiperEquipement(&c1, 0, SLOT_ARME);
    if (c1.equipement[SLOT_ARME].stack != NULL)
        printf("%s\n", c1.equipement[SLOT_ARME].stack->definition->name);

    desequiperEquipement(&c1, SLOT_ARME);
    if (c1.equipement[SLOT_ARME].stack != NULL)
        printf("%s\n", c1.equipement[SLOT_ARME].stack->definition->name);

    printf("%d\n", itemStack->definition->validSlot[0]);
    equiperEquipement(&c1, 1, SLOT_ACTIVABLE1);
    if (c1.equipement[SLOT_ACTIVABLE1].stack != NULL)
        printf("%s\n", c1.equipement[SLOT_ACTIVABLE1].stack->definition->name);

    free_item(item, fichier->blockManager->count);
    itemFree(inv);
    freeFichier(fichier);
    freeFichier(fichier1);
    return 0;
}