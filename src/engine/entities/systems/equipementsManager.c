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

        data = createPairData("descrition", item[i]->description);
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

        sprintf(value, "%f", item[i]->indiceTexture);
        data = createPairData("texture", value);
        addPairData(block, data);

        if (item[i]->validSlot[0] == SLOT_ARME)
            data = createPairData("type", "arme");
        else if (item[i]->validSlot[0] == SLOT_ARMURE)
            data = createPairData("type", "armure");
        else if (item[i]->validSlot[0] == SLOT_ACTIVABLE1 || item[i]->validSlot[0] == SLOT_ACTIVABLE2)
            data = createPairData("type", "activable");
        addPairData(block, data);

        if (item[i]->onEquip == NULL)
            data = createPairData("onEquipe", "0");
        else
            data = createPairData("onEquipe", "1");
        addPairData(block, data);

        if (item[i]->onDeEquip == NULL)
            data = createPairData("onDeEquipe", "0");
        else
            data = createPairData("onDeEquipe", "1");
        addPairData(block, data);

        addBlock(fichier, block);
    }
}

t_item** item_load(t_fichier* fichier, t_tileset* tileset) {
    t_item** item = (t_item**)malloc(fichier->blockManager->count * sizeof(t_item*));
    if (item == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    int resultInt;
    float resultFLOAT;
    char resultChar[200];
    t_block* block;

    for (int i = 0; i < fichier->blockManager->count; i++) {
        item[i] = (t_item*)malloc(sizeof(t_item));

        block = (t_block*)getObject(fichier->blockManager, i);

        getValue(block, "id Item", &resultInt, INT);
        item[i]->id = resultInt;

        getValue(block, "Name", resultChar, STRING);
        strcpy(item[i]->name, resultChar);

        getValue(block, "description", resultChar, STRING);
        strcpy(item[i]->description, resultChar);

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

        getValue(block, "texture", &resultInt, INT);
        item[i]->texture = getObject(tileset->textureTiles, resultInt);
        item[i]->indiceTexture = resultInt;

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

        getValue(block, "OnEquipe", &resultInt, INT);
        if (resultInt == 0)
            item[i]->onEquip = NULL;
        getValue(block, "OnDeEquipe", &resultInt, INT);
        if (resultInt == 0)
            item[i]->onDeEquip = NULL;
    }

    return item;
}

void inventory_save(t_inventaire* inv, t_fichier* fichier) {
    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, blockFreeFunc, "BLOCK_TYPE");
    registerType(registre, pairFreeFunc, "PAIR_TYPE");

    fichier->blockManager = initObjectManager(registre);

    char value[50];

    t_pairData* data;
    t_block* block = createNewBlock();

    sprintf(value, "%d", inv->itemsStack->count);
    data = createPairData("nbItems", value);
    addPairData(block, data);
    addBlock(fichier, block);

    for (int i = 0; i < inv->itemsStack->count; i++) {
        char value[50];

        t_pairData* data;
        t_block* block = createNewBlock();

        t_itemsStack* Stack = (t_itemsStack*)getObject(inv->itemsStack, i);

        sprintf(value, "%d", i);
        data = createPairData("itemNb", value);
        addPairData(block, data);

        sprintf(value, "%d", Stack->definition->id);
        data = createPairData("id Item", value);
        addPairData(block, data);

        data = createPairData("Name", Stack->definition->name);
        addPairData(block, data);

        sprintf(value, "%d", Stack->quantite);
        data = createPairData("Quantite", value);
        addPairData(block, data);

        addBlock(fichier, block);
    }
}

