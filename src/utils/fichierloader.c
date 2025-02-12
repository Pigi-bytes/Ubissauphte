#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"

#define MAX_LINE_LENGTH 100

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

void chargerFichier(t_fichier *fichier, char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fichier->nbBlocks = 0;
    fichier->listBlock = malloc(25 * sizeof(t_block *));
    if (!fichier->listBlock) {
        perror("Erreur d'allocation mémoire pour listBlock");
        exit(EXIT_FAILURE);
    }

    // Création du premier t_block
    char line[MAX_LINE_LENGTH];
    t_block *currentBlock = malloc(sizeof(t_block));
    if (!currentBlock) {
        perror("Erreur d'allocation mémoire pour t_block");
        exit(EXIT_FAILURE);
    }
    currentBlock->nbElement = 0;
    currentBlock->data = malloc(20 * sizeof(t_pairData *));
    if (!currentBlock->data) {
        perror("Erreur d'allocation mémoire pour les données du block");
        exit(EXIT_FAILURE);
    }
    fichier->listBlock[fichier->nbBlocks++] = currentBlock;

    // Lecture ligne par ligne et gestion des données
    while (fgets(line, sizeof(line), file)) {
        // Gestion des blocks séparés par des lignes vides
        if (line[0] == '\n') {
            if (currentBlock->nbElement > 0) {
                // Si un block a des éléments, on crée un nouveau block
                currentBlock = malloc(sizeof(t_block));
                if (!currentBlock) {
                    perror("Erreur d'allocation mémoire pour un nouveau t_block");
                    exit(EXIT_FAILURE);
                }
                currentBlock->nbElement = 0;
                currentBlock->data = malloc(20 * sizeof(t_pairData *));
                if (!currentBlock->data) {
                    perror("Erreur d'allocation mémoire pour les données du nouveau block");
                    exit(EXIT_FAILURE);
                }
                fichier->listBlock[fichier->nbBlocks++] = currentBlock;
            }

        } else {
            // Traitement des lignes contenant des paires clé/valeur
            char *colonPos = strchr(line, ':');
            if (colonPos) {
                t_pairData *pair = malloc(sizeof(t_pairData));
                if (!pair) {
                    perror("Erreur d'allocation mémoire pour t_pairData");
                    exit(EXIT_FAILURE);
                }
                pair->key = strndup(line, colonPos - line);                     // Clé
                pair->value = strndup(colonPos + 2, strlen(colonPos + 2) - 1);  // Valeur
                if (!pair->key || !pair->value) {
                    perror("Erreur d'allocation mémoire pour key ou value");
                    exit(EXIT_FAILURE);
                }
                currentBlock->data[currentBlock->nbElement++] = pair;
            }
        }
    }
    fclose(file);
}

// Recherche une donnée précise dans un block
t_pairData *getValue(t_block *block, char *name, void *result, char *type) {
    for (int j = 0; j < block->nbElement; j++) {
        if (strcmp(block->data[j]->key, name) == 0) {
            // Cast le résultat en fonction du type passé en paramètre
            if (strcmp(type, "int") == 0) {
                *(int *)result = atoi(block->data[j]->value);
            } else if (strcmp(type, "float") == 0) {
                *(float *)result = atof(block->data[j]->value);
            } else if (strcmp(type, "double") == 0) {
                *(double *)result = atof(block->data[j]->value);
            } else if (strcmp(type, "string") == 0) {
                strcpy((char *)result, block->data[j]->value);
            } else {
                fprintf(stderr, "Type non supporté: %s\n", type);
                return NULL;
            }
            return block->data[j];
        }
    }
    return NULL;
}

void freeFichier(t_fichier *fichier) {
    // Libère la mémoire pour chaque block et ses données
    for (int i = 0; i < fichier->nbBlocks; i++) {
        for (int j = 0; j < fichier->listBlock[i]->nbElement; j++) {
            free(fichier->listBlock[i]->data[j]);
        }
        free(fichier->listBlock[i]->data);
        free(fichier->listBlock[i]);
    }
    free(fichier->listBlock);
}

int main() {
    t_fichier fichier;
    chargerFichier(&fichier, "test.txt");

    int niveau;
    for (int i = 0; i < fichier.nbBlocks; i++) {
        if (getValue(fichier.listBlock[i], "Niveau", &niveau, "int")) {
            printf("Bloc %d - Niveau: %d\n", i, niveau);
        }
    }

    freeFichier(&fichier);
    return 0;
}
