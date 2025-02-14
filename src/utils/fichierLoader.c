#include "fichierLoader.h"

// #include "../debug.h"

#define MAX_LINE_LENGTH 100

/*Type: 0
Nom: Épée de feu
Niveau: 10
Quantite: -1
PV Max Modifier: 1.10
Mana Max Modifier: 1.05
Speed Modifier: 1.15
Attack Modifier: 1.25
Defense Modifier: 1.10
PV Max: 50
Mana Max: 20
Speed: 5
Attack: 15
Defense: 2*/

// Chargement des donnees dans un fichier text
void enregistrerDonnees(char *filename, int nbBlock) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    char key[20];
    char value[20];

    // écriture des données bloque par bloque
    printf("rentrez 0 pour chaque fin de bloque\n");
    for (int i = 0; i < nbBlock; i++) {
        do {
            printf("rentrez la cle : ");
            scanf("%[^\n]", key);
            getchar();
            printf("rentrez la value : ");
            scanf("%[^\n]", value);
            getchar();
            // Marque la fin d'un bloque
            if (key[0] == '0')
                fprintf(file, "\n");
            else
                fprintf(file, "%s: %s\n", key, value);

        } while (key[0] != '0');
    }

    fclose(file);
}

void compterLigne(char *filename, int *countLine, int *countBlock) {
    (*countLine) = 0;
    (*countBlock) = 0;

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if (line[0] == '\n')
            (*countBlock)++;
        if (*countBlock == 0)
            (*countLine)++;
    }
    (*countLine)++;
    fclose(file);
}

void chargerFichier(t_fichier *fichier, char *filename) {
    int nbLine, nbBlock;
    compterLigne(filename, &nbLine, &nbBlock);
    printf("%d\n", nbBlock);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fichier->nbBlocks = 0;
    fichier->listBlock = malloc(nbLine * sizeof(t_block *));
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
    currentBlock->data = malloc(nbLine * sizeof(t_pairData *));
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
                // Si le block courent a des éléments, on crée un nouveau block
                currentBlock = malloc(sizeof(t_block));
                if (!currentBlock) {
                    perror("Erreur d'allocation mémoire pour un nouveau t_block");
                    exit(EXIT_FAILURE);
                }
                currentBlock->nbElement = 0;
                currentBlock->data = malloc(nbLine * nbBlock * sizeof(t_pairData *));
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
                pair->key = strndup(line, colonPos - line);
                pair->value = strndup(colonPos + 2, strlen(colonPos + 1));
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
    for (int i = 0; i < fichier->nbBlocks; i++) {
        for (int j = 0; j < fichier->listBlock[i]->nbElement; j++) {
            free(fichier->listBlock[i]->data[j]->key);
            free(fichier->listBlock[i]->data[j]->value);
            free(fichier->listBlock[i]->data[j]);
        }
        free(fichier->listBlock[i]->data);
        free(fichier->listBlock[i]);
    }
    free(fichier->listBlock);
}