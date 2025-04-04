#include "affichage.h"

void calculCasePlayer(SDL_Rect *casePlayer, t_input *input, char *nom) {
    if (strcmp(nom, "Perso") == 0) {
        casePlayer->h = input->windowWidth * 0.14;
        casePlayer->w = input->windowWidth * 0.16;
        casePlayer->x = input->windowWidth * 0.02;
        casePlayer->y = input->windowHeight * 0.08;
    }
}
void calculCaseSlots(SDL_Rect *comp, SDL_Rect *slot, t_input *input, char *nom1, char *nom2) {
    if (!strcmp(nom2, "arme") && !strcmp(nom1, "Perso")) {
        slot->h = comp->h / 2 - comp->h * 0.05;
        slot->w = comp->w / 2;
        slot->x = comp->w + comp->x + input->windowWidth * 0.04;
        slot->y = comp->y;
    }
    if (!strcmp(nom2, "armure") && !strcmp(nom1, "arme")) {
        slot->h = comp->h;
        slot->w = comp->w;
        slot->x = comp->x;
        slot->y = comp->h + input->windowHeight * 0.08 + input->windowWidth * 0.13 * 0.1;
    }

    if (!strcmp(nom2, "activable1") && !strcmp(nom1, "Perso")) {
        slot->h = comp->h / 2;
        slot->w = comp->w / 2 - comp->w * 0.05;
        slot->x = comp->x;
        slot->y = comp->h + comp->y + input->windowHeight * 0.04;
    }

    if (!strcmp(nom2, "activable2") && !strcmp(nom1, "activable1")) {
        slot->h = comp->h;
        slot->w = comp->w;
        slot->x = comp->w + input->windowWidth * 0.02 + input->windowWidth * 0.185 * 0.1;
        slot->y = comp->y;
    }
}

void calculDescrStatsPlayer(SDL_Rect *slotDroit, SDL_Rect *slotBas, SDL_Rect *casePlayer, SDL_Rect *caseStatsPlayer, t_input *input) {
    caseStatsPlayer->h = casePlayer->h + slotBas->h;
    caseStatsPlayer->w = casePlayer->w + slotDroit->w + input->windowWidth * 0.04;
    caseStatsPlayer->x = casePlayer->x;
    caseStatsPlayer->y = slotBas->y + slotBas->h + input->windowHeight * 0.04;
}

void calculInventaire(SDL_Rect *inv, SDL_Rect *statsPlayer, t_input *input) {
    inv->h = statsPlayer->h * 2 + input->windowHeight * 0.08;
    inv->w = statsPlayer->w * 1.5;
    inv->x = statsPlayer->w + input->windowWidth * 0.04;
    inv->y = input->windowHeight * 0.08;
}

void calculStatsItem(SDL_Rect *inv, SDL_Rect *statsItem, t_input *input) {
    statsItem->h = inv->h / 2 - input->windowHeight * 0.01;
    statsItem->w = inv->w / 2.5;
    statsItem->x = inv->x + inv->w + input->windowWidth * 0.04;
    statsItem->y = inv->y;
}

void caculDescrItem(SDL_Rect *statsItem, SDL_Rect *descrItem, t_input *input) {
    descrItem->h = statsItem->h * 2 / 3 - input->windowHeight * 0.01;
    descrItem->w = statsItem->w;
    descrItem->x = statsItem->x;
    descrItem->y = statsItem->h + statsItem->y + input->windowHeight * 0.03;
}

void calculEquiper(SDL_Rect *statsItem, SDL_Rect *descrItem, SDL_Rect *Equiper, t_input *input) {
    Equiper->h = statsItem->h / 3 - input->windowHeight * 0.03;
    Equiper->w = descrItem->w;
    Equiper->x = descrItem->x;
    Equiper->y = descrItem->y + descrItem->h + input->windowHeight * 0.03;
}

