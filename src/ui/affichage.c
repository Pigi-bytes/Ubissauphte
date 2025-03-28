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

void coucou() {
    printf("cocou\n");
}

InventoryUI *inventoryUI_Init(InventoryUI *ui2, SDL_Renderer *renderer, t_character *c, t_item **items, t_input *input) {
    initTextEngine();
    printf("%p\n", input);
    InventoryUI *ui = ui2;  // Utiliser l'instance existante si fournie

    if (!ui) {
        ui = malloc(sizeof(InventoryUI));  // Créer une nouvelle instance si NULL
    }

    ui->ext = malloc(sizeof(t_extern));
    ui->ext->item_font = NULL;
    ui->ext->descr_font = NULL;
    ui->ext->character = malloc(sizeof(t_character));

    ui->ecrit = malloc(sizeof(t_ecritures));
    // Initialiser les membres si nécessaire
    memset(ui->ecrit->nom_txt_item, 0, sizeof(ui->ecrit->nom_txt_item));
    // etc.

    ui->elems = malloc(sizeof(t_elements));

    // Initialisation des références
    ui->ext->character = c;
    ui->ext->items = items;
    ui->nbItems = 40;

    ui->ext->item_font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", ui->elems->statsPlayer.rect.h * ui->elems->statsPlayer.rect.w * 0.0002);
    // Pour les items
    ui->ecrit->nom_txt_item[0] = createStatLine("Health : ", items[0]->stats.health.additive, items[0]->stats.health.multiplicative);
    ui->ecrit->nom_txt_item[1] = createStatLine("Health Max : ", items[0]->stats.healthMax.additive, items[0]->stats.healthMax.multiplicative);
    ui->ecrit->nom_txt_item[2] = createStatLine("Mana : ", items[0]->stats.mana.additive, items[0]->stats.mana.multiplicative);
    ui->ecrit->nom_txt_item[3] = createStatLine("Mana Max : ", items[0]->stats.manaMax.additive, items[0]->stats.manaMax.multiplicative);
    ui->ecrit->nom_txt_item[4] = createStatLine("Attack : ", items[0]->stats.attack.additive, items[0]->stats.attack.multiplicative);
    ui->ecrit->nom_txt_item[5] = createStatLine("Defense : ", items[0]->stats.defense.additive, items[0]->stats.defense.multiplicative);
    ui->ecrit->nom_txt_item[6] = createStatLine("Speed : ", items[0]->stats.speed.additive, items[0]->stats.speed.multiplicative);

    // Pour le joueur (si les stats sont uniquement additives)
    ui->ecrit->nom_txt_player[0] = createStatLine("Health : ", c->baseStats.health.additive, 1.0f);
    ui->ecrit->nom_txt_player[1] = createStatLine("Health Max : ", c->baseStats.healthMax.additive, 1.0f);
    ui->ecrit->nom_txt_player[2] = createStatLine("Mana : ", c->baseStats.mana.additive, 1.0f);
    ui->ecrit->nom_txt_player[3] = createStatLine("Mana Max : ", c->baseStats.manaMax.additive, 1.0f);
    ui->ecrit->nom_txt_player[4] = createStatLine("Attack : ", c->baseStats.attack.additive, 1.0f);
    ui->ecrit->nom_txt_player[5] = createStatLine("Defense : ", c->baseStats.defense.additive, 1.0f);
    ui->ecrit->nom_txt_player[6] = createStatLine("Speed : ", c->baseStats.speed.additive, 1.0f);

    // Calculs initiaux (votre code original)
    calculCasePlayer(&ui->elems->player_panel.rect, input, "Perso");
    ui->elems->player_panel.texture = c->texture;

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
    ui->elems->inventory_slots = malloc(40 * sizeof(UI_Element));

    calculerItem(&ui->elems->inventory_slots[0].rect, ui->elems->inventory_panel.rect,
                 &ui->elems->inventory_panel.rect, 0, 0, input);
    ui->elems->inventory_slots[0].texture = items[0]->texture;
    ui->elems->inventory_slots[0].button = createButton(createText(renderer, " ", ui->ext->item_font, (SDL_Color){255, 255, 255}), (SDL_Color){255, 128, 0}, (SDL_Color){255, 69, 0}, ui->elems->inventory_slots[0].rect, creerFonction(creerDescrWrapper, FONCTION_PARAMS(ui, ui->ext->items[0], renderer, input)));

    for (int i = 1, j = 1; i < 40; j++, i++) {
        calculerItem(&ui->elems->inventory_slots[i].rect, ui->elems->inventory_panel.rect, &ui->elems->inventory_slots[i - 1].rect, i, j, input);
        ui->elems->inventory_slots[i].texture = items[i]->texture;
        ui->elems->inventory_slots[i].button = createButton(createText(renderer, " ", ui->ext->item_font, (SDL_Color){255, 255, 255}), (SDL_Color){255, 128, 0}, (SDL_Color){255, 69, 0}, ui->elems->inventory_slots[i].rect, creerFonction(creerDescrWrapper, FONCTION_PARAMS(ui, ui->ext->items[i], renderer, input)));
        if (j == 4)
            j = 0;
    }

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

    t_text *t = createText(renderer, "cocou", ui->ext->item_font, (SDL_Color){0, 0, 0});
    ui->elems->equiper.button = createButton(t, (SDL_Color){0, 128, 255}, (SDL_Color){0, 150, 255}, ui->elems->equiper.rect, creerFonction(coucou, FONCTION_PARAMS(NULL)));

    // Initialisation des textes statsPlayer
    for (int i = 0; i < 7; i++) {
        ui->ecrit->text_player[i] = createText(renderer, ui->ecrit->nom_txt_player[i], ui->ext->item_font, ui->ecrit->color_txt);
    }

    // Positionnement et affichage des textes
    ui->ecrit->text_player[0]->rect.x = ui->elems->statsPlayer.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_player[0]->rect.y = ui->elems->statsPlayer.rect.y + input->windowHeight * 0.01;

    // Initialisation des textes statsItem

    for (int i = 0; i < 7; i++) {
        ui->ecrit->text_item[i] = createText(renderer, ui->ecrit->nom_txt_item[i], ui->ext->descr_font, ui->ecrit->color_txt);
    }

    // Positionnement et affichage des textes
    ui->ecrit->text_item[0]->rect.x = ui->elems->statsItem.rect.x + input->windowWidth * 0.01;
    ui->ecrit->text_item[0]->rect.y = ui->elems->statsItem.rect.y + input->windowHeight * 0.01;
    ui->ecrit->text_descr = createText(renderer, "Description :", ui->ext->descr_font, ui->ecrit->color_txt);

    ui->ecrit->text_descr->rect.x = ui->elems->descrItem.rect.x + ui->elems->descrItem.rect.x * 0.025;
    ui->ecrit->text_descr->rect.y = ui->elems->descrItem.rect.y + input->windowHeight * 0.005;

    // int i = 0, j, nb = 1;

    // while (ui->ext->items[0]->description[i++] != '\0') {
    //     for (j = 0; ui->ext->items[0]->description[i] != '\n' && ui->ext->items[0]->description[i] != '\0'; i++, j++) {
    //         ui->ecrit->descr[j] = ui->ext->items[0]->description[i];
    //     }
    //     ui->ecrit->descr[j] = '\0';

    //     ui->ecrit->description[nb - 1] = createText(renderer, ui->ecrit->descr, ui->ext->descr_font, ui->ecrit->color_txt);
    //     ui->ecrit->description[nb - 1]->rect.x = ui->elems->descrItem.rect.x + input->windowWidth * 0.005;
    //     ui->ecrit->description[nb - 1]->rect.y = ui->ecrit->text_descr->rect.y + input->windowHeight * 0.03 * nb;
    //     nb++;
    //     ui->ecrit->descr[0] = '\0';
    // }
    // ui->ecrit->count_descr = nb - 1;
    ui2 = ui;

    return ui;
}

