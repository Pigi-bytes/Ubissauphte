#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>

#include "../src/utils/fscene.h"

/*
 *Ce programme sert à tester le programme de fscene, il teste les points suivants;
 *  -> test de création de scéne : test_createScene()
 *  -> test d'ajout d'un objet dans la scène : test_sceneAddObject()
 *  -> test de supression d'un objet d'une scène : test_sceneRemoveObject()
 *  -> test d'ajout d'une fonction dans une scène : test_sceneRegisterFunction()
 *  -> test de changement de scène : test_setScene()
 */

// Fonction sans aucun effet, juste pour tester l'enregistrement et l'appel de fonction
void fonctionTest(t_fonctionParam* param) {
}

// fonction de libération pour un objet de type int
void freeInt(void* ptr) {
    free(ptr);
}

// test de création d'une scéne
void test_createScene() {
    t_typeRegistry* registry = createTypeRegistry();
    t_objectManager* manager = initObjectManager(registry);

    t_scene* scene = createScene(manager, "Scene 1");

    CU_ASSERT_PTR_NOT_NULL(scene);
    CU_ASSERT_STRING_EQUAL(scene->name, "Scene 1");
    CU_ASSERT_PTR_NOT_NULL(scene->objectManager);
    CU_ASSERT_EQUAL(scene->nbFonctions[RENDER_GAME], 0);

    freeScene(scene);
}

// fonction qui teste l'ajout d'un object dans une scène
void test_sceneAddObject() {
    t_typeRegistry* registry = createTypeRegistry();
    t_objectManager* manager = initObjectManager(registry);
    t_scene* scene = createScene(manager, "Scene 1");

    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 42;

    uint8_t intTypeId = registerType(manager->registry, freeInt, "int");

    sceneAddObject(scene, newInt, intTypeId);

    CU_ASSERT_EQUAL(scene->objectManager->count, 1);

    freeScene(scene);
}

// fonction qui teste la supression d'un object dans une scène
void test_sceneRemoveObject() {
    t_typeRegistry* registry = createTypeRegistry();
    t_objectManager* manager = initObjectManager(registry);
    t_scene* scene = createScene(manager, "Scene 1");

    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 42;

    uint8_t intTypeId = registerType(manager->registry, freeInt, "int");

    sceneAddObject(scene, newInt, intTypeId);

    // Supprimer l'objet de la scène
    sceneRemoveObject(scene, newInt);

    CU_ASSERT_EQUAL(scene->objectManager->count, 0);

    freeScene(scene);
}

// fonction qui teste l'ajout d'une fonction dans une scène
void test_sceneRegisterFunction() {
    t_typeRegistry* registry = createTypeRegistry();
    t_objectManager* manager = initObjectManager(registry);
    t_scene* scene = createScene(manager, "Scene 1");

    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 42;

    uint8_t intTypeId = registerType(manager->registry, freeInt, "int");

    sceneAddObject(scene, newInt, intTypeId);

    sceneRegisterFunction(scene, intTypeId, RENDER_GAME, fonctionTest, 0, NULL);

    CU_ASSERT_EQUAL(scene->nbFonctions[RENDER_GAME], 1);

    freeScene(scene);
}

// fonction qui teste le changement de scène
void test_setScene() {
    t_sceneController* controller = initSceneController();
    t_typeRegistry* registry = createTypeRegistry();
    t_objectManager* manager = initObjectManager(registry);

    t_typeRegistry* registry2 = createTypeRegistry();
    t_objectManager* manager2 = initObjectManager(registry2);

    t_scene* scene1 = createScene(manager, "Scene 1");
    t_scene* scene2 = createScene(manager2, "Scene 2");

    addScene(controller, scene1);
    addScene(controller, scene2);

    // Définir la scène courante
    setScene(controller, "Scene 2");

    CU_ASSERT_PTR_EQUAL(getCurrentScene(controller), scene2);

    freeSceneController(&controller);
}

// fonction pour enregistrer les tests dans CUnit
int addTests() {
    CU_pSuite suite = NULL;

    suite = CU_add_suite("Suite_TestSceneManager", NULL, NULL);
    if (suite == NULL) {
        printf("Erreur lors de la création de la suite de tests.\n");
        return CU_get_error();
    }

    if ((CU_add_test(suite, "test createScene", test_createScene) == NULL) ||
        (CU_add_test(suite, "test sceneAddObject", test_sceneAddObject) == NULL) ||
        (CU_add_test(suite, "test sceneRemoveObject", test_sceneRemoveObject) == NULL) ||
        (CU_add_test(suite, "test sceneRegisterFunction", test_sceneRegisterFunction) == NULL) ||
        (CU_add_test(suite, "test setScene", test_setScene) == NULL)) {
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
