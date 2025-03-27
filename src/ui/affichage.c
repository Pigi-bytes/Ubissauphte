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

void handleItemClick(void *data) {
    ItemClickData *clickData = (ItemClickData *)data;
    clickData->ui->selectedItemIndex = clickData->index;
}

void updateItemDisplay(InventoryUI *ui, t_item *item, SDL_Renderer *renderer) {
    // 1. Mise à jour des statistiques
    //--------------------------------
    // Libération des anciennes statistiques
    for (int i = 0; i < 7; i++) {
        free(ui->nom_txt_item[i]);
        freeText(ui->text_item[i]);
    }

    // Génération des nouvelles statistiques
    ui->nom_txt_item[0] = createStatLine("Health : ", item->stats.health.additive, item->stats.health.multiplicative);
    ui->nom_txt_item[1] = createStatLine("Health Max : ", item->stats.healthMax.additive, item->stats.healthMax.multiplicative);
    ui->nom_txt_item[2] = createStatLine("Mana : ", item->stats.mana.additive, item->stats.mana.multiplicative);
    ui->nom_txt_item[3] = createStatLine("Mana Max : ", item->stats.manaMax.additive, item->stats.manaMax.multiplicative);
    ui->nom_txt_item[4] = createStatLine("Attack : ", item->stats.attack.additive, item->stats.attack.multiplicative);
    ui->nom_txt_item[5] = createStatLine("Defense : ", item->stats.defense.additive, item->stats.defense.multiplicative);
    ui->nom_txt_item[6] = createStatLine("Speed : ", item->stats.speed.additive, item->stats.speed.multiplicative);

    // Création des nouvelles textures pour les stats
    for (int i = 0; i < 7; i++) {
        ui->text_item[i] = createText(renderer,
                                      ui->nom_txt_item[i],
                                      ui->descr_font,
                                      ui->color_txt);

        // Positionnement relatif
        ui->text_item[i]->rect.x = ui->statsItem.rect.x + ui->input_ref->windowWidth * 0.01;
        ui->text_item[i]->rect.y = ui->statsItem.rect.y + ui->input_ref->windowHeight * 0.01 + (i * ui->statsItem.rect.h * 0.1);
    }

    // 2. Mise à jour de la description
    //---------------------------------
    // Libération de l'ancienne description
    for (int i = 0; i < ui->count_descr; i++) {
        freeText(ui->description[i]);
    }
    ui->count_descr = 0;

    // Découpage de la nouvelle description
    int lineIndex = 0;
    int descLength = strlen(item->description);
    char buffer[256];
    int bufferIndex = 0;

    for (int i = 0; i <= descLength; i++) {
        if (item->description[i] == '\n' || item->description[i] == '\0') {
            if (bufferIndex > 0) {
                buffer[bufferIndex] = '\0';

                // Création du texte pour la ligne
                ui->description[lineIndex] = createText(renderer,
                                                        buffer,
                                                        ui->descr_font,
                                                        ui->color_txt);

                // Positionnement dynamique
                ui->description[lineIndex]->rect.x = ui->descrItem.rect.x + ui->input_ref->windowWidth * 0.01;
                ui->description[lineIndex]->rect.y = ui->text_descr->rect.y +
                                                     ui->text_descr->rect.h +
                                                     (lineIndex * ui->descrItem.rect.h * 0.1);

                lineIndex++;
                bufferIndex = 0;
            }
        } else {
            buffer[bufferIndex++] = item->description[i];
        }
    }
    ui->count_descr = lineIndex;

    // 3. Mise à jour du nom de l'item
    //--------------------------------
    freeText(ui->text_descr);
    char descrTitle[256];
    snprintf(descrTitle, sizeof(descrTitle), "Description : %s", item->name);
    ui->text_descr = createText(renderer, descrTitle, ui->descr_font, ui->color_txt);
    ui->text_descr->rect.x = ui->descrItem.rect.x + ui->descrItem.rect.w * 0.02;
    ui->text_descr->rect.y = ui->descrItem.rect.y + ui->descrItem.rect.h * 0.02;
}
void inventoryUI_Init(InventoryUI *ui, SDL_Renderer *renderer, t_character *c, t_item **items, t_input *input, int nbItems) {
    initTextEngine();
    // Initialisation des références
    ui->character = c;
    ui->items = items;
    ui->input_ref = input;
    ui->nbItems = nbItems;

    // Pour les items
    ui->nom_txt_item[0] = createStatLine("Health : ", items[0]->stats.health.additive, items[0]->stats.health.multiplicative);
    ui->nom_txt_item[1] = createStatLine("Health Max : ", items[0]->stats.healthMax.additive, items[0]->stats.healthMax.multiplicative);
    ui->nom_txt_item[2] = createStatLine("Mana : ", items[0]->stats.mana.additive, items[0]->stats.mana.multiplicative);
    ui->nom_txt_item[3] = createStatLine("Mana Max : ", items[0]->stats.manaMax.additive, items[0]->stats.manaMax.multiplicative);
    ui->nom_txt_item[4] = createStatLine("Attack : ", items[0]->stats.attack.additive, items[0]->stats.attack.multiplicative);
    ui->nom_txt_item[5] = createStatLine("Defense : ", items[0]->stats.defense.additive, items[0]->stats.defense.multiplicative);
    ui->nom_txt_item[6] = createStatLine("Speed : ", items[0]->stats.speed.additive, items[0]->stats.speed.multiplicative);

    // Pour le joueur (si les stats sont uniquement additives)
    ui->nom_txt_player[0] = createStatLine("Health : ", c->baseStats.health.additive, 1.0f);
    ui->nom_txt_player[1] = createStatLine("Health Max : ", c->baseStats.healthMax.additive, 1.0f);
    ui->nom_txt_player[2] = createStatLine("Mana : ", c->baseStats.mana.additive, 1.0f);
    ui->nom_txt_player[3] = createStatLine("Mana Max : ", c->baseStats.manaMax.additive, 1.0f);
    ui->nom_txt_player[4] = createStatLine("Attack : ", c->baseStats.attack.additive, 1.0f);
    ui->nom_txt_player[5] = createStatLine("Defense : ", c->baseStats.defense.additive, 1.0f);
    ui->nom_txt_player[6] = createStatLine("Speed : ", c->baseStats.speed.additive, 1.0f);

    // Calculs initiaux (votre code original)
    calculCasePlayer(&ui->player_panel.rect, ui->input_ref, "Perso");
    ui->player_panel.texture = c->texture;

    calculCaseSlots(&ui->player_panel.rect, &ui->caseArme.rect, ui->input_ref, "Perso", "arme");
    calculCaseSlots(&ui->caseArme.rect, &ui->caseArmure.rect, ui->input_ref, "arme", "armure");
    calculCaseSlots(&ui->player_panel.rect, &ui->CaseActivable1.rect, ui->input_ref, "Perso", "activable1");
    calculCaseSlots(&ui->CaseActivable1.rect, &ui->caseActivable2.rect, ui->input_ref, "activable1", "activable2");

    calculDescrStatsPlayer(&ui->caseArme.rect, &ui->CaseActivable1.rect, &ui->player_panel.rect, &ui->statsPlayer.rect, ui->input_ref);
    calculInventaire(&ui->inventory_panel.rect, &ui->statsPlayer.rect, ui->input_ref);
    calculStatsItem(&ui->inventory_panel.rect, &ui->statsItem.rect, ui->input_ref);
    caculDescrItem(&ui->statsItem.rect, &ui->descrItem.rect, ui->input_ref);
    calculEquiper(&ui->statsItem.rect, &ui->descrItem.rect, &ui->equiper.rect, ui->input_ref);

    ui->selectedItemIndex = -1;
    ui->inventory_buttons = malloc(nbItems * sizeof(t_button *));

    SDL_Color transparent = {0, 0, 0, 0};
    SDL_Color hoverColor = {255, 255, 255, 50};

    for (int i = 0, j = 0; i < nbItems; j++, i++) {
        SDL_Rect slotRect;
        calculerItem(&slotRect, ui->inventory_panel.rect, (i > 0) ? &ui->inventory_buttons[i - 1]->rect : NULL, i, j, ui->input_ref);

        // Créer un bouton invisible pour l'interaction
        t_text *buttonText = createText(renderer, "", NULL, transparent);
        t_fonctionParam *onClick = creerFonction((void(*)(t_fonctionParam*))handleItemClick, &(ItemClickData){ui, i}, NULL);
        ui->inventory_buttons[i] = createButton(buttonText, transparent, hoverColor, slotRect, onClick);

        if (j == 4) j = 0;
    }

    // Police
    ui->item_font = TTF_OpenFont("assets/fonts/JetBrainsMono-Regular.ttf", ui->statsPlayer.rect.h * ui->statsPlayer.rect.w * 0.0002);
    ui->descr_font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", ui->descrItem.rect.h * ui->descrItem.rect.w * 0.0003);
    ui->color.a = 255;
    ui->color.b = 255;
    ui->color.g = 0;
    ui->color.r = 0;

    // affichage des texts

    ui->color_txt.a = 255;
    ui->color_txt.b = 0;
    ui->color_txt.g = 128;
    ui->color_txt.r = 255;

    // Initialisation des textes statsPlayer
    ui->text_player[0] = createText(renderer, ui->nom_txt_player[0], ui->item_font, ui->color_txt);
    ui->text_player[1] = createText(renderer, ui->nom_txt_player[1], ui->item_font, ui->color_txt);
    ui->text_player[2] = createText(renderer, ui->nom_txt_player[2], ui->item_font, ui->color_txt);
    ui->text_player[3] = createText(renderer, ui->nom_txt_player[3], ui->item_font, ui->color_txt);
    ui->text_player[4] = createText(renderer, ui->nom_txt_player[4], ui->item_font, ui->color_txt);
    ui->text_player[5] = createText(renderer, ui->nom_txt_player[5], ui->item_font, ui->color_txt);
    ui->text_player[6] = createText(renderer, ui->nom_txt_player[6], ui->item_font, ui->color_txt);

    // Positionnement et affichage des textes
    ui->text_player[0]->rect.x = ui->statsPlayer.rect.x + ui->input_ref->windowWidth * 0.01;
    ui->text_player[0]->rect.y = ui->statsPlayer.rect.y + ui->input_ref->windowHeight * 0.01;

    // Initialisation des textes statsItem

    ui->text_item[0] = createText(renderer, ui->nom_txt_item[0], ui->descr_font, ui->color_txt);
    ui->text_item[1] = createText(renderer, ui->nom_txt_item[1], ui->descr_font, ui->color_txt);
    ui->text_item[2] = createText(renderer, ui->nom_txt_item[2], ui->descr_font, ui->color_txt);
    ui->text_item[3] = createText(renderer, ui->nom_txt_item[3], ui->descr_font, ui->color_txt);
    ui->text_item[4] = createText(renderer, ui->nom_txt_item[4], ui->descr_font, ui->color_txt);
    ui->text_item[5] = createText(renderer, ui->nom_txt_item[5], ui->descr_font, ui->color_txt);
    ui->text_item[6] = createText(renderer, ui->nom_txt_item[6], ui->descr_font, ui->color_txt);

    // Positionnement et affichage des textes
    ui->text_item[0]->rect.x = ui->statsItem.rect.x + ui->input_ref->windowWidth * 0.01;
    ui->text_item[0]->rect.y = ui->statsItem.rect.y + ui->input_ref->windowHeight * 0.01;

    ui->text_descr = createText(renderer, "Description :", ui->descr_font, ui->color_txt);

    ui->text_descr->rect.x = ui->descrItem.rect.x + ui->descrItem.rect.x * 0.025;
    ui->text_descr->rect.y = ui->descrItem.rect.y + ui->input_ref->windowHeight * 0.005;

    int i = 0, j, nb = 1;

    while (ui->items[0]->description[i++] != '\0') {
        for (j = 0; ui->items[0]->description[i] != '\n' && ui->items[0]->description[i] != '\0'; i++, j++) {
            ui->descr[j] = ui->items[0]->description[i];
        }
        ui->descr[j] = '\0';

        ui->description[nb - 1] = createText(renderer, ui->descr, ui->descr_font, ui->color_txt);
        ui->description[nb - 1]->rect.x = ui->descrItem.rect.x + ui->input_ref->windowWidth * 0.005;
        ui->description[nb - 1]->rect.y = ui->text_descr->rect.y + ui->input_ref->windowHeight * 0.03 * nb;
        nb++;
        ui->descr[0] = '\0';
    }
    ui->count_descr = nb - 1;
}

