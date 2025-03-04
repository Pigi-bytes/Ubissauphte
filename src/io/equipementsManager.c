#include "equipementsManager.h"

void item_save(t_item** item, t_fichier* fichier, int count) {
    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, blockFreeFunc, "BLOCK_TYPE");
    registerType(registre, pairFreeFunc, "PAIR_TYPE");

    fichier->blockManager = initObjectManager(registre);
    for (int i = 0; i < count; i++) {
        char value[50];

        t_pairData* data;
        t_block* block = createNewBlock();

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

t_item** item_load(t_fichier* fichier) {
    t_item** item = (t_item**)malloc(fichier->blockManager->count * sizeof(t_item*));
    if (item == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    int resultInt;
    float resultFLOAT;
    char resultChar[50];
    t_block* block;

    for (int i = 0; i < fichier->blockManager->count; i++) {
        item[i] = (t_item*)malloc(sizeof(t_item));

        block = (t_block*)getObject(fichier->blockManager, i);

        getValue(block, "id Item", &resultInt, INT);
        item[i]->id = resultInt;

        getValue(block, "Name", resultChar, STRING);
        strcpy(item[i]->name, resultChar);

        getValue(block, "attack Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.attack.additive = resultFLOAT;

        getValue(block, "attack Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.attack.multiplicative = resultFLOAT;

        getValue(block, "defense Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.defense.additive = resultFLOAT;

        getValue(block, "defense Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.defense.multiplicative = resultFLOAT;

        getValue(block, "health Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.health.additive = resultFLOAT;

        getValue(block, "health Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.health.multiplicative = resultFLOAT;

        getValue(block, "healthMax Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.healthMax.additive = resultFLOAT;

        getValue(block, "healthMax Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.healthMax.multiplicative = resultFLOAT;

        getValue(block, "mana Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.mana.additive = resultFLOAT;

        getValue(block, "mana Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.mana.multiplicative = resultFLOAT;

        getValue(block, "manaMax Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.manaMax.additive = resultFLOAT;

        getValue(block, "manaMax Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.manaMax.multiplicative = resultFLOAT;

        getValue(block, "speed Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.speed.additive = resultFLOAT;

        getValue(block, "speed Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.speed.multiplicative = resultFLOAT;
    }

    return item;
}

void free_item(t_item** item, int count) {
    for (int i = 0; i < count; i++) {
        if (item[i] != NULL) {
            free(item[i]);
        }
    }
    free(item);
}

t_inventaire* createInventaire() {
    t_inventaire* inv = (t_inventaire*)malloc(sizeof(t_inventaire));

    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, itemFreeFunc, "ITEMSTACK_TYPE");
    inv->itemsStack = initObjectManager(registre);
    return inv;
}

void inventaireAjoutObjet(t_inventaire* inv, t_item* item, int quantite) {
    t_itemsStack* itemStack = inventaireFindStack(inv, item);

    if (itemStack != NULL)
        itemStack->quantite += quantite;
    else {
        t_itemsStack* itemStackNew = (t_itemsStack*)malloc(sizeof(t_itemsStack));
        itemStackNew->definition = item;
        itemStackNew->quantite = quantite;
        addObject(inv->itemsStack, itemStackNew, getTypeIdByName(inv->itemsStack->registry, "ITEMSTACK_TYPE"));
    }
}

t_itemsStack* inventaireFindStack(t_inventaire* inv, t_item* item) {
    for (int i = 0; i < inv->itemsStack->count; i++) {
        t_itemsStack* itemStack = (t_itemsStack*)getObject(inv->itemsStack, i);
        if (strcmp(itemStack->definition->name, item->name) == 0)
            return itemStack;
    }
    return NULL;
}

void itemFree(void* data) {
    t_inventaire* inv = (t_inventaire*)data;
    if (inv && inv->itemsStack)
        freeObjectManager(inv->itemsStack);

    free(inv);
}

void itemFreeFunc(void* data) {
    t_itemsStack* inv = (t_itemsStack*)data;
    if (inv)
        free(inv);
}
