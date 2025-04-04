#include "equipementsManager.h"

#include "../../player.h"

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
        t_blockData* block = createNewBlock();

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

        sprintf(value, "%f", item[i]->stats.healthMax.additive);
        data = createPairData("healthMax Add Modifier", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->stats.healthMax.multiplicative);
        data = createPairData("healthMax Mult Modifier", value);
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

        sprintf(value, "%f", item[i]->arme->mass);
        data = createPairData("mass", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->arme->damage);
        data = createPairData("damage", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->arme->range);
        data = createPairData("range", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->arme->angleAttack);
        data = createPairData("angleAttack", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->arme->attackDuration);
        data = createPairData("attackDuration", value);
        addPairData(block, data);

        sprintf(value, "%f", item[i]->arme->attackCooldown);
        data = createPairData("attackCooldown", value);
        addPairData(block, data);

        sprintf(value, "%d", item[i]->indiceTexture);
        data = createPairData("texture", value);
        addPairData(block, data);

        if (item[i]->validSlot[0] == SLOT_ARME)
            data = createPairData("type", "arme");
        else if (item[i]->validSlot[0] == SLOT_ARMURE)
            data = createPairData("type", "armure");
        else if (item[i]->validSlot[0] == SLOT_ACTIVABLE1)
            data = createPairData("type", "activable");
        else if (item[i]->validSlot[0] == SLOT_TALISMENT)
            data = createPairData("type", "talisment");
        addPairData(block, data);

        if (item[i]->arme->onEquipe == NULL)
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

