#ifndef HEADER_H
#define HEADER_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"

/**
 * @def MAX_LENGTH
 * @brief Taille maximal pour le nom d'une fonction
 */
#define MAX_LENGTH 100

/**
 * @def CREER_FONCTION
 * @brief Macro utilitaire pour crée une fonction sans precisé la sentinelle NULL
 * @param name Nom de la fonction
 * @param f Pointeur sur fonction de type t_fonctionParam qui renvoie void
 * @param ... Argument de f
 */
#define CREER_FONCTION(name, f, ...) creerFonction(name, f, __VA_ARGS__, NULL)

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
    char nom[MAX_LENGTH];                ///< Nom de la fonction permettant de l'identifier
    void** param;                        ///< Tableau de pointeurs vers les parametres
    int nb_param;                        ///< Nombre de parametres
} t_fonctionParam;

/**
 * @brief Crée un t_fonctionParam
 * @param name Nom de la fonction
 * @param f Pointeur sur fonction de type t_fonctionParam qui renvoie void
 * @param ... Argument de f
 * @return Pointeur vers la fonctionParam
 * @warning ... (Doit se terminer par une sentinelle NULL)
 */
t_fonctionParam* creerFonction(char* name, void (*f)(t_fonctionParam*), ...);

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