void afficherStatPlayer(SDL_Renderer *renderer, InventoryUI *ui, t_input *input) {
    renderText(renderer, ui->text_player[0]);

    afficherText(renderer, ui->text_player[0], ui->text_player[1], input);

    afficherText(renderer, ui->text_player[1], ui->text_player[2], input);

    afficherText(renderer, ui->text_player[2], ui->text_player[3], input);

    afficherText(renderer, ui->text_player[3], ui->text_player[4], input);

    afficherText(renderer, ui->text_player[4], ui->text_player[5], input);

    afficherText(renderer, ui->text_player[5], ui->text_player[6], input);
}

void afficherStatItem(SDL_Renderer *renderer, InventoryUI *ui) {
    renderText(renderer, ui->text_item[0]);

    afficherText(renderer, ui->text_item[0], ui->text_item[1], ui->input_ref);

    afficherText(renderer, ui->text_item[1], ui->text_item[2], ui->input_ref);

    afficherText(renderer, ui->text_item[2], ui->text_item[3], ui->input_ref);

    afficherText(renderer, ui->text_item[3], ui->text_item[4], ui->input_ref);

    afficherText(renderer, ui->text_item[4], ui->text_item[5], ui->input_ref);

    afficherText(renderer, ui->text_item[5], ui->text_item[6], ui->input_ref);
}

