#include <stdarg.h>

typedef struct {
    void (**fonction)(int nbParam, ...);
    int nbFonct;
} t_fonct;

t_fonct *initFonction(void);
void freeFonction(t_fonct **fonct);
void addFunction(t_fonct *fonct, void (*f)(int, ...));