// Ajouter une fonction de nettoyage des slots
void freeInventorySlots(InventoryUI *ui) {
    if (ui->elems && ui->elems->inventory_slots) {
        for (int i = 0; i < ui->nbItems; i++) {
            if (ui->elems->inventory_slots[i].button) {
                freeButton(ui->elems->inventory_slots[i].button);
                ui->elems->inventory_slots[i].button = NULL;
            }
        }
        free(ui->elems->inventory_slots);
        ui->elems->inventory_slots = NULL;
    }
}

char *createStatLine(const char *statLabel, float additive, float multiplicative) {
    char buffer[256] = {0};
    char addPart[32] = {0};
    char multPart[32] = {0};
    int hasContent = 0;

    // Partie additive
    if (additive != 0) {
        sprintf(addPart, "%+.0f", additive);
        hasContent = 1;
    }

    // Partie multiplicative
    if (multiplicative != 1.0f && multiplicative != 0) {
        float percentage = (multiplicative);
        sprintf(multPart, "%+.1f%%", percentage);
        hasContent = 1;
    }

    // Construction de la ligne
    if (hasContent) {
        if (addPart[0] && multPart[0]) {
            sprintf(buffer, "%s %s %s", statLabel, addPart, multPart);
        } else if (addPart[0]) {
            sprintf(buffer, "%s %s", statLabel, addPart);
        } else {
            sprintf(buffer, "%s %s", statLabel, multPart);
        }
    } else {
        sprintf(buffer, "%s 0", statLabel);
    }

    return strdup(buffer);
}

void calculerItem(SDL_Rect *item, SDL_Rect inv, SDL_Rect *comp, int nb, int ind, t_input *input) {
    item->h = inv.h / 6;
    item->w = item->h;
    if (ind != 0 && ind != 4)
        item->x = comp->x + comp->w + input->windowWidth * 0.02;
    else
        item->x = inv.x + input->windowWidth * 0.02;
    if (ind == 0)
        item->y = inv.y + input->windowHeight * 0.016;
    else if (ind == 4)
        item->y = comp->y + comp->h + input->windowHeight * 0.02;
    else
        item->y = comp->y;
}

void afficherText(SDL_Renderer *renderer, t_text *txt1, t_text *txt2, t_input *input) {
    txt2->rect.x = txt1->rect.x;
    txt2->rect.y = txt1->rect.y + input->windowWidth * 0.03;
    renderText(renderer, txt2);
}

void equiperSlot(InventoryUI *ui, t_item **item, SDL_Renderer *renderer, t_input *input) {
    if (!ui->peutEquiper || (*item) == NULL || item == NULL)
        return;

    printf("DEBUG: Trying to equip item: %s (ID: %d)\n", (*item)->name, (*item)->id);

    // Find the actual inventory index
    int actualIndex = findItemInventoryIndex(ui->ext->character->inventaire, *item);
    printf("DEBUG: Item ID: %d, actual inventory index: %d\n", (*item)->id, actualIndex);

    if (actualIndex < 0) {
        printf("ERROR: Item not found in inventory!\n");
        return;
    }
    if (ui->peutEquiper) {
        if ((*item)->validSlot[0] == SLOT_ACTIVABLE1) {
            ui->elems->CaseActivable1.texture = (*item)->texture;
        }
        if ((*item)->validSlot[0] == SLOT_TALISMENT) {
            ui->elems->caseActivable2.texture = (*item)->texture;
        }
        if ((*item)->validSlot[0] == SLOT_ARME) {
            ui->elems->caseArme.texture = (*item)->texture;
        }
        if ((*item)->validSlot[0] == SLOT_ARMURE) {
            ui->elems->caseArmure.texture = (*item)->texture;
        }
    }
    ui->peutEquiper = 0;

    printf("DEBUG: Using actual inventory index: %d\n", actualIndex);
    printf("DEBUG: Using arme->indice (previus): %d\n", (*ui).ext->character->currentWeapon->indice);
    equiperEquipement(&ui->ext->character, actualIndex, (*item)->validSlot[0]);
    printf("DEBUG: Using arme->indice (now): %d\n", (*ui).ext->character->currentWeapon->indice);

    // Remplacer les références à baseStats par calculatedStats
    ui->ecrit->nom_txt_player[0] = createStatLine("Health Max : ", ui->ext->character->calculatedStats.healthMax.additive, ui->ext->character->calculatedStats.healthMax.multiplicative);
    ui->ecrit->nom_txt_player[1] = createStatLine("Attack : ", ui->ext->character->calculatedStats.attack.additive, ui->ext->character->calculatedStats.attack.multiplicative);
    ui->ecrit->nom_txt_player[2] = createStatLine("Defense : ", ui->ext->character->calculatedStats.defense.additive, ui->ext->character->calculatedStats.defense.multiplicative);
    ui->ecrit->nom_txt_player[3] = createStatLine("Speed : ", ui->ext->character->calculatedStats.speed.additive, ui->ext->character->calculatedStats.speed.multiplicative);

    for (int i = 0; i < 4; i++) {
        ui->ecrit->text_player[i] = createText(renderer, ui->ecrit->nom_txt_player[i], ui->ext->item_font, ui->ecrit->color_txt);
    }
    ui->ecrit->text_player[0]->rect.x = ui->elems->statsPlayer.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_player[0]->rect.y = ui->elems->statsPlayer.rect.y + input->windowHeight * 0.01;
}