t_inventaire* inventory_load(t_fichier* fichier, t_item** item, int count) {
    t_inventaire* inv = createInventaire();
    if (inv == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    int resultInt;
    char resultChar[50];
    t_block* block;

    for (int i = 1; i < fichier->blockManager->count; i++) {
        block = (t_block*)getObject(fichier->blockManager, i);

        t_itemsStack* Stack = (t_itemsStack*)malloc(sizeof(t_itemsStack));
        Stack->definition = (t_item*)malloc(sizeof(t_item));

        getValue(block, "id Item", &resultInt, INT);
        Stack->definition->id = resultInt;

        getValue(block, "Name", resultChar, STRING);
        strcpy(Stack->definition->name, resultChar);

        getValue(block, "Quantite", &resultInt, INT);
        Stack->quantite = resultInt;

        for (int j = 0; j < count; j++) {
            if ((Stack->definition->id == item[j]->id) && (!strcmp(Stack->definition->name, item[j]->name))) {
                inventaireAjoutObjet(inv, item[j], Stack->quantite);
                break;
            }
        }
        free(Stack->definition);
        free(Stack);
    }

    return inv;
}

t_inventaire* createInventaire() {
    t_inventaire* inv = (t_inventaire*)malloc(sizeof(t_inventaire));

    t_typeRegistry* registre = createTypeRegistry();
    registerType(registre, itemFreeFunc, "ITEMSTACK_TYPE");
    inv->itemsStack = initObjectManager(registre);
    return inv;
}

t_character* createCharactere() {
    t_character* charactere = (t_character*)malloc(sizeof(t_character));

    charactere->inventaire = createInventaire();
    charactere->baseStats.health.additive = 10;
    charactere->baseStats.health.multiplicative = 1;

    charactere->baseStats.healthMax.additive = 10;
    charactere->baseStats.healthMax.multiplicative = 1;

    charactere->baseStats.mana.additive = 10;
    charactere->baseStats.mana.multiplicative = 1;

    charactere->baseStats.manaMax.additive = 10;
    charactere->baseStats.manaMax.multiplicative = 1;

    charactere->baseStats.attack.additive = 10;
    charactere->baseStats.attack.multiplicative = 1;

    charactere->baseStats.defense.additive = 10;
    charactere->baseStats.defense.multiplicative = 1;

    charactere->baseStats.speed.additive = 10;
    charactere->baseStats.speed.multiplicative = 1;

    charactere->calculatedStats.health.additive = 0;
    charactere->calculatedStats.health.multiplicative = 1;

    charactere->calculatedStats.healthMax.additive = 0;
    charactere->calculatedStats.healthMax.multiplicative = 1;

    charactere->calculatedStats.mana.additive = 0;
    charactere->calculatedStats.mana.multiplicative = 1;

    charactere->calculatedStats.manaMax.additive = 0;
    charactere->calculatedStats.manaMax.multiplicative = 1;

    charactere->calculatedStats.attack.additive = 0;
    charactere->calculatedStats.attack.multiplicative = 1;

    charactere->calculatedStats.defense.additive = 0;
    charactere->calculatedStats.defense.multiplicative = 1;

    charactere->calculatedStats.speed.additive = 0;
    charactere->calculatedStats.speed.multiplicative = 1;

    charactere->level = 1;
    charactere->gold = 5000;
    charactere->experience = 0;

    charactere->equipement[SLOT_ARME].stack = NULL;
    charactere->equipement[SLOT_ARMURE].stack = NULL;
    charactere->equipement[SLOT_ACTIVABLE1].stack = NULL;
    charactere->equipement[SLOT_ACTIVABLE2].stack = NULL;

    return charactere;
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

void equipementRecalculerStats(t_character* c) {
    // Initialisation des valeurs finales
    t_stats finalStats = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};

    // Parcours de tous les slots d'équipement
    for (int i = 0; i < TOTAL_EQUIPMENT_SLOTS; i++) {
        if (c->equipement[i].stack != NULL) {
            // Ajouter les valeurs additive
            finalStats.health.additive += c->equipement[i].stack->definition->stats.health.additive;
            finalStats.healthMax.additive += c->equipement[i].stack->definition->stats.healthMax.additive;
            finalStats.mana.additive += c->equipement[i].stack->definition->stats.mana.additive;
            finalStats.manaMax.additive += c->equipement[i].stack->definition->stats.manaMax.additive;
            finalStats.attack.additive += c->equipement[i].stack->definition->stats.attack.additive;
            finalStats.defense.additive += c->equipement[i].stack->definition->stats.defense.additive;
            finalStats.speed.additive += c->equipement[i].stack->definition->stats.speed.additive;

            // Appliquer les multiplicateurs
            finalStats.health.multiplicative *= c->equipement[i].stack->definition->stats.health.multiplicative;
            finalStats.healthMax.multiplicative *= c->equipement[i].stack->definition->stats.healthMax.multiplicative;
            finalStats.mana.multiplicative *= c->equipement[i].stack->definition->stats.mana.multiplicative;
            finalStats.manaMax.multiplicative *= c->equipement[i].stack->definition->stats.manaMax.multiplicative;
            finalStats.attack.multiplicative *= c->equipement[i].stack->definition->stats.attack.multiplicative;
            finalStats.defense.multiplicative *= c->equipement[i].stack->definition->stats.defense.multiplicative;
            finalStats.speed.multiplicative *= c->equipement[i].stack->definition->stats.speed.multiplicative;
        }
    }

    // Appliquer les multiplicatifs sur les valeurs additives pour calculer les calculatedStats
    c->calculatedStats.health.additive = (c->baseStats.health.additive + finalStats.health.additive) * finalStats.health.multiplicative;
    c->calculatedStats.healthMax.additive = (c->baseStats.healthMax.additive + finalStats.healthMax.additive) * finalStats.healthMax.multiplicative;
    c->calculatedStats.mana.additive = (c->baseStats.mana.additive + finalStats.mana.additive) * finalStats.mana.multiplicative;
    c->calculatedStats.manaMax.additive = (c->baseStats.manaMax.additive + finalStats.manaMax.additive) * finalStats.manaMax.multiplicative;
    c->calculatedStats.attack.additive = (c->baseStats.attack.additive + finalStats.attack.additive) * finalStats.attack.multiplicative;
    c->calculatedStats.defense.additive = (c->baseStats.defense.additive + finalStats.defense.additive) * finalStats.defense.multiplicative;
    c->calculatedStats.speed.additive = (c->baseStats.speed.additive + finalStats.speed.additive) * finalStats.speed.multiplicative;
}

void equiperEquipement(t_character** c, int inventoryIndex, equipementSlotType slot) {
    int i;

    t_itemsStack* itemStack = (t_itemsStack*)getObject((*c)->inventaire->itemsStack, inventoryIndex);
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
        (*c)->equipement[slot].stack = itemStack;
    else
        printf("Slot invalide\n");
    if ((*c)->equipement[slot].stack->definition->onEquip != NULL)
        callFonction((*c)->equipement[slot].stack->definition->onEquip);
    equipementRecalculerStats(*c);
}

void desequiperEquipement(t_character** c, equipementSlotType slot) {
    (*c)->equipement[slot].stack = NULL;
    if ((*c)->equipement[slot].stack->definition->onDeEquip != NULL)
        callFonction((*c)->equipement[slot].stack->definition->onDeEquip);
    equipementRecalculerStats(*c);
}

void equipementUse(t_character* c, equipementSlotType slot) {
    if (c->equipement[slot].stack->definition->onUse != NULL)
        callFonction(c->equipement[slot].stack->definition->onUse);
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

void charactereFree(t_character* c) {
    itemFree(c->inventaire);
    free(c);
}

void inventory_print(t_inventaire* inv) {
    printf("Inventory \n");
    for (int i = 0; i < inv->itemsStack->count; i++) {
        t_itemsStack* Satck = (t_itemsStack*)getObject(inv->itemsStack, i);
        printf("Quantité : %d\n", Satck->quantite);

        printf("Id : %d\n", Satck->definition->id);

        printf("Name : %s\n", Satck->definition->name);

        printf("Health additive : %.2f\n", Satck->definition->stats.health.additive);
        printf("Health multiplicative : %.2f\n", Satck->definition->stats.health.multiplicative);

        printf("Health Max additive : %.2f\n", Satck->definition->stats.healthMax.additive);
        printf("Health Max multiplicative : %.2f\n", Satck->definition->stats.healthMax.multiplicative);

        printf("Mana additive : %.2f\n", Satck->definition->stats.mana.additive);
        printf("Mana multiplicative : %.2f\n", Satck->definition->stats.mana.multiplicative);

        printf("Mana Max additive : %.2f\n", Satck->definition->stats.manaMax.additive);
        printf("Mana Max multiplicative : %.2f\n", Satck->definition->stats.manaMax.multiplicative);

        printf("Attack additive : %.2f\n", Satck->definition->stats.attack.additive);
        printf("Attack multiplicative : %.2f\n", Satck->definition->stats.attack.multiplicative);

        printf("Defense additive : %.2f\n", Satck->definition->stats.defense.additive);
        printf("Defense multiplicative : %.2f\n", Satck->definition->stats.defense.multiplicative);

        printf("Speed additive : %.2f\n", Satck->definition->stats.speed.additive);
        printf("Speed multiplicative : %.2f\n", Satck->definition->stats.speed.multiplicative);
    }
}

void equipment_print(t_character* c) {
    // Affichage des statistiques de base (baseStats)
    printf("Base Stats - Health additive : %.2f\n", c->baseStats.health.additive);
    printf("Base Stats - Health multiplicative : %.2f\n", c->baseStats.health.multiplicative);

    printf("Base Stats - Health Max additive : %.2f\n", c->baseStats.healthMax.additive);
    printf("Base Stats - Health Max multiplicative : %.2f\n", c->baseStats.healthMax.multiplicative);

    printf("Base Stats - Mana additive : %.2f\n", c->baseStats.mana.additive);
    printf("Base Stats - Mana multiplicative : %.2f\n", c->baseStats.mana.multiplicative);

    printf("Base Stats - Mana Max additive : %.2f\n", c->baseStats.manaMax.additive);
    printf("Base Stats - Mana Max multiplicative : %.2f\n", c->baseStats.manaMax.multiplicative);

    printf("Base Stats - Attack additive : %.2f\n", c->baseStats.attack.additive);
    printf("Base Stats - Attack multiplicative : %.2f\n", c->baseStats.attack.multiplicative);

    printf("Base Stats - Defense additive : %.2f\n", c->baseStats.defense.additive);
    printf("Base Stats - Defense multiplicative : %.2f\n", c->baseStats.defense.multiplicative);

    printf("Base Stats - Speed additive : %.2f\n", c->baseStats.speed.additive);
    printf("Base Stats - Speed multiplicative : %.2f\n", c->baseStats.speed.multiplicative);

    // Affichage des statistiques calculées (calculatedStats)
    printf("Calculated Stats - Health additive : %.2f\n", c->calculatedStats.health.additive);
    printf("Calculated Stats - Health multiplicative : %.2f\n", c->calculatedStats.health.multiplicative);

    printf("Calculated Stats - Health Max additive : %.2f\n", c->calculatedStats.healthMax.additive);
    printf("Calculated Stats - Health Max multiplicative : %.2f\n", c->calculatedStats.healthMax.multiplicative);

    printf("Calculated Stats - Mana additive : %.2f\n", c->calculatedStats.mana.additive);
    printf("Calculated Stats - Mana multiplicative : %.2f\n", c->calculatedStats.mana.multiplicative);

    printf("Calculated Stats - Mana Max additive : %.2f\n", c->calculatedStats.manaMax.additive);
    printf("Calculated Stats - Mana Max multiplicative : %.2f\n", c->calculatedStats.manaMax.multiplicative);

    printf("Calculated Stats - Attack additive : %.2f\n", c->calculatedStats.attack.additive);
    printf("Calculated Stats - Attack multiplicative : %.2f\n", c->calculatedStats.attack.multiplicative);

    printf("Calculated Stats - Defense additive : %.2f\n", c->calculatedStats.defense.additive);
    printf("Calculated Stats - Defense multiplicative : %.2f\n", c->calculatedStats.defense.multiplicative);

    printf("Calculated Stats - Speed additive : %.2f\n", c->calculatedStats.speed.additive);
    printf("Calculated Stats - Speed multiplicative : %.2f\n", c->calculatedStats.speed.multiplicative);

    if (c->inventaire->itemsStack->count != 0)
        inventory_print(c->inventaire);
    else
        printf("Inventaire vide\n");

    // Affichage du niveau du personnage
    printf("Level : %d\n", c->level);

    // Affichage de l'expérience du personnage
    printf("Experience : %d\n", c->experience);

    // Affichage de l'or du personnage
    printf("Gold : %d\n", c->gold);
}
