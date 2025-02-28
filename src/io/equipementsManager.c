#include "equipementsManager.h"

void item_save(t_item** item, t_fichier* fichier) {
    t_pairData* data;

    for (int i = 0; i < sizeof(item); i++) {
        char value[20];
        t_block* block=(t_block*)malloc(sizeof(t_block));

        sprintf(value, "%d", item[i]->id);
        data = createPairData("id Item", value);
        addPairData(block, data);

        data = createPairData("Name", item[i]->name);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.attack.additive);
        data = createPairData("attack Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.attack.multiplicative);
        data = createPairData("attack Mult Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.defense.additive);
        data = createPairData("defense Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.defense.multiplicative);
        data = createPairData("defense Mult Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.health.additive);
        data = createPairData("health Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.health.multiplicative);
        data = createPairData("health Mult Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.healthMax.additive);
        data = createPairData("healthMax Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.healthMax.multiplicative);
        data = createPairData("healthMax Mult Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.mana.additive);
        data = createPairData("mana Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.mana.multiplicative);
        data = createPairData("mana Mult Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.manaMax.additive);
        data = createPairData("manaMax Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.manaMax.multiplicative);
        data = createPairData("manaMax Mult Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.speed.additive);
        data = createPairData("speed Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.speed.multiplicative);
        data = createPairData("speed Mult Modifier", value);
        addPairData(block, data);

        addBlock(fichier, block);
    }
}

// bool inventory_save(t_fichier* fichier, t_inventaire* inv) {
//     t_block* block;
//     block->pairManager = inv->items;
//     addBlock(fichier, block);
// }

t_item** item_load(t_fichier* fichier) {
    t_item** item = (t_item**)malloc(sizeof(t_item));

    void* result;
    t_block* block;

    for (int i = 0; i < fichier->blockManager->count; i++) {
        block = (t_block*)getObject(fichier->blockManager, i);

        getValue(block, "id Item", &result, INT);
        item[i]->id = (int)result;

        getValue(block, "Name", &result, STRING);
        strcpy(item[i]->name, (char*)result);

        getValue(block, "attack Add Modifier", &result, INT);
        item[i]->stats.attack.additive = (int)result;

        getValue(block, "attack Mult Modifier", &result, INT);
        item[i]->stats.attack.multiplicative = (int)result;

        getValue(block, "defense Add Modifier", &result, INT);
        item[i]->stats.defense.additive = (int)result;

        getValue(block, "defense Mult Modifier", &result, INT);
        item[i]->stats.defense.multiplicative = (int)result;

        getValue(block, "health Add Modifier", &result, INT);
        item[i]->stats.health.multiplicative = (int)result;

        getValue(block, "health Mult Modifier", &result, INT);
        item[i]->stats.health.multiplicative = (int)result;

        getValue(block, "healthMax Add Modifier", &result, INT);
        item[i]->stats.healthMax.additive = (int)result;

        getValue(block, "healthMax Mult Modifier", &result, INT);
        item[i]->stats.healthMax.multiplicative = (int)result;

        getValue(block, "mana Add Modifier", &result, INT);
        item[i]->stats.mana.additive = (int)result;

        getValue(block, "mana Mult Modifier", &result, INT);
        item[i]->stats.mana.multiplicative = (int)result;

        getValue(block, "manaMax Add Modifier", &result, INT);
        item[i]->stats.manaMax.additive = (int)result;

        getValue(block, "manaMax Mult Modifier", &result, INT);
        item[i]->stats.manaMax.multiplicative = (int)result;

        getValue(block, "speed Add Modifier", &result, INT);
        item[i]->stats.speed.additive = (int)result;

        getValue(block, "speed Mult Modifier", &result, INT);
        item[i]->stats.speed.multiplicative = (int)result;
    }
    return item;
}