t_item** item_load(t_fichier* fichier, t_tileset* tileset, t_joueur* player) {
    t_item** item = (t_item**)malloc(fichier->blockManager->count * sizeof(t_item*));
    if (item == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    int resultInt;
    float resultFLOAT;
    char resultChar[200];
    t_blockData* block;

    for (int i = 0; i < fichier->blockManager->count; i++) {
        item[i] = (t_item*)malloc(sizeof(t_item));

        item[i]->arme = malloc(sizeof(t_arme));

        block = (t_blockData*)getObject(fichier->blockManager, i);

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

        getValue(block, "healthMax Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.healthMax.additive = resultFLOAT;

        getValue(block, "healthMax Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.healthMax.multiplicative = resultFLOAT;

        getValue(block, "speed Add Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.speed.additive = resultFLOAT;

        getValue(block, "speed Mult Modifier", &resultFLOAT, FLOAT);
        item[i]->stats.speed.multiplicative = resultFLOAT;

        getValue(block, "flags", &resultInt, INT);
        item[i]->flags = resultInt;

        getValue(block, "texture", &resultInt, INT);
        item[i]->texture = (SDL_Texture*)getObject(tileset->textureTiles, resultInt);
        item[i]->indiceTexture = resultInt;
        item[i]->arme->texture = item[i]->texture;
        item[i]->arme->displayRect = (SDL_Rect){0, 0, 16, 16};

        getValue(block, "mass", &resultFLOAT, FLOAT);
        item[i]->arme->mass = resultFLOAT;

        getValue(block, "damage", &resultFLOAT, FLOAT);
        item[i]->arme->damage = resultFLOAT;

        getValue(block, "range", &resultFLOAT, FLOAT);
        item[i]->arme->range = resultFLOAT;

        getValue(block, "angleAttack", &resultFLOAT, FLOAT);
        item[i]->arme->angleAttack = resultFLOAT;

        getValue(block, "attackDuration", &resultFLOAT, FLOAT);
        item[i]->arme->attackDuration = resultFLOAT;

        getValue(block, "attackCooldown", &resultFLOAT, FLOAT);
        item[i]->arme->attackCooldown = resultFLOAT;

        item[i]->arme->onEquipe = NULL;

        getValue(block, "type", &resultChar, STRING);
        if (strcmp(resultChar, "arme") == 0) {
            item[i]->validSlot[0] = SLOT_ARME;
            getValue(block, "OnEquipe", &resultInt, INT);
            if (resultInt == 0)
                item[i]->arme->onEquipe = NULL;
            else {
                item[i]->arme->onEquipe = creerFonction(peutEquiperWrapper, FONCTION_PARAMS(&player->currentWeapon, item[i]->arme));
            }
        } else if (strcmp(
            resultChar, "armure") == 0)
            item[i]->validSlot[0] = SLOT_ARMURE;
        else if (strcmp(resultChar, "activable") == 0)
            item[i]->validSlot[0] = SLOT_ACTIVABLE1;
        else if (strcmp(resultChar, "talisment") == 0)
            item[i]->validSlot[0] = SLOT_TALISMENT;

        item[i]->arme->indice = i;

        getValue(block, "OnDeEquipe", &resultInt, INT);
        if (resultInt == 0)
            item[i]->onDeEquip = NULL;

        getValue(block, "rarity", resultChar, STRING);
        if (strcmp(resultChar, "uncommon") == 0)
            item[i]->rarity = RARITY_UNCOMMON;
        else if (strcmp(resultChar, "rare") == 0)
            item[i]->rarity = RARITY_RARE;
        else if (strcmp(resultChar, "epic") == 0)
            item[i]->rarity = RARITY_EPIC;
        else if (strcmp(resultChar, "legendary") == 0)
            item[i]->rarity = RARITY_LEGENDARY;
        else if (strcmp(resultChar, "common") == 0)
            item[i]->rarity = RARITY_COMMON;
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
    t_blockData* block = createNewBlock();

    sprintf(value, "%d", inv->itemsStack->count);
    data = createPairData("nbItems", value);
    addPairData(block, data);
    addBlock(fichier, block);

    for (int i = 0; i < inv->itemsStack->count; i++) {
        char value[50];

        t_pairData* data;
        t_blockData* block = createNewBlock();

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
    t_blockData* block;

    for (int i = 1; i < fichier->blockManager->count; i++) {
        block = (t_blockData*)getObject(fichier->blockManager, i);

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

t_item* getItemByRarity(t_item** items, int count, t_itemRarity rarity) {
    int* indices = malloc(count * sizeof(int));
    if (!indices) return NULL;
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (items[i]->rarity == rarity)
            indices[found++] = i;
    }
    if (found == 0) {
        free(indices);
        return NULL;
    }
    int chosen = indices[rand() % found];
    free(indices);
    return items[chosen];
}

t_itemRarity determineRarityByPercentage() {
    int chance = rand() % 100;

    if (chance < 60)
        return RARITY_COMMON;
    else if (chance < 85)
        return RARITY_UNCOMMON;
    else if (chance < 95)
        return RARITY_RARE;
    else if (chance < 99)
        return RARITY_EPIC;
    else
        return RARITY_LEGENDARY;
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

void equipementRecalculerStats(t_joueur** c) {
    // Initialisation des valeurs finales
    t_stats finalStats = {{0, 1}, {0, 1}, {0, 1}, {0, 1}};

    // Parcours de tous les slots d'équipement
    for (int i = 0; i < TOTAL_EQUIPMENT_SLOTS; i++) {
        if ((*c)->equipement[i].stack != NULL) {
            // Ajouter les valeurs additive
            finalStats.healthMax.additive += (*c)->equipement[i].stack->definition->stats.healthMax.additive;
            finalStats.attack.additive += (*c)->equipement[i].stack->definition->stats.attack.additive;
            finalStats.defense.additive += (*c)->equipement[i].stack->definition->stats.defense.additive;
            finalStats.speed.additive += (*c)->equipement[i].stack->definition->stats.speed.additive;

            // Appliquer les multiplicateurs
            finalStats.healthMax.multiplicative *= (*c)->equipement[i].stack->definition->stats.healthMax.multiplicative;
            finalStats.attack.multiplicative *= (*c)->equipement[i].stack->definition->stats.attack.multiplicative;
            finalStats.defense.multiplicative *= (*c)->equipement[i].stack->definition->stats.defense.multiplicative;
            finalStats.speed.multiplicative *= (*c)->equipement[i].stack->definition->stats.speed.multiplicative;
        }
    }

    // Appliquer les multiplicatifs sur les valeurs additives pour calculer les calculatedStats
    (*c)->calculatedStats.healthMax.additive = ((*c)->baseStats.healthMax.additive + finalStats.healthMax.additive) * finalStats.healthMax.multiplicative;
    (*c)->calculatedStats.attack.additive = ((*c)->baseStats.attack.additive + finalStats.attack.additive) * finalStats.attack.multiplicative;
    (*c)->calculatedStats.defense.additive = ((*c)->baseStats.defense.additive + finalStats.defense.additive) * finalStats.defense.multiplicative;
    (*c)->calculatedStats.speed.additive = ((*c)->baseStats.speed.additive + finalStats.speed.additive) * finalStats.speed.multiplicative;
}

void equiperEquipement(t_joueur** c, int inventoryIndex, equipementSlotType slot) {
    int i;

    printf("SLOT : %d \n", slot);

    t_itemsStack* itemStack = (t_itemsStack*)getObject((*c)->inventaire->itemsStack, inventoryIndex);
    if (itemStack == NULL || itemStack->definition == NULL) {
        fprintf(stderr, "Erreur: ItemStack ou definition invalide.\n");
        return;
    }
    for (i = 0; i < 2; i++) {
        if (slot == itemStack->definition->validSlot[i])
            break;
    }
    if (slot == itemStack->definition->validSlot[i]) {
        (*c)->equipement[slot].stack = itemStack;

    } else {
        printf("Slot invalide\n");
    }

    if ((*c)->equipement[slot].stack->definition->arme->onEquipe != NULL) {
        callFonction((*c)->equipement[slot].stack->definition->arme->onEquipe);
    }

    equipementRecalculerStats(c);
}

void peutEquiper(t_arme** arme, t_arme* armeAjout) {
    arme[0] = armeAjout;
}
void peutEquiperWrapper(t_fonctionParam* f) {
    peutEquiper(GET_PTR(f, 0, t_arme**), GET_PTR(f, 1, t_arme*));
}

void desequiperEquipement(t_joueur** c, equipementSlotType slot) {
    (*c)->equipement[slot].stack = NULL;
    if ((*c)->equipement[slot].stack->definition->onDeEquip != NULL)
        callFonction((*c)->equipement[slot].stack->definition->onDeEquip);
    equipementRecalculerStats(c);
}

void equipementUse(t_joueur* c, equipementSlotType slot) {
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

void inventory_print(t_inventaire* inv) {
    printf("Inventory \n");
    for (int i = 0; i < inv->itemsStack->count; i++) {
        t_itemsStack* Satck = (t_itemsStack*)getObject(inv->itemsStack, i);
        printf("Quantité : %d\n", Satck->quantite);

        printf("Id : %d\n", Satck->definition->id);

        printf("Name : %s\n", Satck->definition->name);

        printf("Health Max additive : %.2f\n", Satck->definition->stats.healthMax.additive);
        printf("Health Max multiplicative : %.2f\n", Satck->definition->stats.healthMax.multiplicative);

        printf("Attack additive : %.2f\n", Satck->definition->stats.attack.additive);
        printf("Attack multiplicative : %.2f\n", Satck->definition->stats.attack.multiplicative);

        printf("Defense additive : %.2f\n", Satck->definition->stats.defense.additive);
        printf("Defense multiplicative : %.2f\n", Satck->definition->stats.defense.multiplicative);

        printf("Speed additive : %.2f\n", Satck->definition->stats.speed.additive);
        printf("Speed multiplicative : %.2f\n", Satck->definition->stats.speed.multiplicative);
    }
}

void equipment_print(t_joueur* c) {
    // Affichage des statistiques de base (baseStats)

    printf("Base Stats - Health Max additive : %.2f\n", c->baseStats.healthMax.additive);
    printf("Base Stats - Health Max multiplicative : %.2f\n", c->baseStats.healthMax.multiplicative);

    printf("Base Stats - Attack additive : %.2f\n", c->baseStats.attack.additive);
    printf("Base Stats - Attack multiplicative : %.2f\n", c->baseStats.attack.multiplicative);

    printf("Base Stats - Defense additive : %.2f\n", c->baseStats.defense.additive);
    printf("Base Stats - Defense multiplicative : %.2f\n", c->baseStats.defense.multiplicative);

    printf("Base Stats - Speed additive : %.2f\n", c->baseStats.speed.additive);
    printf("Base Stats - Speed multiplicative : %.2f\n", c->baseStats.speed.multiplicative);

    // Affichage des statistiques calculées (calculatedStats)

    printf("Calculated Stats - Health Max additive : %.2f\n", c->calculatedStats.healthMax.additive);
    printf("Calculated Stats - Health Max multiplicative : %.2f\n", c->calculatedStats.healthMax.multiplicative);

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
    printf("Experience : %d\n", c->xp);

    // Affichage de l'or du personnage
    printf("Gold : %d\n", c->gold);
}

int findItemInventoryIndex(t_inventaire* inv, t_item* item) {
    for (int i = 0; i < inv->itemsStack->count; i++) {
        t_itemsStack* stack = (t_itemsStack*)getObject(inv->itemsStack, i);
        if (stack && stack->definition == item) {
            return i;
        }
    }
    return -1;  // Item not found
}