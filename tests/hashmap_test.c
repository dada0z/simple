#include "hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"
static hashmap_t map = NULL;
void TestHashmapNew(CuTest* tc) {
    map = hashmap_new();
    CuAssertPtrNotNull(tc, map);
    CuAssertIntEquals(tc, 256, map->max_size);
}

void TestHashmapFree(CuTest* tc) {
    hashmap_free(&map);
    CuAssertPtrEquals(tc, NULL, map);
}

void TestHashmapPut(CuTest* tc) {
    char key[20] = {0};
    char value[20] = {0};
    ssize_t error = 0;
    snprintf(key, 20, "key%d", 20);
    snprintf(value, 20, "value%d", 20);
    error = hashmap_put(map, key, value);
    CuAssertIntEquals(tc, HASHMAP_OK, error);
    CuAssertIntEquals(tc, 1, map->current_size);
}

void TestHashmapRemove(CuTest* tc) {
    char key[20] = {0};
    char value[20] = {0};
    ssize_t error = 0;
    snprintf(key, 20, "key%d", 20);
    snprintf(value, 20, "value%d", 20);
    error = hashmap_remove(map, key);
    CuAssertIntEquals(tc, HASHMAP_OK, error);
    CuAssertIntEquals(tc, 0, map->current_size);
}
CuSuite* hashmapGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHashmapNew);
    SUITE_ADD_TEST(suite, TestHashmapPut);
    SUITE_ADD_TEST(suite, TestHashmapRemove);
    SUITE_ADD_TEST(suite, TestHashmapFree);
    return suite;
}