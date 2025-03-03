#include "./io/equipementsManager.h"

int main() {
    t_fichier* fichier = chargerFichier("./src/test.txt");

    t_item** item = item_load(fichier);

    t_fichier* fichier1 = item_save(item, fichier->blockManager->count);

     saveFichier(fichier1, "nouv.txt");
    printf("%f\n", item[1]->stats.health.additive);

    free_item(item,fichier->blockManager->count);
    freeFichier(fichier);
    freeFichier(fichier1);
    return 0;
}