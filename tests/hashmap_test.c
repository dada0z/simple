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

void TestHashmapPut(CuTest* tc) {
    char key[20] = {0};
    char value[20] = {0};
    ssize_t error = 0;
    for (size_t i = 0; i < 100; i++) {
        snprintf(key, 20, "key%lu", i);
        snprintf(value, 20, "value%lu", i);
        error = hashmap_put(map, key, value);
        CuAssertIntEquals(tc, HASHMAP_OK, error);
    }

    CuAssertIntEquals(tc, 100, map->current_size);
}

void TestHashmapRemove(CuTest* tc) {
    char key[20] = {0};
    char value[20] = {0};
    ssize_t error = 0;
    snprintf(key, 20, "key%d", 20);
    snprintf(value, 20, "value%d", 20);
    error = hashmap_remove(map, key);
    CuAssertIntEquals(tc, HASHMAP_OK, error);
    CuAssertIntEquals(tc, 99, map->current_size);
    hashmap_free(map);
}
CuSuite* hashmapGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHashmapNew);
    SUITE_ADD_TEST(suite, TestHashmapPut);
    SUITE_ADD_TEST(suite, TestHashmapRemove);
    return suite;
}