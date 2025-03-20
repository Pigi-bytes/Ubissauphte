#include "affichage.h"

void calculCasePlayer(SDL_Rect* casePlayer, t_input* input, char* nom) {
    if (strcmp(nom, "Perso") == 0) {
        casePlayer->h = input->windowWidth * 0.14;
        casePlayer->w = input->windowWidth * 0.16;
        casePlayer->x = input->windowWidth * 0.02;
        casePlayer->y = input->windowHeight * 0.08;
    }
}
void calculCaseSlots(SDL_Rect* comp, SDL_Rect* slot, t_input* input, char* nom1, char* nom2) {
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

void calculDescrStatsPlayer(SDL_Rect* slotDroit, SDL_Rect* slotBas, SDL_Rect* casePlayer, SDL_Rect* caseStatsPlayer, t_input* input) {
    caseStatsPlayer->h = casePlayer->h + slotBas->h;
    caseStatsPlayer->w = casePlayer->w + slotDroit->w + input->windowWidth * 0.04;
    caseStatsPlayer->x = casePlayer->x;
    caseStatsPlayer->y = slotBas->y + slotBas->h + input->windowHeight * 0.04;
}

void calculInventaire(SDL_Rect* inv, SDL_Rect* statsPlayer, t_input* input) {
    inv->h = statsPlayer->h * 2 + input->windowHeight * 0.08;
    inv->w = statsPlayer->w * 1.5;
    inv->x = statsPlayer->w + input->windowWidth * 0.04;
    inv->y = input->windowHeight * 0.08;
}

void calculStatsItem(SDL_Rect* inv, SDL_Rect* statsItem, t_input* input) {
    statsItem->h = inv->h / 2 - input->windowHeight * 0.01;
    statsItem->w = inv->w / 2.5;
    statsItem->x = inv->x + inv->w + input->windowWidth * 0.04;
    statsItem->y = inv->y;
}

void caculDescrItem(SDL_Rect* statsItem, SDL_Rect* descrItem, t_input* input) {
    descrItem->h = statsItem->h * 2 / 3 - input->windowHeight * 0.01;
    descrItem->w = statsItem->w;
    descrItem->x = statsItem->x;
    descrItem->y = statsItem->h + statsItem->y + input->windowHeight * 0.03;
}

void calculEquiper(SDL_Rect* statsItem, SDL_Rect* descrItem, SDL_Rect* Equiper, t_input* input) {
    Equiper->h = statsItem->h / 3 - input->windowHeight * 0.03;
    Equiper->w = descrItem->w;
    Equiper->x = descrItem->x;
    Equiper->y = descrItem->y + descrItem->h + input->windowHeight * 0.03;
}

char* createStatText(char* statName, float statValue) {
    char* name = strdup(statName);
    char* value = malloc(sizeof(char*));
    sprintf(value, "%.2f", statValue);
    strcat(name, value);

    return name;
}

void calculerItem(SDL_Rect* item, SDL_Rect inv, int nb, t_input* input) {
    item->h = inv.h / 5;
    item->w = inv.w / 5;
    if (nb != 0)
        item->x = inv.x + inv.x * nb / 4 + input->windowWidth * 0.02 * nb;
    else
        item->x = inv.x + input->windowWidth * 0.004;
    item->y = inv.y + input->windowHeight * 0.004;
}

void afficherText(SDL_Renderer* renderer, t_text* txt1, t_text* txt2, t_input* input) {
    txt2->rect.x = txt1->rect.x;
    txt2->rect.y = txt1->rect.y + input->windowWidth * 0.03;
    renderText(renderer, txt2);
}

void afficherStatsPlayer(SDL_Renderer* renderer, SDL_Rect rect, t_character* c, TTF_Font* font, SDL_Color color, t_input* input) {
    char* nom_txt1 = createStatText("Health : ", c->baseStats.health.additive);
    t_text* txt1 = createText(renderer, nom_txt1, font, color);
    char* nom_txt2 = createStatText("Health Max : ", c->baseStats.healthMax.additive);
    t_text* txt2 = createText(renderer, nom_txt2, font, color);
    char* nom_txt3 = createStatText("Mana : ", c->baseStats.mana.additive);
    char* nom_txt4 = createStatText("Mana Max : ", c->baseStats.manaMax.additive);
    char* nom_txt5 = createStatText("Attack : ", c->baseStats.attack.additive);
    char* nom_txt6 = createStatText("Defense : ", c->baseStats.defense.additive);
    char* nom_txt7 = createStatText("Speed : ", c->baseStats.speed.additive);

    txt1->rect.x = rect.x + input->windowWidth * 0.01;
    txt1->rect.y = rect.y + input->windowHeight * 0.01;
    renderText(renderer, txt1);

    afficherText(renderer, txt1, txt2, input);

    updateText(&txt1, renderer, nom_txt3, color);
    afficherText(renderer, txt2, txt1, input);

    updateText(&txt2, renderer, nom_txt4, color);
    afficherText(renderer, txt1, txt2, input);

    updateText(&txt1, renderer, nom_txt5, color);
    afficherText(renderer, txt2, txt1, input);

    updateText(&txt2, renderer, nom_txt6, color);
    afficherText(renderer, txt1, txt2, input);

    updateText(&txt1, renderer, nom_txt7, color);
    afficherText(renderer, txt2, txt1, input);
}

void afficherStatsItem(SDL_Renderer* renderer, SDL_Rect rect, t_item* item, TTF_Font* font, SDL_Color color, t_input* input) {
    t_text* name = createText(renderer, item->name, font, color);
    char* nom_txt1 = createStatText("Health : ", item->stats.health.additive);
    t_text* txt1 = createText(renderer, nom_txt1, font, color);
    char* nom_txt2 = createStatText("Health Max : ", item->stats.healthMax.additive);
    t_text* txt2 = createText(renderer, nom_txt2, font, color);
    char* nom_txt3 = createStatText("Mana : ", item->stats.mana.additive);
    char* nom_txt4 = createStatText("Mana Max : ", item->stats.manaMax.additive);
    char* nom_txt5 = createStatText("Attack : ", item->stats.attack.additive);
    char* nom_txt6 = createStatText("Defense : ", item->stats.defense.additive);
    char* nom_txt7 = createStatText("Speed : ", item->stats.speed.additive);

    name->rect.x = rect.x + rect.x * 0.025;
    name->rect.y = rect.y + input->windowHeight * 0.005;
    renderText(renderer, name);

    updateText(&txt1, renderer, nom_txt1, color);
    txt1->rect.x = rect.x + input->windowWidth * 0.005;
    txt1->rect.y = name->rect.y + input->windowHeight * 0.04;
    renderText(renderer, txt1);

    updateText(&txt2, renderer, nom_txt2, color);
    afficherText(renderer, txt1, txt2, input);

    updateText(&txt1, renderer, nom_txt3, color);
    afficherText(renderer, txt2, txt1, input);

    updateText(&txt2, renderer, nom_txt4, color);
    afficherText(renderer, txt1, txt2, input);

    updateText(&txt1, renderer, nom_txt5, color);
    afficherText(renderer, txt2, txt1, input);

    updateText(&txt2, renderer, nom_txt6, color);
    afficherText(renderer, txt1, txt2, input);

    updateText(&txt1, renderer, nom_txt7, color);
    afficherText(renderer, txt2, txt1, input);
}

void afficherDescription(SDL_Renderer* renderer, SDL_Rect rect, t_item* item, TTF_Font* font, SDL_Color color, t_input* input) {
    t_text* name = createText(renderer, "Description :", font, color);

    name->rect.x = rect.x + rect.x * 0.025;
    name->rect.y = rect.y + input->windowHeight * 0.005;
    renderText(renderer, name);

    int i = 0, j, nb = 1;

    while (item->description[i++] != '\0') {
        char descr[50];
        for (j = 0; item->description[i] != '\n' && item->description[i] != '\0'; i++, j++) {
            descr[j] = item->description[i];
        }
        descr[j] = '\0';

        printf("%d\n", i);

        t_text* description = createText(renderer, descr, font, color);
        description->rect.x = rect.x + input->windowWidth * 0.005;
        description->rect.y = name->rect.y + input->windowHeight * 0.03 * nb;
        renderText(renderer, description);
        nb++;
    }
}

void afficherInventaire(SDL_Renderer* renderer, t_input* input, t_character* c, t_item* item) {
    initTextEngine();

    SDL_Color color = {
        .a = 255,
        .b = 255,
        .g = 0,
        .r = 255};

    SDL_Rect casePerso;
    SDL_Rect caseArme;
    SDL_Rect caseArmure;
    SDL_Rect CaseActivable1;
    SDL_Rect caseActivable2;
    SDL_Rect statsPlayer;
    SDL_Rect inv;
    SDL_Rect statsItem;
    SDL_Rect descrItem;
    SDL_Rect equiper;
    SDL_Rect caseItem[16];

    calculCasePlayer(&casePerso, input, "Perso");

    calculCaseSlots(&casePerso, &caseArme, input, "Perso", "arme");
    calculCaseSlots(&caseArme, &caseArmure, input, "arme", "armure");
    calculCaseSlots(&casePerso, &CaseActivable1, input, "Perso", "activable1");
    calculCaseSlots(&CaseActivable1, &caseActivable2, input, "activable1", "activable2");

    calculDescrStatsPlayer(&caseArme, &CaseActivable1, &casePerso, &statsPlayer, input);

    calculInventaire(&inv, &statsPlayer, input);

    calculStatsItem(&inv, &statsItem, input);

    caculDescrItem(&statsItem, &descrItem, input);

    calculEquiper(&statsItem, &descrItem, &equiper, input);

    for (int i = 0; i < 4; i++) {
        calculerItem(&caseItem[i], inv, i, input);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &casePerso);
    SDL_RenderDrawRect(renderer, &caseArme);
    SDL_RenderDrawRect(renderer, &caseArmure);
    SDL_RenderDrawRect(renderer, &CaseActivable1);
    SDL_RenderDrawRect(renderer, &caseActivable2);
    SDL_RenderDrawRect(renderer, &statsPlayer);

    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", statsPlayer.h * statsPlayer.w * 0.0002);

    afficherStatsPlayer(renderer, statsPlayer, c, font, color, input);

    SDL_RenderDrawRect(renderer, &inv);

    for (int i = 0; i < 4; i++) {
        SDL_RenderDrawRect(renderer, &caseItem[i]);
    }

    SDL_RenderDrawRect(renderer, &statsItem);

    TTF_Font* font1 = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", statsItem.h * statsItem.w * 0.0002);

    afficherStatsItem(renderer, statsItem, item, font1, color, input);

    SDL_RenderDrawRect(renderer, &descrItem);
    TTF_Font* font2 = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", descrItem.h * descrItem.w * 0.0003);

    afficherDescription(renderer, descrItem, item, font2, color, input);
    SDL_RenderDrawRect(renderer, &equiper);
}