#include "hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "CuTest.h"
static hashmap_t map = NULL;
void TestHashmapPut(CuTest* tc) {
    map = hashmap_new();
    char key[20] = {0};
    char value[20] = {0};
    ssize_t error = 0;
    for (size_t i = 0; i < 100; i++) {
        snprintf(key, 20, "key%lu", i);
        snprintf(value, 20, "value%lu", i);
        error = hashmap_put(map, key, value);
        CuAssertIntEquals(tc, HASHMAP_OK, error);
    }

    CuAssertIntEquals(tc, 100, hashmap_get_size(map));
}

void TestHashmapGet(CuTest* tc) {
    char key[20] = {0};
    char* value = NULL;
    ssize_t error = 0;
    snprintf(key, 20, "key%d", 20);
    error = hashmap_get(map, key, &value);
    CuAssertIntEquals(tc, HASHMAP_OK, error);
    CuAssertStrEquals(tc, "value20", value);
}

ssize_t iterateCallback(const char* key, const char* value) {
    printf("key:%s,value:%s\n", key, value);
    return HASHMAP_OK;
}
void TestHashmapIterate(CuTest* tc) {
    ssize_t error = 0;
    error = hashmap_iterate(map, iterateCallback);
    CuAssertIntEquals(tc, HASHMAP_OK, error);
}

void TestHashmapRemove(CuTest* tc) {
    char key[20] = {0};
    char value[20] = {0};
    ssize_t error = 0;
    snprintf(key, 20, "key%d", 20);
    snprintf(value, 20, "value%d", 20);
    error = hashmap_remove(map, key);
    CuAssertIntEquals(tc, HASHMAP_OK, error);
    CuAssertIntEquals(tc, 99, hashmap_get_size(map));
    hashmap_free(map);
}
CuSuite* hashmapGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHashmapPut);
    SUITE_ADD_TEST(suite, TestHashmapGet);
    SUITE_ADD_TEST(suite, TestHashmapIterate);
    SUITE_ADD_TEST(suite, TestHashmapRemove);
    return suite;
}