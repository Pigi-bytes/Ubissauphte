#include <stdio.h>
#include <time.h>

#include "genmap.h"

void main() {
    int niveau = 100;
    clock_t start = clock();
    genMap(niveau);
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("Temps d'exécution de genMap: %.2f ms\n", duration);
}
