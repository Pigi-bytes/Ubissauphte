#ifndef EQUIPEMENTS_MANAGER_H
#define EQUIPEMENTS_MANAGER_H

#define MAX_ITEM_NAME 64

#include "../../../../debug.h"
#include "../../../../io/fichierLoader.h"
#include "../../../../utils/fonctionManager.h"
#include "../../../../utils/objectManager.h"
#include "../../../tileset.h"
#include "../../items/weapon.h"

/**
 * @enum itemsFlags
 * @brief Drapeaux définissant les propriétés d'un item
 */
typedef enum {
    ITEM_FLAG_STACKABLE = 1 << 0,   ///< L'item peut être empilé
    ITEM_FLAG_CONSUMABLE = 1 << 1,  ///< L'item peut être consommé
    ITEM_FLAG_EQUIPABLE = 1 << 2,   ///< L'item peut être équipé
    ITEM_FLAG_UNIQUE = 1 << 3       ///< L'item est unique
} itemsFlags;

/**
 * @enum equipementSlotType
 * @brief Types d'emplacements d'équipement disponibles
 */
typedef enum {
    SLOT_ARME,             ///< Emplacement pour les armes
    SLOT_ARMURE,           ///< Emplacement pour l'armure
    SLOT_ACTIVABLE1,       ///< Emplacement pour item activable
    SLOT_TALISMENT,        ///< Emplacement pour talisman
    TOTAL_EQUIPMENT_SLOTS  ///< Nombre total d'emplacements
} equipementSlotType;

/**
 * @struct t_statModifier
 * @brief Modificateurs de statistiques
 */
typedef struct {
    float additive;        ///< Bonus additif
    float multiplicative;  ///< Bonus multiplicatif
} t_statModifier;

/**
 * @struct t_stats
 * @brief Statistiques d'un personnage ou item
 */
typedef struct {
    t_statModifier healthMax;  ///< Modificateur de points de vie maximum
    t_statModifier attack;     ///< Modificateur d'attaque
    t_statModifier defense;    ///< Modificateur de défense
    t_statModifier speed;      ///< Modificateur de vitesse
} t_stats;

/**
 * @enum t_itemRarity
 * @brief Niveaux de rareté des items
 */
typedef enum {
    RARITY_COMMON,    ///< Item commun
    RARITY_UNCOMMON,  ///< Item peu commun
    RARITY_RARE,      ///< Item rare
    RARITY_EPIC,      ///< Item épique
    RARITY_LEGENDARY  ///< Item légendaire
} t_itemRarity;

/**
 * @struct t_item
 * @brief Définition d'un item
 */
typedef struct {
    char name[MAX_ITEM_NAME];    ///< Nom de l'item
    t_stats stats;               ///< Statistiques de l'item
    int id;                      ///< Identifiant unique
    unsigned int flags;          ///< Propriétés de l'item
    int validSlot[2];            ///< Emplacements valides
    t_arme* arme;                ///< Pointeur vers une arme si applicable
    t_fonctionParam* onDeEquip;  ///< Fonction appelée au déséquipement
    t_fonctionParam* onUse;      ///< Fonction appelée à l'utilisation
    char description[200];       ///< Description de l'item
    SDL_Texture* texture;        ///< Texture de l'item
    int indiceTexture;           ///< Index de la texture
    t_itemRarity rarity;         ///< Rareté de l'item
} t_item;

/**
 * @struct t_itemsStack
 * @brief Pile d'items identiques
 */
typedef struct {
    t_item* definition;  ///< Définition de l'item
    int quantite;       ///< Nombre d'items dans la pile
} t_itemsStack;

/**
 * @struct t_equipementSlotType
 * @brief Emplacement d'équipement
 */
typedef struct {
    equipementSlotType slotType;  ///< Type d'emplacement
    t_itemsStack* stack;          ///< Item équipé
} t_equipementSlotType;

/**
 * @struct t_inventaire
 * @brief Inventaire contenant les items
 */
typedef struct {
    t_objectManager* itemsStack;  ///< Gestionnaire des piles d'items
} t_inventaire;

/**
 * @struct t_charactere
 * @brief Données d'un personnage
 */
typedef struct {
    t_stats baseStats;           ///< Statistiques de base
    t_stats calculatedStats;     ///< Statistiques calculées
    t_equipementSlotType equipement[TOTAL_EQUIPMENT_SLOTS];  ///< Équipements
    t_inventaire* inventaire;    ///< Inventaire
    SDL_Texture* texture;        ///< Texture du personnage
    int level;                   ///< Niveau
    int experience;              ///< Points d'expérience
    int gold;                    ///< Or possédé
} t_charactere;

// Fonctions de gestion d'inventaire
/**
 * @brief Crée un nouvel inventaire vide
 * @return Pointeur vers l'inventaire créé
 */
t_inventaire* createInventaire();

/**
 * @brief Ajoute un objet à l'inventaire
 * @param inv Inventaire cible
 * @param item Item à ajouter
 * @param quantite Quantité à ajouter
 */
