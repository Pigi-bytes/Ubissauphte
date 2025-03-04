#include "./io/equipementsManager.h"

int main() {
    t_fichier* fichier = chargerFichier("./src/test.txt");
    t_fichier* fichier1 = (t_fichier*)malloc(sizeof(t_fichier));

    t_item** item = item_load(fichier);
    t_inventaire* inv = createInventaire();

    item_save(item, fichier1, fichier->blockManager->count);
    saveFichier(fichier1, "nouv.txt");

    inventaireAjoutObjet(inv, item[0], 5);
    t_itemsStack* itemStack = (t_itemsStack*)getObject(inv->itemsStack, 0);
    printf("%d\n", itemStack->quantite);
    inventaireAjoutObjet(inv, item[0], 2);
    itemStack = (t_itemsStack*)getObject(inv->itemsStack, 0);
    printf("%d\n", itemStack->quantite);

    free_item(item, fichier->blockManager->count);
    itemFreeFunc(inv);
    freeFichier(fichier);
    freeFichier(fichier1);
    return 0;
}