void afficherStatPlayer(SDL_Renderer *renderer, InventoryUI *ui, t_input *input) {
    renderText(renderer, ui->ecrit->text_player[0]);

    for (int i = 1; i < 7; i++) {
        afficherText(renderer, ui->ecrit->text_player[i - 1], ui->ecrit->text_player[i], input);
    }
}

void afficherStatItem(SDL_Renderer *renderer, InventoryUI *ui, t_input *input) {
    renderText(renderer, ui->ecrit->text_item[0]);
    for (int i = 1; i < 7; i++) {
        afficherText(renderer, ui->ecrit->text_item[i - 1], ui->ecrit->text_item[i], input);
    }
}

void afficherDescription(SDL_Renderer *renderer, InventoryUI *ui) {
    renderText(renderer, ui->ecrit->text_descr);

    for (int i = 0; i < ui->ecrit->count_descr; i++) {
        renderText(renderer, ui->ecrit->description[i]);
    }
}

void inventoryUI_Render(InventoryUI *ui, SDL_Renderer *renderer, t_input *input) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderCopy(renderer, ui->elems->player_panel.texture, NULL, &ui->elems->player_panel.rect);
    SDL_RenderDrawRect(renderer, &ui->elems->player_panel.rect);

    SDL_RenderDrawRect(renderer, &ui->elems->caseArme.rect);
    SDL_RenderDrawRect(renderer, &ui->elems->caseArmure.rect);
    SDL_RenderDrawRect(renderer, &ui->elems->CaseActivable1.rect);
    SDL_RenderDrawRect(renderer, &ui->elems->caseActivable2.rect);

    SDL_RenderDrawRect(renderer, &ui->elems->statsPlayer.rect);
    afficherStatPlayer(renderer, ui, input);

    SDL_RenderDrawRect(renderer, &ui->elems->inventory_panel.rect);

    SDL_RenderSetClipRect(renderer, &ui->elems->inventory_panel.rect);
    for (int i = 0; i < ui->nbItems; i++) {
        SDL_Rect dest = ui->elems->inventory_slots[i].rect;

        dest.y -= ui->scrollY;  // Appliquer le décalage
        ui->elems->inventory_slots[i].button->rect.y = dest.y;
        renderButton(renderer, ui->elems->inventory_slots[i].button);

        SDL_RenderCopy(renderer, ui->elems->inventory_slots[i].texture, NULL, &dest);
        SDL_RenderDrawRect(renderer, &dest);
    }
    SDL_RenderSetClipRect(renderer, NULL);

    SDL_RenderDrawRect(renderer, &ui->elems->statsItem.rect);
    afficherStatItem(renderer, ui, input);

    SDL_RenderDrawRect(renderer, &ui->elems->descrItem.rect);
    afficherDescription(renderer, ui);

    SDL_RenderDrawRect(renderer, &ui->elems->equiper.rect);
    renderButton(renderer, ui->elems->equiper.button);
}