void equiperSlotWrapper(t_fonctionParam *f) {
    equiperSlot(GET_PTR(f, 0, InventoryUI *), GET_PTR(f, 1, t_item **), GET_PTR(f, 2, SDL_Renderer *), GET_PTR(f, 3, t_input *));
}

InventoryUI *inventoryUI_Init(InventoryUI *ui2, SDL_Renderer *renderer, int nb, t_joueur *c, t_input *input) {
    InventoryUI *ui = malloc(sizeof(InventoryUI));
    if (ui2 != NULL) {
        ui->ext = ui2->ext;
        ui->ecrit = ui2->ecrit;
    }

    t_itemsStack **items = malloc(sizeof(t_itemsStack *) * nb);
    for (int i = 0; i < c->inventaire->itemsStack->count; i++) {
        items[i] = (t_itemsStack *)getObject(c->inventaire->itemsStack, i);
    }

    ui->ext = malloc(sizeof(t_extern));
    ui->ext->item_font = NULL;
    ui->ext->descr_font = NULL;
    ui->ext->character = malloc(sizeof(t_joueur));

    ui->ecrit = malloc(sizeof(t_ecritures));
    // Initialiser les membres si nécessaire
    memset(ui->ecrit->nom_txt_item, 0, sizeof(ui->ecrit->nom_txt_item));
    // etc.
    if (ui2 != NULL)
        free(ui->elems);

    ui->elems = malloc(sizeof(t_elements));

    if (ui2 == NULL)
        ui->itemclique = NULL;
    // Initialisation des références
    ui->ext->character = c;
    ui->nbItems = ui->ext->character->inventaire->itemsStack->count;

    ui->ext->item_font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", ui->elems->statsPlayer.rect.h * ui->elems->statsPlayer.rect.w * 0.0002);
    // Pour les items
    ui->ecrit->nom_txt_item[0] = createStatLine("Health Max : ", 0, 0);
    ui->ecrit->nom_txt_item[1] = createStatLine("Attack : ", 0, 0);
    ui->ecrit->nom_txt_item[2] = createStatLine("Defense : ", 0, 0);
    ui->ecrit->nom_txt_item[3] = createStatLine("Speed : ", 0, 0);

    // Pour le joueur (si les stats sont uniquement additives)
    ui->ecrit->nom_txt_player[0] = createStatLine("Health Max : ", c->baseStats.healthMax.additive, 1.0f);
    ui->ecrit->nom_txt_player[1] = createStatLine("Attack : ", c->baseStats.attack.additive, 1.0f);
    ui->ecrit->nom_txt_player[2] = createStatLine("Defense : ", c->baseStats.defense.additive, 1.0f);
    ui->ecrit->nom_txt_player[3] = createStatLine("Speed : ", c->baseStats.speed.additive, 1.0f);

    ui->elems->CaseActivable1.texture = NULL;
    ui->elems->caseActivable2.texture = NULL;
    ui->elems->caseArme.texture = NULL;
    ui->elems->caseArmure.texture = NULL;

    // Calculs initiaux (votre code original)

    calculCasePlayer(&ui->elems->player_panel.rect, input, "Perso");
    ui->elems->player_panel.texture = c->entity.texture;

    calculCaseSlots(&ui->elems->player_panel.rect, &ui->elems->caseArme.rect, input, "Perso", "arme");
    calculCaseSlots(&ui->elems->caseArme.rect, &ui->elems->caseArmure.rect, input, "arme", "armure");
    calculCaseSlots(&ui->elems->player_panel.rect, &ui->elems->CaseActivable1.rect, input, "Perso", "activable1");
    calculCaseSlots(&ui->elems->CaseActivable1.rect, &ui->elems->caseActivable2.rect, input, "activable1", "activable2");

    calculDescrStatsPlayer(&ui->elems->caseArme.rect, &ui->elems->CaseActivable1.rect, &ui->elems->player_panel.rect, &ui->elems->statsPlayer.rect, input);
    calculInventaire(&ui->elems->inventory_panel.rect, &ui->elems->statsPlayer.rect, input);
    calculStatsItem(&ui->elems->inventory_panel.rect, &ui->elems->statsItem.rect, input);
    caculDescrItem(&ui->elems->statsItem.rect, &ui->elems->descrItem.rect, input);
    calculEquiper(&ui->elems->statsItem.rect, &ui->elems->descrItem.rect, &ui->elems->equiper.rect, input);

    // Initialisation slots (votre code original)
    ui->elems->inventory_slots = malloc(ui->nbItems * sizeof(UI_Element));
    calculerItem(&ui->elems->inventory_slots[0].rect, ui->elems->inventory_panel.rect,
                 &ui->elems->inventory_panel.rect, 0, 0, input);
    if (ui2 == NULL) {
        ui->elems->inventory_slots[0].button = createButton(createText(renderer, " ", ui->ext->item_font, (SDL_Color){255, 255, 255}), (SDL_Color){255, 128, 0}, (SDL_Color){255, 69, 0}, ui->elems->inventory_slots[0].rect, creerFonction(creerDescrWrapper, FONCTION_PARAMS(ui, items[0]->definition, renderer, input)));
        ui->elems->inventory_slots[0].texture = items[0]->definition->texture;
    } else {
        ui->elems->inventory_slots[0].button = ui2->elems->inventory_slots[0].button;
        ui->elems->inventory_slots[0].button->rect = ui->elems->inventory_slots[0].rect;
    }
    int i, j;
    for (i = 1, j = 1; i < ui->nbItems; j++, i++) {
        calculerItem(&ui->elems->inventory_slots[i].rect, ui->elems->inventory_panel.rect, &ui->elems->inventory_slots[i - 1].rect, i, j, input);
        if (ui2 == NULL) {
            ui->elems->inventory_slots[i].button = createButton(createText(renderer, " ", ui->ext->item_font, (SDL_Color){255, 255, 255}), (SDL_Color){255, 128, 0}, (SDL_Color){255, 69, 0}, ui->elems->inventory_slots[i].rect, creerFonction(creerDescrWrapper, FONCTION_PARAMS(ui, items[i]->definition, renderer, input)));
            ui->elems->inventory_slots[i].texture = items[i]->definition->texture;
        } else {
            ui->elems->inventory_slots[i].button = ui2->elems->inventory_slots[i].button;
            ui->elems->inventory_slots[i].button->rect = ui->elems->inventory_slots[i].rect;
        }
        if (j == 4)
            j = 0;
    }
    ui->j = j;
    // ui->elems->caseArme.texture = c->currentWeapon->texture;
    int totalContentHeight = 0;
    for (int i = 0; i < ui->nbItems; i++) {
        int slotBottom = ui->elems->inventory_slots[i].rect.y + ui->elems->inventory_slots[i].rect.h;
        if (slotBottom > totalContentHeight) {
            totalContentHeight = slotBottom;
        }
    }
    totalContentHeight -= ui->elems->inventory_panel.rect.y;
    ui->maxScrollY = totalContentHeight - ui->elems->inventory_panel.rect.h;
    if (ui->maxScrollY < 0) ui->maxScrollY = 0;
    ui->scrollY = 0;

    // Police

    ui->color.a = 255;
    ui->color.b = 255;
    ui->color.g = 0;
    ui->color.r = 0;

    // affichage des texts

    ui->ext->item_font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", ui->elems->statsPlayer.rect.h * ui->elems->statsPlayer.rect.w * 0.0002);
    ui->ext->descr_font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", ui->elems->descrItem.rect.h * ui->elems->descrItem.rect.w * 0.0003);
    ui->ecrit->color_txt.a = 255;
    ui->ecrit->color_txt.b = 0;
    ui->ecrit->color_txt.g = 128;
    ui->ecrit->color_txt.r = 255;

    if (ui2 == NULL) {
        t_text *t = createText(renderer, "EQUIPER", ui->ext->item_font, (SDL_Color){0, 0, 0});
        ui->elems->equiper.button = createButton(t, (SDL_Color){255, 128, 0}, (SDL_Color){255, 150, 0}, ui->elems->equiper.rect, creerFonction(equiperSlotWrapper, FONCTION_PARAMS(ui, &ui->itemclique, renderer, input)));
    } else {
        ui->elems->equiper.button = ui2->elems->equiper.button;
        ui->elems->equiper.button->rect = ui->elems->equiper.rect;
    }
    // Initialisation des textes statsPlayer
    for (int i = 0; i < 4; i++) {
        ui->ecrit->text_player[i] = createText(renderer, ui->ecrit->nom_txt_player[i], ui->ext->item_font, ui->ecrit->color_txt);
    }

    // Positionnement et affichage des textes

    ui->ecrit->text_player[0]->rect.x = ui->elems->statsPlayer.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_player[0]->rect.y = ui->elems->statsPlayer.rect.y + input->windowHeight * 0.01;

    // Initialisation des textes statsItem

    for (int i = 0; i < 4; i++) {
        ui->ecrit->text_item[i] = createText(renderer, ui->ecrit->nom_txt_item[i], ui->ext->descr_font, ui->ecrit->color_txt);
    }

    // Positionnement et affichage des textes
    ui->ecrit->text_item[0]->rect.x = ui->elems->statsItem.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_item[0]->rect.y = ui->elems->statsItem.rect.y + input->windowHeight * 0.01;
    ui->ecrit->text_descr = createText(renderer, "Description :", ui->ext->descr_font, ui->ecrit->color_txt);

    ui->ecrit->text_descr->rect.x = ui->elems->descrItem.rect.x + ui->elems->descrItem.rect.x * 0.025;
    ui->ecrit->text_descr->rect.y = ui->elems->descrItem.rect.y + input->windowHeight * 0.005;

    ui2 = ui;

    return ui;
}

