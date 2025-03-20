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

char* createStatText(SDL_Renderer* renderer, char* statName, float statValue, TTF_Font* font, SDL_Color color) {
    char* name = strdup(statName);
    char* value = malloc(sizeof(char*));
    sprintf(value, "%.2f", statValue);
    strcat(name, value);

    return name;
}

void afficherText(SDL_Renderer* renderer, t_text* txt1, t_text* txt2, SDL_Rect rect, t_input* input) {
    txt2->rect.x = txt1->rect.x;
    txt2->rect.y = txt1->rect.y + input->windowWidth * 0.03;
    renderText(renderer, txt2);
}
void afficherInventaire(SDL_Renderer* renderer, t_input* input, t_character* c, t_item* item) {
    initTextEngine();
    TTF_Font* font = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 24);
    TTF_Font* font1 = loadFont("assets/fonts/JetBrainsMono-Regular.ttf", 18);

    SDL_Color color = {
        .a = 255,
        .b = 255,
        .g = 0,
        .r = 255};

    char* nom_txt1 = createStatText(renderer, "Health : ", c->baseStats.health.additive, font, color);
    t_text* txt1 = createText(renderer, nom_txt1, font, color);
    char* nom_txt2 = createStatText(renderer, "Health Max : ", c->baseStats.healthMax.additive, font, color);
    t_text* txt2 = createText(renderer, nom_txt2, font, color);
    char* nom_txt3 = createStatText(renderer, "Mana : ", c->baseStats.mana.additive, font, color);
    char* nom_txt4 = createStatText(renderer, "Mana Max : ", c->baseStats.manaMax.additive, font, color);
    char* nom_txt5 = createStatText(renderer, "Attack : ", c->baseStats.attack.additive, font, color);
    char* nom_txt6 = createStatText(renderer, "Defense : ", c->baseStats.defense.additive, font, color);
    char* nom_txt7 = createStatText(renderer, "Speed : ", c->baseStats.speed.additive, font, color);

    t_text* name = createText(renderer, item->name, font1, color);
    char* nom_txt8 = createStatText(renderer, "Health : ", item->stats.health.additive, font1, color);
    char* nom_txt9 = createStatText(renderer, "Health Max : ", item->stats.healthMax.additive, font1, color);
    char* nom_txt10 = createStatText(renderer, "Mana : ", item->stats.mana.additive, font1, color);
    char* nom_txt11 = createStatText(renderer, "Mana Max : ", item->stats.manaMax.additive, font1, color);
    char* nom_txt12 = createStatText(renderer, "Attack : ", item->stats.attack.additive, font1, color);
    char* nom_txt13 = createStatText(renderer, "Defense : ", item->stats.defense.additive, font1, color);
    char* nom_txt14 = createStatText(renderer, "Speed : ", item->stats.speed.additive, font1, color);

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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &casePerso);
    SDL_RenderDrawRect(renderer, &caseArme);
    SDL_RenderDrawRect(renderer, &caseArmure);
    SDL_RenderDrawRect(renderer, &CaseActivable1);
    SDL_RenderDrawRect(renderer, &caseActivable2);
    SDL_RenderDrawRect(renderer, &statsPlayer);

    txt1->rect.x = statsPlayer.x + input->windowWidth * 0.01;
    txt1->rect.y = statsPlayer.y + input->windowHeight * 0.01;
    renderText(renderer, txt1);

    afficherText(renderer, txt1, txt2, statsPlayer, input);

    updateText(&txt1, renderer, nom_txt3, color);
    afficherText(renderer, txt2, txt1, statsPlayer, input);

    updateText(&txt2, renderer, nom_txt4, color);
    afficherText(renderer, txt1, txt2, statsPlayer, input);

    updateText(&txt1, renderer, nom_txt5, color);
    afficherText(renderer, txt2, txt1, statsPlayer, input);

    updateText(&txt2, renderer, nom_txt6, color);
    afficherText(renderer, txt1, txt2, statsPlayer, input);

    updateText(&txt1, renderer, nom_txt7, color);
    afficherText(renderer, txt2, txt1, statsPlayer, input);

    SDL_RenderDrawRect(renderer, &inv);
    SDL_RenderDrawRect(renderer, &statsItem);

    name->rect.x = statsItem.x + statsItem.x * 0.025;
    name->rect.y = statsItem.y + input->windowHeight * 0.005;
    renderText(renderer, name);

    updateText(&txt1, renderer, nom_txt8, color);
    txt1->rect.x = statsItem.x + input->windowWidth * 0.005;
    txt1->rect.y = name->rect.y + input->windowHeight * 0.04;
    renderText(renderer, txt1);

    updateText(&txt2, renderer, nom_txt9, color);
    afficherText(renderer, txt1, txt2, statsPlayer, input);

    updateText(&txt1, renderer, nom_txt10, color);
    afficherText(renderer, txt2, txt1, statsPlayer, input);

    updateText(&txt2, renderer, nom_txt11, color);
    afficherText(renderer, txt1, txt2, statsPlayer, input);

    updateText(&txt1, renderer, nom_txt12, color);
    afficherText(renderer, txt2, txt1, statsPlayer, input);

    updateText(&txt2, renderer, nom_txt13, color);
    afficherText(renderer, txt1, txt2, statsPlayer, input);

    updateText(&txt1, renderer, nom_txt14, color);
    afficherText(renderer, txt2, txt1, statsPlayer, input);

    SDL_RenderDrawRect(renderer, &descrItem);
    SDL_RenderDrawRect(renderer, &equiper);
}