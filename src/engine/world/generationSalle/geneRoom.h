/**
 * @file generoom.h
 * @brief Générateur de salles procédurales
 *
 * Ce module permet de générer des salles procédurales avec :
 * - Placement intelligent des tuiles
 * - Application de règles de construction
 * - Gestion des ouvertures et sorties
 *
 * @author Ubissauphte
 * @version 1.0
 * @date 2025
 */

#ifndef GENEROOM_H
#define GENEROOM_H

#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../genmap.h"
#include "fillGaps.h"
#include "grille.h"
#include "lissage.h"
#include "listeBlock.h"
#include "perlinNoise.h"

/*=== FONCTIONS DE SELECTION DE TUILES ===*/

/**
 * @brief Récupère un plafond aléatoire
 * @param listAllBlock Liste complète des blocs
 * @return Pointeur vers le bloc plafond sélectionné
 */
t_block *getPlafond(t_listeBlock **listAllBlock);

/**
 * @brief Sélectionne un sol adapté à une case
 * @param listAllBlock Liste complète des blocs
 * @param c Case cible
 * @return Pointeur vers le bloc sol sélectionné
 */
t_block *getSol(t_listeBlock **listAllBlock, t_case *c);

/**
 * @brief Récupère une décoration aléatoire
 * @param listAllBlock Liste complète des blocs
 * @return Pointeur vers le bloc décoration sélectionné
 */
t_block *getDeco(t_listeBlock **listAllBlock);

/**
 * @brief Sélectionne une tuile frontale adaptée
 * @param listAllBlock Liste complète des blocs
 * @param c Case cible
 * @return Pointeur vers le bloc frontal sélectionné
 */
t_block *getFrontale(t_listeBlock **listAllBlock, t_case *c);

/*=== REGLES DE CONSTRUCTION ===*/

/**
 * @brief Vérifie si un mur avant est nécessaire
 * @param c Case à analyser
 * @return SDL_TRUE si un mur avant est requis
 */
SDL_bool murAvant(t_case *c);

/**
 * @brief Détecte un angle droit à combler
 * @param c Case à analyser
 * @return SDL_TRUE si un angle droit est détecté
 */
SDL_bool angleDroit(t_case *c);

/**
 * @brief Détecte un angle gauche à combler
 * @param c Case à analyser
 * @return SDL_TRUE si un angle gauche est détecté
 */
SDL_bool angleGauche(t_case *c);

/**
 * @brief Détecte un arrondi inférieur droit
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool arrondiInferieurDroit(t_case *c);

/**
 * @brief Détecte un arrondi inférieur gauche
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool arrondiInferieurGauche(t_case *c);

/**
 * @brief Détecte un arrondi supérieur droit
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool arrondiSuperieurDroit(t_case *c);

/**
 * @brief Détecte un arrondi supérieur gauche
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool arrondiSuperieurGauche(t_case *c);

/**
 * @brief Vérifie si un mur arrière est nécessaire
 * @param c Case à analyser
 * @return SDL_TRUE si un mur arrière est requis
 */
SDL_bool murArriere(t_case *c);

/**
 * @brief Détecte un angle continu à gauche
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool angleContinueGauche(t_case *c);

/**
 * @brief Détecte un angle continu à droite
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool angleContinueDroit(t_case *c);

/**
 * @brief Détecte une bordure mur avant
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool bordureMurAvant(t_case *c);

/**
 * @brief Détecte une bordure mur gauche
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool bordureMurGauche(t_case *c);

/**
 * @brief Détecte une bordure mur droit
 * @param c Case à analyser
 * @return SDL_TRUE si le motif est présent
 */
SDL_bool bordureMurDroit(t_case *c);

/*=== FONCTIONS PRINCIPALES ===*/

/**
 * @brief Génère une salle complète
 * @param s Paramètres de la salle à générer
 * @return Grille générée
 */
t_grille *geneRoom(t_salle *s);

/**
 * @brief Applique les tuiles à la grille
 * @param listAllBlock Liste complète des blocs
 * @param g Grille cible
 */
void choixTiles(t_listeBlock **listAllBlock, t_grille *g);

/**
 * @brief Sauvegarde la carte générée
 * @param g Grille à sauvegarder
 */
void saveMap(t_grille *g);

/*=== GESTION DES OUVERTURES ===*/

/**
 * @brief Vérifie si une case est ouverte
 * @param c Case à vérifier
 * @param changementLigne Variation en ligne
 * @param changementColonne Variation en colonne
 * @return 1 si ouvert, 0 sinon
 */
int estOuvert(t_case *c, int changementLigne, int changementColonne);

/**
 * @brief Crée des ouvertures dans la grille
 * @param grille Grille à modifier
 * @param xdebut Position x de départ
 * @param ydebut Position y de départ
 * @param changementLigne Direction ligne
 * @param changementColonne Direction colonne
 * @param lb Liste des blocs disponibles
 * @return SDL_TRUE si succès
 */
SDL_bool trouerGrille(t_grille **grille, int xdebut, int ydebut,
                      int changementLigne, int changementColonne,
                      t_listeBlock *lb);

/**
 * @brief Place la sortie dans la salle
 * @param grille Grille cible
 * @param salle Paramètres de la salle
 * @param lab Liste des blocs disponibles
 */
void placerSortie(t_grille **grille, t_salle *salle, t_listeBlock **lab);

#endif