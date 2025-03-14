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

void afficherInventaire(SDL_Renderer* renderer, t_input* input) {
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
    SDL_RenderDrawRect(renderer, &inv);
    SDL_RenderDrawRect(renderer, &statsItem);
    SDL_RenderDrawRect(renderer, &descrItem);
    SDL_RenderDrawRect(renderer, &equiper);
}