#include "equipementsManager.h"

int hasFlag(int itemFlags, itemsFlags flag) {
    return (itemFlags & flag) != 0;  // Vérifie si le bit correspondant est activé
}

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

        sprintf(value, "%d", item[i]->flags);
        data = createPairData("flags", value);
        addPairData(block, data);

        if (item[i]->validSlot[0] == SLOT_ARME)
            data = createPairData("type", "arme");
        else if (item[i]->validSlot[0] == SLOT_ARMURE)
            data = createPairData("type", "armure");
        else if (item[i]->validSlot[0] == SLOT_ACTIVABLE1 || item[i]->validSlot[0] == SLOT_ACTIVABLE2)
            data = createPairData("type", "activable");

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

        getValue(block, "flags", &resultInt, INT);
        item[i]->flags = resultInt;

        getValue(block, "type", &resultChar, STRING);
        if (strcmp(resultChar, "arme") == 0)
            item[i]->validSlot[0] = SLOT_ARME;
        else if (strcmp(resultChar, "armure") == 0)
            item[i]
                ->validSlot[0] = SLOT_ARMURE;
        else if (strcmp(resultChar, "activable") == 0) {
            item[i]->validSlot[0] = SLOT_ACTIVABLE1;
            item[i]->validSlot[1] = SLOT_ACTIVABLE2;
        }
    }

    return item;
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

    if (itemStack != NULL) {
        if ((hasFlag(itemStack->definition->flags, ITEM_FLAG_STACKABLE)))
            itemStack->quantite += quantite;

    } else {
        t_itemsStack* itemStackNew = (t_itemsStack*)malloc(sizeof(t_itemsStack));
        itemStackNew->definition = item;
        if (hasFlag(item->flags, ITEM_FLAG_STACKABLE))
            itemStackNew->quantite = quantite;
        else
            itemStackNew->quantite = 1;
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

void equiperEquipement(t_character* c, int inventoryIndex, equipementSlotType slot) {
    int i;

    t_itemsStack* itemStack = (t_itemsStack*)getObject(c->inventaire->itemsStack, inventoryIndex);
    if (itemStack == NULL || itemStack->definition == NULL) {
        fprintf(stderr, "Erreur: ItemStack ou definition invalide.\n");
        return;
    }
    for (i = 0; i < 2; i++) {
        printf("%d\n", itemStack->definition->validSlot[i]);
        if (slot == itemStack->definition->validSlot[i])
            break;
    }
    if (slot == itemStack->definition->validSlot[i])
        c->equipement[slot].stack = itemStack;
    else
        printf("Slot invalide\n");
}

void desequiperEquipement(t_character* c, equipementSlotType slot) {
    c->equipement[slot].stack = NULL;
}

void free_item(t_item** item, int count) {
    for (int i = 0; i < count; i++) {
        if (item[i] != NULL) {
            free(item[i]);
        }
    }
    free(item);
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