void afficherStatPlayer(SDL_Renderer *renderer, InventoryUI *ui, t_input *input) {
    renderText(renderer, ui->ecrit->text_player[0]);

    for (int i = 1; i < 4; i++) {
        afficherText(renderer, ui->ecrit->text_player[i - 1], ui->ecrit->text_player[i], input);
    }
}

void afficherStatItem(SDL_Renderer *renderer, InventoryUI *ui, t_input *input) {
    renderText(renderer, ui->ecrit->text_item[0]);
    for (int i = 1; i < 4; i++) {
        afficherText(renderer, ui->ecrit->text_item[i - 1], ui->ecrit->text_item[i], input);
    }
}

void afficherDescription(SDL_Renderer *renderer, InventoryUI *ui) {
    renderText(renderer, ui->ecrit->text_descr);

    for (int i = 0; i < ui->ecrit->count_descr; i++) {
        renderText(renderer, ui->ecrit->description[i]);
    }
}

void updateAjoutObjet(InventoryUI *ui, SDL_Renderer *renderer, t_input *input) {
    if (ui->nbItems < ui->ext->character->inventaire->itemsStack->count) {
        int newIndex = ui->nbItems;  // Index du nouvel item AVANT incrément
        ui->nbItems++;

        // Reallocate the inventory slots array safely
        UI_Element *newSlots = realloc(ui->elems->inventory_slots, ui->nbItems * sizeof(UI_Element));
        if (newSlots == NULL) {
            // Handle allocation failure
            ui->nbItems--;  // Roll back the counter
            return;
        }
        ui->elems->inventory_slots = newSlots;

        // Calculate item position based on previous item or starting point
        SDL_Rect *refRect = (newIndex == 0) ? &ui->elems->inventory_panel.rect : &ui->elems->inventory_slots[newIndex - 1].rect;

        calculerItem(&ui->elems->inventory_slots[newIndex].rect,
                     ui->elems->inventory_panel.rect,
                     refRect, newIndex, ui->j, input);

        // Get the item stack and validate it
        t_itemsStack *stack = (t_itemsStack *)getObject(ui->ext->character->inventaire->itemsStack, newIndex);
        if (stack == NULL || stack->definition == NULL) {
            ui->nbItems--;  // Roll back on error
            return;
        }

        ui->elems->inventory_slots[newIndex].button = createButton(
            createText(renderer, " ", ui->ext->item_font, (SDL_Color){255, 255, 255}),
            (SDL_Color){255, 128, 0}, (SDL_Color){255, 69, 0},
            ui->elems->inventory_slots[newIndex].rect,
            creerFonction(creerDescrWrapper, FONCTION_PARAMS(ui, stack->definition, renderer, input)));

        ui->elems->inventory_slots[newIndex].texture = stack->definition->texture;

        ui->j++;
        if (ui->j == 4) ui->j = 0;

        int totalContentHeight = 0;
        for (int i = 0; i < ui->nbItems; i++) {
            int slotBottom = ui->elems->inventory_slots[i].rect.y + ui->elems->inventory_slots[i].rect.h;
            if (slotBottom > totalContentHeight) {
                totalContentHeight = slotBottom;
            }
        }
        
        totalContentHeight -= ui->elems->inventory_panel.rect.y;
        ui->maxScrollY = totalContentHeight - ui->elems->inventory_panel.rect.h;
        if (ui->maxScrollY < 0) ui->maxScrollY = 0;
    }
}

