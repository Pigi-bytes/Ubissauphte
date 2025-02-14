#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} t_pairData;

typedef struct {
    int nbElement;
    t_pairData **data;
} t_block;

typedef struct {
    t_block **listBlock;
    int nbBlocks;
} t_fichier;

void chargerFichier(t_fichier *fichier, char *filename);
t_pairData *getValue(t_block *block, char *name, void *result, char *type);
void freeFichier(t_fichier *fichier);
void enregistrerDonnees(char *filename,int nbBlock);