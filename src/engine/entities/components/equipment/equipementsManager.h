#ifndef EQUIPEMENTS_MANAGER_H
#define EQUIPEMENTS_MANAGER_H

#define MAX_ITEM_NAME 64

#include "../../../../debug.h"
#include "../../../../io/fichierLoader.h"
#include "../../../../utils/fonctionManager.h"
#include "../../../../utils/objectManager.h"
#include "../../../tileset.h"
#include "../../items/weapon.h"

struct s_joueur;

typedef enum {
    ITEM_FLAG_STACKABLE = 1 << 0,
    ITEM_FLAG_CONSUMABLE = 1 << 1,
    ITEM_FLAG_EQUIPABLE = 1 << 2,
    ITEM_FLAG_UNIQUE = 1 << 3
} itemsFlags;

typedef enum {
    SLOT_ARME,
    SLOT_ARMURE,
    SLOT_ACTIVABLE1,
    SLOT_TALISMENT,
    TOTAL_EQUIPMENT_SLOTS
} equipementSlotType;

typedef struct {
    float additive;
    float multiplicative;
} t_statModifier;

typedef struct {
    t_statModifier health;
    t_statModifier healthMax;
    t_statModifier mana;
    t_statModifier manaMax;
    t_statModifier attack;
    t_statModifier defense;
    t_statModifier speed;
} t_stats;

typedef struct {
    char name[MAX_ITEM_NAME];
    t_stats stats;
    int id;
    unsigned int flags;  // Propriete
    int validSlot[2];    // Ou il va
    t_arme* arme;
    t_fonctionParam* onDeEquip;
    t_fonctionParam* onUse;
    char description[200];
    SDL_Texture* texture;
    int indiceTexture;
} t_item;

typedef struct {
    t_item* definition;
    int quantite;
} t_itemsStack;

typedef struct {
    equipementSlotType slotType;
    t_itemsStack* stack;

} t_equipementSlotType;

typedef struct {
    t_objectManager* itemsStack;
} t_inventaire;

typedef struct {
    t_stats baseStats;
    t_stats calculatedStats;

    t_equipementSlotType equipement[TOTAL_EQUIPMENT_SLOTS];
    t_inventaire* inventaire;

    SDL_Texture* texture;

    int level;
    int experience;
    int gold;
} t_charactere;

// Gestion Inventaire
t_inventaire* createInventaire();
void inventaireAjoutObjet(t_inventaire* inv, t_item* item, int quantite);
t_itemsStack* inventaireFindStack(t_inventaire* inv, t_item* item);
void itemFree(void* data);
void itemFreeFunc(void* data);
int hasFlag(int itemFlags, itemsFlags flag);

// Gestion equipement
void equiperEquipement(struct s_joueur** c, int inventoryIndex, equipementSlotType slot);
void desequiperEquipement(struct s_joueur** c, equipementSlotType slot);
void peutEquiper(t_arme** arme, t_arme* armeAjout);
void peutEquiperWrapper(t_fonctionParam* f);

// Gestion Interaction
void equipementRecalculerStats(struct s_joueur** c);

// fonction use
void equipementUse(struct s_joueur* c, equipementSlotType slot);

// Debug
void inventory_print(t_inventaire* inv);
void equipment_print(struct s_joueur* c);

void item_save(t_item** item, t_fichier* fichier, int count);
t_item** item_load(t_fichier* fichier, t_tileset* tileset, t_joueur* player);
void free_item(t_item** items, int count);

void inventory_save(t_inventaire* inv, t_fichier* fichier);
t_inventaire* inventory_load(t_fichier* fichier, t_item** item, int count);

#endif