void inventoryUI_Render(InventoryUI *ui, t_context *context) {
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
    SDL_RenderClear(context->renderer);

    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);

    SDL_RenderCopy(context->renderer, ui->elems->player_panel.texture, NULL, &ui->elems->player_panel.rect);
    SDL_RenderDrawRect(context->renderer, &ui->elems->player_panel.rect);

    SDL_RenderDrawRect(context->renderer, &ui->elems->caseArme.rect);
    if (ui->elems->caseArme.texture != NULL)
        SDL_RenderCopy(context->renderer, ui->elems->caseArme.texture, NULL, &ui->elems->caseArme.rect);

    SDL_RenderDrawRect(context->renderer, &ui->elems->caseArmure.rect);
    if (ui->elems->caseArmure.texture != NULL)
        SDL_RenderCopy(context->renderer, ui->elems->caseArmure.texture, NULL, &ui->elems->caseArmure.rect);

    SDL_RenderDrawRect(context->renderer, &ui->elems->CaseActivable1.rect);
    if (ui->elems->CaseActivable1.texture != NULL)
        SDL_RenderCopy(context->renderer, ui->elems->CaseActivable1.texture, NULL, &ui->elems->CaseActivable1.rect);

    SDL_RenderDrawRect(context->renderer, &ui->elems->caseActivable2.rect);
    if (ui->elems->caseActivable2.texture != NULL)
        SDL_RenderCopy(context->renderer, ui->elems->caseActivable2.texture, NULL, &ui->elems->caseActivable2.rect);

    SDL_RenderDrawRect(context->renderer, &ui->elems->statsPlayer.rect);
    afficherStatPlayer(context->renderer, ui, context->input);

    SDL_RenderDrawRect(context->renderer, &ui->elems->inventory_panel.rect);

    SDL_RenderSetClipRect(context->renderer, &ui->elems->inventory_panel.rect);
    for (int i = 0; i < ui->nbItems; i++) {
        SDL_Rect dest = ui->elems->inventory_slots[i].rect;

        dest.y -= ui->scrollY;  // Appliquer le décalage
        ui->elems->inventory_slots[i].button->rect.y = dest.y;
        renderButton(context, ui->elems->inventory_slots[i].button);

        SDL_RenderCopy(context->renderer, ui->elems->inventory_slots[i].texture, NULL, &dest);
        SDL_RenderDrawRect(context->renderer, &dest);
    }
    SDL_RenderSetClipRect(context->renderer, NULL);

    SDL_RenderDrawRect(context->renderer, &ui->elems->statsItem.rect);
    afficherStatItem(context->renderer, ui, context->input);

    SDL_RenderDrawRect(context->renderer, &ui->elems->descrItem.rect);
    afficherDescription(context->renderer, ui);

    SDL_RenderDrawRect(context->renderer, &ui->elems->equiper.rect);
    renderButton(context, ui->elems->equiper.button);
}

