#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "../src/engine/world/genmap.h"

/*
 * Ce programme sert à tester le programme de genMap, il teste les points suivants :
 *  -> test de création d'une salle : test_initRoom()
 *  -> test de connexion de deux salles : test_connectRoom()
 *  -> test de génération d'une carte (plusieurs salles reliées les unes aux autres) : test_genMap()
 *  -> test de suppression d'un niveau : test_freeLevel()
 *  -> test de changement de scène : test_setScene()
 */

// fonction de test de création de salle
void test_initRoom() {
    t_salle* room = initRoom();

    CU_ASSERT_PTR_NOT_NULL(room);  // test d'existance
    CU_ASSERT_EQUAL(room->ID, 0);
    CU_ASSERT_PTR_NULL(room->droite);
    CU_ASSERT_PTR_NULL(room->gauche);
    CU_ASSERT_PTR_NULL(room->haut);
    CU_ASSERT_PTR_NULL(room->bas);

    free(room);
}

// fonction de test de connexion de salle
void test_connectRoom() {
    t_salle* room1 = initRoom();
    t_salle* room2 = initRoom();

    connectRoom(room1, room2, 0);               // on conecte les deux salles
    CU_ASSERT_PTR_EQUAL(room1->droite, room2);  // Vérification de la connexion droite
    CU_ASSERT_PTR_EQUAL(room2->gauche, room1);  // Vérification de la connexion gauche

    free(room1);
    free(room2);
}

// fonction de test de generation d'une map
void test_genMap() {
    SDL_Rect roomCoords[10];
    int numberRoom = 10;
    t_salle** roomList = genMap(numberRoom, roomCoords);

    CU_ASSERT_PTR_NOT_NULL(roomList);
    CU_ASSERT_EQUAL(roomList[0]->ID, 0);

    // vérification des salles non-nulles
    for (int i = 1; i < numberRoom; i++) {
        CU_ASSERT_PTR_NOT_NULL(roomList[i]);
    }

    // vérification des connexions entre les salles (doit être entre 1 et 4 connexions)
    for (int i = 0; i < numberRoom; i++) {
        t_salle* room = roomList[i];
        int connections = 0;
        if (room->droite) connections++;
        if (room->gauche) connections++;
        if (room->haut) connections++;
        if (room->bas) connections++;

        CU_ASSERT_TRUE(connections >= 1 && connections <= 4);
    }

    for (int i = 0; i < numberRoom; i++) {
        free(roomList[i]);
    }
    free(roomList);
}

// fonction de supression d'une map
void test_freeLevel() {
    SDL_Rect* roomCoords = malloc((sizeof(SDL_Rect)) * 3);
    t_salle** roomList = malloc((sizeof(t_salle*)) * 3);

    roomList[0] = initRoom();
    roomList[1] = initRoom();
    roomList[2] = initRoom();

    t_level* level = malloc((sizeof(t_level)) * 3);
    level->salles = roomList;
    level->rectcord = roomCoords;
    level->nbSalles = 3;

    freeLevel(level);
}

// fonction pour enregistrer les tests dans CUnit
int addTests() {
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Suite_TestGenMap", NULL, NULL);
    if (pSuite == NULL) {
        printf("Erreur lors de la création de la suite de tests.\n");
        return CU_get_error();
    }

    if (((NULL == CU_add_test(pSuite, "Test initRoom()", test_initRoom)) ||
         (NULL == CU_add_test(pSuite, "Test connectRoom()", test_connectRoom)) ||
         (NULL == CU_add_test(pSuite, "Test genMap()", test_genMap)) ||
         (NULL == CU_add_test(pSuite, "Test freeLevel()", test_freeLevel)))) {
        printf("Erreur lors de l'ajout des tests.\n");
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

int main() {
    // Initialisation de CUnit
    if (CUE_SUCCESS != CU_initialize_registry()) {
        printf("Erreur d'initialisation du registre CUnit.\n");
        return CU_get_error();
    }

    // Ajouter les tests
    if (addTests() != CUE_SUCCESS) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Lancer les tests
    CU_basic_set_mode(CU_BRM_VERBOSE);  // Mode détaillé pour voir tous les tests
    CU_basic_run_tests();

    // Libération des ressources de CUnit
    CU_cleanup_registry();
    return 0;
}