void afficherDescription(SDL_Renderer *renderer, InventoryUI *ui) {
    renderText(renderer, ui->text_descr);

    for (int i = 0; i < ui->count_descr; i++) {
        renderText(renderer, ui->description[i]);
    }
}

void inventoryUI_Render(InventoryUI *ui, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderCopy(renderer, ui->player_panel.texture, NULL, &ui->player_panel.rect);
    SDL_RenderDrawRect(renderer, &ui->player_panel.rect);

    SDL_RenderDrawRect(renderer, &ui->caseArme.rect);
    SDL_RenderDrawRect(renderer, &ui->caseArmure.rect);
    SDL_RenderDrawRect(renderer, &ui->CaseActivable1.rect);
    SDL_RenderDrawRect(renderer, &ui->caseActivable2.rect);

    SDL_RenderDrawRect(renderer, &ui->statsPlayer.rect);
    afficherStatPlayer(renderer, ui, ui->input_ref);

    SDL_RenderDrawRect(renderer, &ui->inventory_panel.rect);

    // Rendu des slots
    SDL_RenderSetClipRect(renderer, &ui->inventory_panel.rect);
    for (int i = 0; i < ui->nbItems; i++) {
        // Rendu de l'item
        SDL_Rect dest = ui->inventory_buttons[i]->rect;
        dest.y -= ui->scrollY;

        if (ui->items[i]->texture) {
            SDL_RenderCopy(renderer, ui->items[i]->texture, NULL, &dest);
        }

        // Rendu du bouton (cadre interactif)
        renderButton(renderer, ui->inventory_buttons[i]);
    }
    SDL_RenderSetClipRect(renderer, NULL);

    // Afficher les stats de l'item sélectionné
    if (ui->selectedItemIndex >= 0 && ui->selectedItemIndex < ui->nbItems) {
        updateItemDisplay(ui, ui->items[ui->selectedItemIndex], renderer);
        afficherStatItem(renderer, ui);
        afficherDescription(renderer, ui);
    }
    SDL_RenderSetClipRect(renderer, NULL);

    SDL_RenderDrawRect(renderer, &ui->statsItem.rect);
    afficherStatItem(renderer, ui);

    SDL_RenderDrawRect(renderer, &ui->descrItem.rect);
    afficherDescription(renderer, ui);

    SDL_RenderDrawRect(renderer, &ui->equiper.rect);
}

void inventoryUI_Update(InventoryUI *ui, SDL_Renderer *renderer, t_input *input) {
    // Recalcul si la fenêtre est redimensionnée

    int lastScroll = ui->scrollY;

    // Réinitialiser l'UI
    inventoryUI_Init(ui, renderer, ui->character, ui->items, input, ui->nbItems);

    // Restaurer le scroll dans les nouvelles limites
    ui->scrollY = lastScroll;
    if (ui->scrollY > ui->maxScrollY) ui->scrollY = ui->maxScrollY;
}