void inventoryUI_Update(InventoryUI *ui, t_context *context) {
    // Recalcul si la fenêtre est redimensionnée

    for (int i = 0; i < ui->nbItems; i++) {
        handleInputButton(context, ui->elems->inventory_slots[i].button);
    }
    updateAjoutObjet(ui, context->renderer, context->input);
    handleInputButton(context, ui->elems->equiper.button);
    if (context->input->windowWidth != ui->width || context->input->windowHeight != ui->height) {
        int lastScroll = ui->scrollY;

        // Réinitialiser l'UI
        inventoryUI_Init(ui, context->renderer, ui->nbItems, ui->ext->character, context->input);

        // Restaurer le scroll dans les nouvelles limites
        ui->scrollY = lastScroll;
        if (ui->scrollY > ui->maxScrollY) ui->scrollY = ui->maxScrollY;
    }
}

void creerDescr(InventoryUI *ui, t_item *item, SDL_Renderer *renderer, t_input *input) {
    printf("DEBUG: Selected item: %s (ID: %d)\n", item->name, item->id);

    char processedDesc[256];
    strcpy(processedDesc, item->description);

    // Remplacer les "\\n" par "\n"
    char *found;
    while ((found = strstr(processedDesc, "\\n")) != NULL) {
        *found = '\n';                                         // Remplacer le premier '\' par '\n'
        memmove(found + 1, found + 2, strlen(found + 2) + 1);  // Décaler le reste
    }

    int i = 0, j, nb = 1;
    while (processedDesc[i++] != '\0') {
        for (j = 0; processedDesc[i] != '\n' && processedDesc[i] != '\0'; i++, j++) {
            processedDesc[j] = processedDesc[i];
        }
        processedDesc[j] = '\0';

        ui->ecrit->description[nb - 1] = createText(renderer, processedDesc, ui->ext->descr_font, ui->ecrit->color_txt);
        ui->ecrit->description[nb - 1]->rect.x = ui->elems->descrItem.rect.x + input->windowWidth * 0.005;
        ui->ecrit->description[nb - 1]->rect.y = ui->ecrit->text_descr->rect.y + input->windowHeight * 0.03 * nb;
        nb++;
        ui->ecrit->descr[0] = '\0';
    }
    ui->ecrit->count_descr = nb - 1;

    ui->ecrit->nom_txt_item[0] = createStatLine("Health Max : ", item->stats.healthMax.additive, item->stats.healthMax.multiplicative);
    ui->ecrit->nom_txt_item[1] = createStatLine("Attack : ", item->stats.attack.additive, item->stats.attack.multiplicative);
    ui->ecrit->nom_txt_item[2] = createStatLine("Defense : ", item->stats.defense.additive, item->stats.defense.multiplicative);
    ui->ecrit->nom_txt_item[3] = createStatLine("Speed : ", item->stats.speed.additive, item->stats.speed.multiplicative);

    for (int i = 0; i < 4; i++) {
        ui->ecrit->text_item[i] = createText(renderer, ui->ecrit->nom_txt_item[i], ui->ext->descr_font, ui->ecrit->color_txt);
        ui->ecrit->text_player[i] = createText(renderer, ui->ecrit->nom_txt_player[i], ui->ext->item_font, ui->ecrit->color_txt);
    }

    ui->ecrit->text_player[0]->rect.x = ui->elems->statsPlayer.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_player[0]->rect.y = ui->elems->statsPlayer.rect.y + input->windowHeight * 0.01;
    ui->ecrit->text_item[0]->rect.x = ui->elems->statsItem.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_item[0]->rect.y = ui->elems->statsItem.rect.y + input->windowHeight * 0.01;

    ui->itemclique = item;
    ui->peutEquiper = 1;
}

