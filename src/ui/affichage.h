#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <SDL2/SDL.h>

#include "../engine/entities/systems/equipementsManager.h"
#include "../io/imageLoader.h"
#include "../io/input.h"
#include "button.h"
#include "text.h"

#define window_width 1280
#define window_height 960

#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef struct
{
    SDL_Rect rect;
    SDL_Texture *texture;
    TTF_Font *font;
    SDL_Color color;
    char *text;
    int is_visible;
} UI_Element;

typedef struct
{
    // Éléments de base (votre structure originale)
    UI_Element player_panel;
    UI_Element caseArme;
    UI_Element caseArmure;
    UI_Element CaseActivable1;
    UI_Element caseActivable2;
    UI_Element statsPlayer;
    UI_Element inventory_panel;
    UI_Element statsItem;
    UI_Element descrItem;
    UI_Element equiper;
    // UI_Element *inventory_slots;

    int nbItems;
    SDL_Color color;

    // Références externes
    t_character *character;
    t_item **items;
    t_input *input_ref;
    TTF_Font *item_font;
    TTF_Font *descr_font;

    t_text *text_player[7];
    t_text *text_item[7];
    t_text *text_descr;
    t_text *description[15];

    char *nom_txt_item[7];
    char *nom_txt_player[7];
    char descr[50];
    int count_descr;
    SDL_Color color_txt;

    int scrollY;
    int maxScrollY;

    t_button** inventory_buttons; // Remplacer UI_Element* inventory_slots
    int selectedItemIndex;


} InventoryUI;

typedef struct {
    InventoryUI* ui;
    int index;
} ItemClickData;


void inventoryUI_Init(InventoryUI *ui, SDL_Renderer *renderer, t_character *c, t_item **items, t_input *input, int nbItems);
void inventoryUI_Update(InventoryUI *ui, SDL_Renderer *renderer, t_input *input);
void inventoryUI_Render(InventoryUI *ui, SDL_Renderer *renderer);

void calculCasePlayer(SDL_Rect *casePlayer, t_input *input, char *nom);
void calculCaseSlots(SDL_Rect *comp, SDL_Rect *slot, t_input *input, char *nom1, char *nom2);
void calculDescrStatsPlayer(SDL_Rect *slotDroit, SDL_Rect *slotBas, SDL_Rect *casePlayer, SDL_Rect *caseStatsPlayer, t_input *input);
void calculInventaire(SDL_Rect *Inv, SDL_Rect *statsPlayer, t_input *input);
void calculStatsItem(SDL_Rect *inv, SDL_Rect *statsItem, t_input *input);
void caculDescrItem(SDL_Rect *statsItem, SDL_Rect *descrItem, t_input *input);

#endif