void inventoryUI_Update(InventoryUI *ui, SDL_Renderer *renderer, t_input *input) {
    // Recalcul si la fenêtre est redimensionnée
    for (int i = 0; i < ui->nbItems; i++) {
        handleInputButton(input, ui->elems->inventory_slots[i].button);
    }

    handleInputButton(input, ui->elems->equiper.button);
    if (input->windowWidth != ui->width || input->windowHeight != ui->height) {
        int lastScroll = ui->scrollY;

        // Réinitialiser l'UI
        inventoryUI_Init(ui, renderer, ui->ext->character, ui->ext->items, input);

        // Restaurer le scroll dans les nouvelles limites
        ui->scrollY = lastScroll;
        if (ui->scrollY > ui->maxScrollY) ui->scrollY = ui->maxScrollY;
    }
}

void creerDescr(InventoryUI *ui, t_item *item, SDL_Renderer *renderer, t_input *input) {
    int i = 0, j, nb = 1;
    printf("%p\n", input);
        char descr[50];

    while (item->description[i++] != '\0') {
        for (j = 0; item->description[i] != '\n' && item->description[i] != '\0'; i++, j++) {
            descr[j] = item->description[i];
        }
        descr[j] = '\0';

        ui->ecrit->description[nb - 1] = createText(renderer, descr, ui->ext->descr_font, ui->ecrit->color_txt);
        ui->ecrit->description[nb - 1]->rect.x = ui->elems->descrItem.rect.x + input->windowWidth * 0.005;
        ui->ecrit->description[nb - 1]->rect.y = ui->ecrit->text_descr->rect.y + input->windowHeight * 0.03 * nb;
        nb++;
        ui->ecrit->descr[0] = '\0';
    }
    ui->ecrit->count_descr = nb - 1;
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
        for (int i = 0; i < 7; i++) {
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