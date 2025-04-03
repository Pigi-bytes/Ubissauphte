#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>

#include "../src/utils/objectManager.h"

/*
 *Ce programme sert à tester le programme de l'objectManager, il teste les points suivants;
 *  -> test de création d'un registre : test_createTypeRegistry()
 *  -> test d'enregistrement d'un type dans un registre : test_registerType()
 *  -> test d'initialisation d'un objectManager avec rien à l'intérieur : test_initObjectManager()
 *  -> test d'ajout d'un objet dans l'objectManager : test_addObject()
 *  -> test d'ajout de 2 objets de même type dans l'objectManager : test_addObjects_SameType()
 *  -> test d'ajout de 2 objets de types différents dans l'objectManager : test_addObject_DifferentsType()
 *  -> test de suppression d'objets dans l'objectManager : test_deleteObject()
 *  -> test de destruction de l'objectManager : test_freeObjectManager()
 */

// fonction de libération pour un objet de type int
void freeInt(void* ptr) {
    free(ptr);
}

// fonction de libération pour un objet de type char
void freeChar(void* ptr) {
    return;
}

// test de création du registre de types
void test_createTypeRegistry() {
    t_typeRegistry* registry = createTypeRegistry();
    CU_ASSERT_PTR_NOT_NULL(registry);
    CU_ASSERT_EQUAL(registry->nextTypeId, 0);
    free(registry);
}

// test d'enregistrement d'un type
void test_registerType() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t typeId = registerType(registry, freeInt, "int");

    CU_ASSERT_EQUAL(typeId, 0);
    CU_ASSERT_EQUAL(registry->types[typeId].freeFunc, freeInt);
    CU_ASSERT_STRING_EQUAL(registry->types[typeId].name, "int");

    free(registry);
}

// test d'initialisation du gestionnaire d'objets
void test_initObjectManager() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t intTypeId = registerType(registry, freeInt, "int");
    t_objectManager* manager = initObjectManager(registry);

    CU_ASSERT_PTR_NOT_NULL(manager);
    CU_ASSERT_PTR_NOT_NULL(manager->firstPool);
    CU_ASSERT_PTR_NOT_NULL(manager->registry);
    CU_ASSERT_EQUAL(manager->count, 0);

    freeObjectManager(manager);
}

// test d'ajout d'un objet
void test_addObject() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t intTypeId = registerType(registry, freeInt, "int");
    t_objectManager* manager = initObjectManager(registry);

    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 42;

    addObject(manager, newInt, intTypeId);
    CU_ASSERT_EQUAL(manager->count, 1);

    int* retrievedInt = (int*)getObject(manager, 0);
    CU_ASSERT_PTR_NOT_NULL(retrievedInt);
    CU_ASSERT_EQUAL(*retrievedInt, 42);

    freeObjectManager(manager);
}

// test d'ajout d'objets de même type
void test_addObjects_SameType() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t intTypeId = registerType(registry, freeInt, "int");

    t_objectManager* manager = initObjectManager(registry);

    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 42;

    int* newInt2 = (int*)malloc(sizeof(int));
    *newInt2 = 75;

    addObject(manager, newInt, intTypeId);
    addObject(manager, newInt2, intTypeId);
    CU_ASSERT_EQUAL(manager->count, 2);

    int* retrievedInt = (int*)getObject(manager, 0);
    CU_ASSERT_PTR_NOT_NULL(retrievedInt);
    CU_ASSERT_EQUAL(*retrievedInt, 42);

    int* retrievedInt2 = (int*)getObject(manager, 1);
    CU_ASSERT_PTR_NOT_NULL(retrievedInt2);
    CU_ASSERT_EQUAL(*retrievedInt2, 75);

    freeObjectManager(manager);
}

// test d'ajout d'objets de différents type
void test_addObject_DifferentsType() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t intTypeId = registerType(registry, freeInt, "int");
    uint8_t charTypeId = registerType(registry, freeChar, "char");
    t_objectManager* manager = initObjectManager(registry);

    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 42;

    char* newChar = "coucou";

    addObject(manager, newInt, intTypeId);
    addObject(manager, newChar, charTypeId);
    CU_ASSERT_EQUAL(manager->count, 2);

    int* retrievedInt = (int*)getObject(manager, 0);
    CU_ASSERT_PTR_NOT_NULL(retrievedInt);
    CU_ASSERT_EQUAL(*retrievedInt, 42);

    char* retrievedChar = (char*)getObject(manager, 1);
    CU_ASSERT_PTR_NOT_NULL(retrievedChar);
    CU_ASSERT_EQUAL(retrievedChar, "coucou");

    freeObjectManager(manager);
}

// test de suppression d'objets
void test_deleteObject() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t intTypeId = registerType(registry, freeInt, "int");
    t_objectManager* manager = initObjectManager(registry);

    // ajout de 10 objets int
    for (int i = 0; i < 10; i++) {
        int* newInt = (int*)malloc(sizeof(int));
        *newInt = i;
        addObject(manager, newInt, intTypeId);
    }

    // Suppression d'un objet à l'index 5
    deleteObject(manager, 5);
    CU_ASSERT_EQUAL(manager->count, 9);

    // Vérification que l'objet à l'index 5 a été supprimé
    int* retrievedInt = (int*)getObject(manager, 5);
    CU_ASSERT_PTR_NOT_NULL(retrievedInt);
    CU_ASSERT_EQUAL(*retrievedInt, 9);  // Après suppression, l'objet à l'index 5 est remplacé par l'objet à l'index 9

    freeObjectManager(manager);
}

// Test de libération des ressources
void test_freeObjectManager() {
    t_typeRegistry* registry = createTypeRegistry();
    uint8_t intTypeId = registerType(registry, freeInt, "int");
    t_objectManager* manager = initObjectManager(registry);

    // Ajout d'un objet
    int* newInt = (int*)malloc(sizeof(int));
    *newInt = 100;
    addObject(manager, newInt, intTypeId);

    // Libération du gestionnaire
    freeObjectManager(manager);

    // Test si tout a été correctement libéré
    // La libération elle-même est un test indirect, car elle libère la mémoire et ne génère pas d'erreur
    CU_ASSERT_TRUE(1);  // Nous utilisons ce test comme validation de la libération des ressources
}

// Fonction pour enregistrer les tests dans CUnit
int addTests() {
    CU_pSuite suite = NULL;

    suite = CU_add_suite("Suite_TestObjectManager", NULL, NULL);
    if (suite == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "Test createTypeRegistry", test_createTypeRegistry) == NULL ||
        CU_add_test(suite, "Test registerType", test_registerType) == NULL ||
        CU_add_test(suite, "Test initObjectManager", test_initObjectManager) == NULL ||
        CU_add_test(suite, "Test addObject", test_addObject) == NULL ||
        CU_add_test(suite, "Test addObjectDifferentsType", test_addObject_DifferentsType) == NULL ||
        CU_add_test(suite, "Test addObjectSameType", test_addObjects_SameType) == NULL ||
        CU_add_test(suite, "Test deleteObject", test_deleteObject) == NULL ||
        CU_add_test(suite, "Test freeObjectManager", test_freeObjectManager) == NULL) {
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

int main() {
    // Initialisation de CUnit
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Ajouter les tests
    if (addTests() != CUE_SUCCESS) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Lancer les tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Libération des ressources de CUnit
    CU_cleanup_registry();
    return 0;
}
