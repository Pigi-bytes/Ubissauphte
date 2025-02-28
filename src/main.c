#include "./io/equipementsManager.h"

int main() {
    t_fichier* fichier = chargerFichier("test.txt");
    t_fichier* fichier1;
    t_item** item = item_load(fichier);

    item_save(item, fichier1);

    saveFichier(fichier1, "nouv.txt");

    return 0;
}