void creerDescrWrapper(t_fonctionParam *f) {
    creerDescr(GET_PTR(f, 0, InventoryUI *), GET_PTR(f, 1, t_item *), GET_PTR(f, 2, SDL_Renderer *), GET_PTR(f, 3, t_input *));
}
void updateScroll(InventoryUI *ui, t_input *input) {
    if (input->mouseYWheel != 0) {
        int scrollStep = ui->nbItems;  // Ajustez cette valeur selon vos besoins
        ui->scrollY -= input->mouseYWheel * scrollStep;

        // Limiter le défilement
        if (ui->scrollY < 0)
            ui->scrollY = 0;
        else if (ui->scrollY > ui->maxScrollY)
            ui->scrollY = ui->maxScrollY;
    }
}

void update(t_input *input, InventoryUI *ui) {
    ui->width = input->windowWidth;
    ui->height = input->windowHeight;
}

void freeInv(void *elt) {
    InventoryUI *ui = (InventoryUI *)elt;
    if (!ui) return;

    // Libération de la structure externe
    if (ui->ext) {
        // Ne pas libérer character ici car il est géré ailleurs
        /* Liberation des polices */
        if (ui->ext->item_font) TTF_CloseFont(ui->ext->item_font);
        if (ui->ext->descr_font) TTF_CloseFont(ui->ext->descr_font);
        free(ui->ext);
    }

    /* Liberation des elements */
    if (ui->elems) {
        /* Liberation des slots d'inventaire */
        if (ui->elems->inventory_slots) {
            for (int i = 0; i < ui->nbItems; i++) {
                if (ui->elems->inventory_slots[i].texture) {
                    SDL_DestroyTexture(ui->elems->inventory_slots[i].texture);
                }
            }
            free(ui->elems->inventory_slots);
        }
        free(ui->elems);
    }

    /* Liberation des ecritures */
    if (ui->ecrit) {
        /* Liberation des chaines de caractères */
        for (int i = 0; i < 4; i++) {
            free(ui->ecrit->nom_txt_item[i]);
            free(ui->ecrit->nom_txt_player[i]);
            if (ui->ecrit->text_player[i]) freeText(ui->ecrit->text_player[i]);
            if (ui->ecrit->text_item[i]) freeText(ui->ecrit->text_item[i]);
        }

        /* Liberation des descriptions */
        for (int i = 0; i < ui->ecrit->count_descr; i++) {
            if (ui->ecrit->description[i]) freeText(ui->ecrit->description[i]);
        }

        /* Liberation du texte de description */
        if (ui->ecrit->text_descr) freeText(ui->ecrit->text_descr);
        free(ui->ecrit);
    }

    free(ui);
}

void handleInputInventaire(t_input *input, t_joueur *player, t_sceneController *sceneController) {
    if (input->key[player->control->inventaire]) {
        getPrevuisScene(sceneController);
        input->key[player->control->inventaire] = SDL_FALSE;
    } else if (input->key[player->control->escape]) {
        getPrevuisScene(sceneController);
        input->key[player->control->escape] = SDL_FALSE;
    }
}