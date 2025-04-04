#ifndef AFFICHAGE_H
#define AFFICHAGE_H

/**
 * @file affichage.h
 * @brief Gestionnaire de l'interface d'inventaire et d'équipement
 *
 * Ce module gère l'affichage et l'interaction avec l'inventaire du joueur.
 * Il permet de visualiser, équiper et gérer les items ainsi que leurs statistiques.
 */

#include <SDL2/SDL.h>

#include "../context.h"
#include "../engine/entities/components/equipment/equipementsManager.h"
#include "../engine/entities/player.h"
#include "../io/imageLoader.h"
#include "../io/input.h"
#include "../utils/fscene.h"
#include "button.h"
#include "text.h"

/**
 * @def window_width
 * @brief Largeur par défaut de la fenêtre en pixels
 */
#define window_width 1280

/**
 * @def window_height
 * @brief Hauteur par défaut de la fenêtre en pixels
 */
#define window_height 960

/**
 * @def CLAMP
 * @brief Limite une valeur entre un minimum et un maximum
 * @param x Valeur à contraindre
 * @param min Valeur minimale
 * @param max Valeur maximale
 * @return Valeur contrainte entre min et max
 */

/**
 * @struct UI_Element
 * @brief Élément de base de l'interface utilisateur
 *
 * Structure générique pour tout élément graphique de l'interface
 */
typedef struct {
    SDL_Rect rect;         ///< Rectangle définissant la position et taille
    SDL_Texture* texture;  ///< Texture de l'élément
    TTF_Font* font;        ///< Police utilisée
    SDL_Color color;       ///< Couleur de l'élément
    t_button* button;      ///< Bouton associé si interactif
    char* text;            ///< Texte à afficher
} UI_Element;

/**
 * @struct t_extern
 * @brief Données externes nécessaires à l'interface
 *
 * Regroupe les références aux données du jeu et ressources partagées
 */
typedef struct {
    t_joueur* character;   ///< Référence au joueur
    t_item** items;        ///< Liste des items disponibles
    TTF_Font* item_font;   ///< Police pour les items
    TTF_Font* descr_font;  ///< Police pour les descriptions
} t_extern;

/**
 * @struct t_ecritures
 * @brief Gestionnaire des textes de l'interface
 *
 * Centralise tous les textes affichés dans l'interface
 */
typedef struct {
    t_text* text_player[7];   ///< Textes des statistiques du joueur
    t_text* text_item[7];     ///< Textes des statistiques de l'item
    t_text* text_descr;       ///< Titre de la description
    t_text* description[15];  ///< Lignes de description de l'item

    char* nom_txt_item[7];    ///< Labels des stats d'items
    char* nom_txt_player[7];  ///< Labels des stats du joueur
    char descr[50];           ///< Buffer de description temporaire
    int count_descr;          ///< Nombre de lignes de description
    SDL_Color color_txt;      ///< Couleur du texte
} t_ecritures;

/**
 * @struct t_elements
 * @brief Collection des éléments graphiques de l'interface
 *
 * Regroupe tous les éléments visuels composant l'interface
 */
typedef struct {
    UI_Element player_panel;      ///< Panneau d'informations du joueur
    UI_Element caseArme;          ///< Emplacement d'arme
    UI_Element caseArmure;        ///< Emplacement d'armure
    UI_Element CaseActivable1;    ///< Premier emplacement d'objet activable
    UI_Element caseActivable2;    ///< Second emplacement d'objet activable
    UI_Element statsPlayer;       ///< Panneau des statistiques du joueur
    UI_Element inventory_panel;   ///< Grille d'inventaire
    UI_Element statsItem;         ///< Panneau des statistiques de l'item
    UI_Element descrItem;         ///< Zone de description de l'item
    UI_Element equiper;           ///< Bouton pour équiper l'item
    UI_Element* inventory_slots;  ///< Tableau des emplacements d'inventaire
} t_elements;

/**
 * @struct InventoryUI
 * @brief Structure principale de l'interface d'inventaire
 *
 * Coordonne tous les aspects de l'interface : affichage, état et interactions
 */
typedef struct {
    int nbItems;         ///< Nombre d'items dans l'inventaire
    SDL_Color color;     ///< Couleur de base de l'interface
    t_elements* elems;   ///< Éléments graphiques
    t_extern* ext;       ///< Données externes
    t_ecritures* ecrit;  ///< Gestionnaire de textes
    int scrollY;         ///< Position de défilement vertical
    int maxScrollY;      ///< Limite de défilement
    int width;           ///< Largeur actuelle de l'interface
    int height;          ///< Hauteur actuelle de l'interface
    int peutEquiper;     ///< Flag indiquant si l'équipement est possible
    int onSlot;          ///< Index du slot sélectionné
    int j;               ///< Index temporaire pour le calcul de position
    t_item* itemclique;  ///< Item actuellement sélectionné
} InventoryUI;

// Fonctions d'initialisation et cycle de vie
/**
 * @brief Initialise ou met à jour l'interface d'inventaire
 * @param ui2 Interface existante à mettre à jour (NULL pour nouvelle)
 * @param c Joueur propriétaire de l'inventaire
 * @param context Contexte global du jeu
 * @return Interface initialisée ou mise à jour
 */
InventoryUI* inventoryUI_Init(InventoryUI* ui2, t_joueur* c, t_context* context);

