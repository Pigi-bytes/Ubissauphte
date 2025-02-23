
#include "fichierLoader.h"

void saveFichier(t_fichier *fichier, char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    t_block *Block;
    t_pairData *data;
    for (int i = 0; i < fichier->blockManager->count; i++) {
        Block = (t_block *)getObject(fichier->blockManager, i);
        for (int j = 0; j < Block->pairManager->count; j++) {
            data = (t_pairData *)getObject(Block->pairManager, j);
            fprintf(file, "%s: %s\n", data->key, data->value);
        }
        if (i != fichier->blockManager->count - 1)
            fprintf(file, "\n");
    }
    fclose(file);
}

void addBlock(t_fichier *fichier, t_block *Block) {
    addObject(fichier->blockManager, Block, getTypeIdByName(fichier->blockManager->registry, "BLOCK_TYPE"));
}

void addPairData(t_block *block, t_pairData *pair) {
    addObject(block->pairManager, pair, getTypeIdByName(block->pairManager->registry, "PAIR_TYPE"));
}

t_fichier *chargerFichier(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    t_fichier *fichier = malloc(sizeof(t_fichier));
    // Initialisation d'une zone de stockage pour le premier block

    t_typeRegistry *registre = createTypeRegistry();
    registerType(registre, blockFreeFunc, "BLOCK_TYPE");
    registerType(registre, pairFreeFunc, "PAIR_TYPE");

    fichier->blockManager = initObjectManager(registre);

    t_block *currentBlock = NULL;
    char line[MAX_LINE_LENGTH];

    // Lecture du fichier
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[strcspn(line, "\n")] = 0;
        // Fin du block en cas de ligne vide puis stockage dans la zone prévu pour
        if (strlen(line) == 0) {
            if (currentBlock) {
                addBlock(fichier, currentBlock);
                currentBlock = NULL;
            }
        } else {  // Allocation de mémoire pour chaque case du tableau t_block
            if (!currentBlock) {
                currentBlock = createNewBlock();
            }

            // Séparation de la cle et de la valeur grace au ':'
            char *separator = strchr(line, ':');
            if (separator) {
                *separator = '\0';            // On coupe la chaine ici ("cle\0 valeur") -> avec un char de fin de chaine
                char *key = line;             // Partie cle a gauche du ': '
                char *value = separator + 1;  // Partie valeur à droite du ': '

                key = key;
                value = value + (value[0] == ' ' ? 1 : 0);

                addPairData(currentBlock, createPairData(line, value));
            }
        }
    }
    if (currentBlock) {
        addBlock(fichier, currentBlock);
    }

    fclose(file);
    return fichier;
}

bool getValue(t_block *block, char *name, void *result, t_valueType type) {
    for (int i = 0; i < block->pairManager->count; i++) {
        t_pairData *pair = (t_pairData *)getObject(block->pairManager, i);
        if (strcmp(pair->key, name) == 0) {
            switch (type) {
                case INT:
                    *(int *)result = atoi(pair->value);
                    break;
                case FLOAT:
                    *(float *)result = atof(pair->value);
                    break;
                case DOUBLE:
                    *(double *)result = atof(pair->value);
                    break;
                case STRING:
                    strcpy((char *)result, pair->value);
                    break;
                default:
                    return false;
            }
            return true;
        }
    }
    return false;
}

t_block *createNewBlock() {
    t_block *block = malloc(sizeof(t_block));

    t_typeRegistry *registre = createTypeRegistry();
    registerType(registre, pairFreeFunc, "PAIR_TYPE");
    block->pairManager = initObjectManager(registre);
    return block;
}

t_pairData *createPairData(char *key, char *value) {
    t_pairData *pair = malloc(sizeof(t_pairData));

    pair->key = strdup(key);
    pair->value = strdup(value);
    
    return pair;
}

void blockFreeFunc(void *data) {
    t_block *block = (t_block *)data;
    freeObjectManager(block->pairManager);
    free(block);
}

void pairFreeFunc(void *data) {
    t_pairData *pair = (t_pairData *)data;
    free(pair->key);
    free(pair->value);
    free(pair);
}

void freeFichier(t_fichier *fichier) {
    if (fichier && fichier->blockManager) {
        freeObjectManager(fichier->blockManager);
    }
    free(fichier);
}
