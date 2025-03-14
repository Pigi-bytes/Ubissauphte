#include "affichage.h"

void calculPlayer(SDL_Rect* Player, t_input* input, char* nom) {
    if (strcmp(nom, "Perso") == 0) {
        Player->h = input->windowWidth * 0.14;
        Player->w = input->windowWidth * 0.16;
        Player->x = input->windowWidth * 0.02;
        Player->y = input->windowHeight * 0.08;
    }
}
void calculSlot(SDL_Rect* slot, SDL_Rect* comp, t_input* input, char* nom1, char* nom2) {
    if (!strcmp(nom2, "arme") && !strcmp(nom1, "Perso")) {
        comp->h = slot->h / 2 - slot->h * 0.05;
        comp->w = slot->w / 2;
        comp->x = slot->w + slot->x + input->windowWidth * 0.04;
        comp->y = slot->y;
    }
    if (!strcmp(nom2, "armure") && !strcmp(nom1, "arme")) {
        comp->h = slot->h;
        comp->w = slot->w;
        comp->x = slot->x;
        comp->y = slot->h + input->windowHeight * 0.08 + input->windowWidth * 0.13 * 0.1;
    }

    if (!strcmp(nom2, "activable1") && !strcmp(nom1, "Perso")) {
        comp->h = slot->h / 2;
        comp->w = slot->w / 2 - slot->w * 0.05;
        comp->x = slot->x;
        comp->y = slot->h + slot->y + input->windowHeight * 0.04;
    }

    if (!strcmp(nom2, "activable2") && !strcmp(nom1, "activable1")) {
        comp->h = slot->h;
        comp->w = slot->w;
        comp->x = slot->w + input->windowWidth * 0.02 + input->windowWidth * 0.185 * 0.1;
        comp->y = slot->y;
    }
}

void calculDescrPlayer(SDL_Rect* slot1, SDL_Rect* slot2, SDL_Rect* player, SDL_Rect* descr, t_input* input) {
    descr->h = player->h + slot2->h;
    descr->w = player->w + slot1->w + input->windowWidth * 0.04;
    descr->x = player->x;
    descr->y = slot2->y + slot2->h + input->windowHeight * 0.04;
}

void calculInventaire(SDL_Rect* inv, SDL_Rect* descr, t_input* input) {
    inv->h = descr->h * 2 + input->windowHeight * 0.08;
    inv->w = descr->w * 1.5;
    inv->x = descr->w + input->windowWidth * 0.04;
    inv->y = input->windowHeight * 0.08;
}

void calculDescrItem(SDL_Rect* inv, SDL_Rect* descrItem, t_input* input) {
    descrItem->h = inv->h / 2 - input->windowHeight * 0.01;
    descrItem->w = inv->w / 2.5;
    descrItem->x = inv->x + inv->w + input->windowWidth * 0.04;
    descrItem->y = inv->y;
}

void caculDescr(SDL_Rect* descrItem, SDL_Rect* descr, t_input* input) {
    descr->h = descrItem->h * 2 / 3 - input->windowHeight * 0.01;
    descr->w = descrItem->w;
    descr->x = descrItem->x;
    descr->y = descrItem->h + descrItem->y + input->windowHeight * 0.03;
}

void calculEquiper(SDL_Rect* descrItem, SDL_Rect* descr, SDL_Rect* Equiper, t_input* input) {
    Equiper->h = descrItem->h / 3 - input->windowHeight * 0.03;
    Equiper->w = descr->w;
    Equiper->x = descr->x;
    Equiper->y = descr->y + descr->h + input->windowHeight * 0.03;
}

void afficherInventaire(SDL_Renderer* renderer, t_input* input) {
    SDL_Rect casePerso;
    SDL_Rect arme;
    SDL_Rect armure;
    SDL_Rect activable1;
    SDL_Rect activable2;
    SDL_Rect descrPlayer;
    SDL_Rect inv;
    SDL_Rect descrItem;
    SDL_Rect descr;
    SDL_Rect equiper;

    calculPlayer(&casePerso, input, "Perso");
    calculSlot(&casePerso, &arme, input, "Perso", "arme");
    calculSlot(&arme, &armure, input, "arme", "armure");
    calculSlot(&casePerso, &activable1, input, "Perso", "activable1");
    calculSlot(&activable1, &activable2, input, "activable1", "activable2");
    calculDescrPlayer(&arme, &activable1, &casePerso, &descrPlayer, input);
    calculInventaire(&inv, &descrPlayer, input);
    calculDescrItem(&inv, &descrItem, input);
    caculDescr(&descrItem, &descr, input);
    calculEquiper(&descrItem, &descr, &equiper, input);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &casePerso);
    SDL_RenderDrawRect(renderer, &arme);
    SDL_RenderDrawRect(renderer, &armure);
    SDL_RenderDrawRect(renderer, &activable1);
    SDL_RenderDrawRect(renderer, &activable2);
    SDL_RenderDrawRect(renderer, &descrPlayer);
    SDL_RenderDrawRect(renderer, &inv);
    SDL_RenderDrawRect(renderer, &descrItem);
    SDL_RenderDrawRect(renderer, &descr);
    SDL_RenderDrawRect(renderer, &equiper);
}