/**
 * @brief Met à jour la logique de l'interface
 * @param ui Interface à mettre à jour
 * @param context Contexte du jeu
 */
void inventoryUI_Update(InventoryUI* ui, t_context* context);

/**
 * @brief Effectue le rendu de l'interface
 * @param ui Interface à afficher
 * @param context Contexte du jeu
 */
void inventoryUI_Render(InventoryUI* ui, t_context* context);

/**
 * @brief Crée le texte pour afficher un nombre d'items
 * @param renderer Le renderer SDL
 * @param font La police à utiliser
 * @param count Le nombre à afficher
 * @return Le texte créé
 */
t_text* createStackCountText(SDL_Renderer* renderer, TTF_Font* font, int count);

/**
 * @brief Affiche le nombre d'items dans une pile
 * @param renderer Le renderer SDL
 * @param font La police à utiliser
 * @param count Le nombre à afficher
 * @param itemRect Position de l'item
 */
void renderStackCount(SDL_Renderer* renderer, TTF_Font* font, int count, SDL_Rect itemRect);

/**
 * @brief Calcule la position du portrait du joueur
 * @param casePlayer Rectangle de destination
 * @param input Gestionnaire d'entrées
 * @param nom Identifiant de la case
 */
void calculCasePlayer(SDL_Rect* casePlayer, t_input* input, char* nom);

/**
 * @brief Calcule la position des slots d'équipement
 * @param comp Rectangle de référence
 * @param slot Rectangle de destination
 * @param input Gestionnaire d'entrées
 * @param nom1 Identifiant du composant parent
 * @param nom2 Identifiant du slot
 */
void calculCaseSlots(SDL_Rect* comp, SDL_Rect* slot, t_input* input, char* nom1, char* nom2);

/**
 * @brief Calcule la position du panneau de stats
 * @param slotDroit Slot de droite
 * @param slotBas Slot du bas
 * @param casePlayer Portrait du joueur
 * @param caseStatsPlayer Rectangle de destination
 * @param input Gestionnaire d'entrées
 */
void calculDescrStatsPlayer(SDL_Rect* slotDroit, SDL_Rect* slotBas, SDL_Rect* casePlayer,
                            SDL_Rect* caseStatsPlayer, t_input* input);

/**
 * @brief Calcule la position du panneau d'inventaire
 * @param inv Rectangle de destination
 * @param statsPlayer Panneau de stats
 * @param input Gestionnaire d'entrées
 */
void calculInventaire(SDL_Rect* inv, SDL_Rect* statsPlayer, t_input* input);

/**
 * @brief Calcule la position du panneau de stats d'item
 * @param inv Panneau d'inventaire
 * @param statsItem Rectangle de destination
 * @param input Gestionnaire d'entrées
 */
void calculStatsItem(SDL_Rect* inv, SDL_Rect* statsItem, t_input* input);

/**
 * @brief Calcule la position de la description d'item
 * @param statsItem Panneau de stats d'item
 * @param descrItem Rectangle de destination
 * @param input Gestionnaire d'entrées
 */
void caculDescrItem(SDL_Rect* statsItem, SDL_Rect* descrItem, t_input* input);

/**
 * @brief Crée une ligne de texte pour une statistique
 * @param statLabel Nom de la statistique
 * @param additive Valeur additive
 * @param multiplicative Valeur multiplicative
 * @return La chaîne formatée
 */
char* createStatLine(const char* statLabel, float additive, float multiplicative);

/**
 * @brief Calcule la position d'un slot d'inventaire
 * @param item Rectangle de destination
 * @param inv Panneau d'inventaire
 * @param comp Rectangle de référence
 * @param nb Nombre total d'items
 * @param ind Index de l'item
 * @param input Gestionnaire d'entrées
 */
void calculerItem(SDL_Rect* item, SDL_Rect inv, SDL_Rect* comp, int nb, int ind, t_input* input);

/**
 * @brief Affiche un texte
 * @param renderer Le renderer SDL
 * @param txt1 Premier texte (référence)
 * @param txt2 Second texte à afficher
 * @param input Gestionnaire d'entrées
 */
void afficherText(SDL_Renderer* renderer, t_text* txt1, t_text* txt2, t_input* input);

/**
 * @brief Met à jour l'interface après redimensionnement
 * @param input Gestionnaire d'entrées
 * @param ui Interface à mettre à jour
 */
void update(t_input* input, InventoryUI* ui);

/**
 * @brief Met à jour le défilement de l'inventaire
 * @param ui Interface à mettre à jour
 * @param input Gestionnaire d'entrées
 */
void updateScroll(InventoryUI* ui, t_input* input);

/**
 * @brief Wrapper pour la création de description
 * @param f Paramètres de la fonction
 */
void creerDescrWrapper(t_fonctionParam* f);

/**
 * @brief Libère la mémoire de l'interface
 * @param elt Pointeur vers l'interface à libérer
 */
void freeInv(void* elt);

/**
 * @brief Gère les entrées de l'inventaire
 * @param input Gestionnaire d'entrées
 * @param player Joueur
 * @param sceneController Contrôleur de scènes
 */
void handleInputInventaire(t_input* input, t_joueur* player, t_sceneController* sceneController);

#endif  // AFFICHAGE_H