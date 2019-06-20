#include "hashmap.h"
#include <stddef.h>
#include "CuTest.h"

void TestHashmapNew(CuTest* tc) {
    hashmap_t p = hashmap_new();
    CuAssertPtrNotNull(tc, p);
    hashmap_free(&p);
    CuAssertPtrEquals(tc, NULL, p);
}
CuSuite* hashmapGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHashmapNew);
    return suite;
}