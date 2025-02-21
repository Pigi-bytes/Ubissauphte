/**
 * @file fonctionManager.h
 * @brief Gestion de fonction à paramètres variables
 */
#ifndef FONCTION_MANAGER_H
#define FONCTION_MANAGER_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"

/**
 * @brief Ajoute automatiquement un NULL à la fin d'une liste d'arguments.
 * @param ... Liste d'arguments variadiques à passer à la fonction.
 * @return La liste des arguments fournie suivie de NULL.
 *
 * @note Évite d'ajouter manuellement NULL lors des appels de fonctions.
 *
 * // Appel sans la macro
 * exempleFonction("un", "deux", "trois", NULL);
 *
 * // Appel avec la macro
 * exempleFonction(FONCTION_PARAMS("un", "deux", "trois"));
 */
#define FONCTION_PARAMS(...) __VA_ARGS__, NULL

/**
 * @def GET_VALUE
 * @brief Macro utilitaire pour recupéré des parametres en valeur
 * @param f pointeur vers la fonction
 * @param index position des arguments dans le tableau
 * @param type Type de la valeur à récupérer
 */
#define GET_VALUE(f, index, type) (*(type*)(f)->param[index])

/**
 * @def GET_PTR
 * @brief Macro utilitaire pour recupéré des parametres en valeur
 * @param f pointeur vers la fonction
 * @param index position des arguments dans le tableau
 * @param type Type du pointeur à récupérer
 */
#define GET_PTR(f, index, type) ((type)(f)->param[index])

/**
 * @struct t_fonctionParam
 * @brief Structure contenant les informations d'une fonction
 */
typedef struct fonction {
    void (*fonction)(struct fonction*);  ///< Pointeur sur la fonction de callback qui prend un t_fonctionParam et renvoie un void
    void** param;                        ///< Tableau de pointeurs vers les parametres
    int nb_param;                        ///< Nombre de parametres
} t_fonctionParam;

/**
 * @brief Crée un t_fonctionParam
 * @param f Pointeur sur fonction de type t_fonctionParam qui renvoie void
 * @param ... Argument de f
 * @return Pointeur vers la fonctionParam
 * @warning ... (Doit se terminer par une sentinelle NULL)
 */
t_fonctionParam* creerFonction(void (*f)(t_fonctionParam*), ...);

/**
 * @brief Execute la fonctionParam
 * @param func Pointeur vers le fonctionParam à exécuter
 */
void callFonction(t_fonctionParam* funct);

/**
 * @brief Libere la mémoire d'une fonctionParametrable
 * @param fonctParam Double pointeur vers la fonction a liberer
 */
void freeFonction(t_fonctionParam** fonctParam);

#endif