void inventaireAjoutObjet(t_inventaire* inv, t_item* item, int quantite);

/**
 * @brief Trouve une pile d'items dans l'inventaire
 * @param inv Inventaire à fouiller
 * @param item Item à trouver
 * @return Pointeur vers la pile si trouvée, NULL sinon
 */
t_itemsStack* inventaireFindStack(t_inventaire* inv, t_item* item);

/**
 * @brief Libère la mémoire d'un item
 * @param data Pointeur vers l'item à libérer
 */
void itemFree(void* data);

/**
 * @brief Fonction wrapper pour la libération d'item
 * @param data Pointeur vers l'item à libérer
 */
void itemFreeFunc(void* data);

/**
 * @brief Vérifie si un item possède un flag donné
 * @param itemFlags Flags de l'item
 * @param flag Flag à vérifier
 * @return 1 si le flag est présent, 0 sinon
 */
int hasFlag(int itemFlags, itemsFlags flag);

// Fonctions de gestion d'équipement
/**
 * @brief Équipe un item depuis l'inventaire
 * @param c Pointeur vers le joueur
 * @param inventoryIndex Index de l'item dans l'inventaire
 * @param slot Emplacement d'équipement ciblé
 */
void equiperEquipement(struct s_joueur** c, int inventoryIndex, equipementSlotType slot);

/**
 * @brief Déséquipe un item
 * @param c Pointeur vers le joueur
 * @param slot Emplacement à vider
 */
void desequiperEquipement(struct s_joueur** c, equipementSlotType slot);

/**
 * @brief Vérifie si une arme peut être équipée
 * @param arme Arme actuellement équipée
 * @param armeAjout Arme à équiper
 */
void peutEquiper(t_arme** arme, t_arme* armeAjout);

/**
 * @brief Wrapper pour la fonction peutEquiper
 * @param f Paramètres de la fonction
 */
void peutEquiperWrapper(t_fonctionParam* f);

/**
 * @brief Recalcule les statistiques après changement d'équipement
 * @param c Pointeur vers le joueur
 */
void equipementRecalculerStats(struct s_joueur** c);

// Fonctions d'utilisation
/**
 * @brief Utilise un item équipé
 * @param c Joueur
 * @param slot Emplacement de l'item à utiliser
 */
void equipementUse(struct s_joueur* c, equipementSlotType slot);

/**
 * @brief Fonction de soin
 * @param f Paramètres de la fonction
 */
void ajoutVie(t_fonctionParam* f);

/**
 * @brief Récupère un item aléatoire d'une rareté donnée
 * @param items Tableau d'items
 * @param count Nombre d'items dans le tableau
 * @param rarity Rareté souhaitée
 * @return Item choisi ou NULL
 */
t_item* getItemByRarity(t_item** items, int count, t_itemRarity rarity);

/**
 * @brief Détermine une rareté aléatoire selon des probabilités
 * @return Rareté déterminée
 */
t_itemRarity determineRarityByPercentage();

// Fonctions de debug
/**
 * @brief Affiche le contenu de l'inventaire
 * @param inv Inventaire à afficher
 */
void inventory_print(t_inventaire* inv);

/**
 * @brief Affiche l'équipement
 * @param c Joueur dont l'équipement est à afficher
 */
void equipment_print(struct s_joueur* c);

/**
 * @brief Trouve l'index d'un item dans l'inventaire
 * @param inv Inventaire à fouiller
 * @param item Item à trouver
 * @return Index de l'item ou -1 si non trouvé
 */
int findItemInventoryIndex(t_inventaire* inv, t_item* item);

// Fonctions de sauvegarde/chargement
/**
 * @brief Sauvegarde des items dans un fichier
 * @param item Items à sauvegarder
 * @param fichier Fichier cible
 * @param count Nombre d'items
 */
void item_save(t_item** item, t_fichier* fichier, int count);

/**
 * @brief Charge des items depuis un fichier
 * @param fichier Fichier source
 * @param tileset Tileset pour les textures
 * @param player Joueur propriétaire
 * @return Tableau d'items chargés
 */
t_item** item_load(t_fichier* fichier, t_tileset* tileset, t_joueur* player);

/**
 * @brief Libère la mémoire d'un tableau d'items
 * @param items Items à libérer
 * @param count Nombre d'items
 */
void free_item(t_item** items, int count);

/**
 * @brief Sauvegarde un inventaire dans un fichier
 * @param inv Inventaire à sauvegarder
 * @param fichier Fichier cible
 */
void inventory_save(t_inventaire* inv, t_fichier* fichier);

/**
 * @brief Charge un inventaire depuis un fichier
 * @param fichier Fichier source
 * @param item Items disponibles
 * @param count Nombre d'items disponibles
 * @return Inventaire chargé
 */
t_inventaire* inventory_load(t_fichier* fichier, t_item** item, int count);

#endif // EQUIPEMENTS_MANAGER_H