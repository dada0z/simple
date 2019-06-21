#include <stdio.h>
#include "CuTest.h"
CuSuite* hashmapGetSuite();
CuSuite* mimeGetSuite();

void RunAllTests(void) {
    CuString* output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, hashmapGetSuite());
    CuSuiteAddSuite(suite, mimeGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuSuiteDelete(suite);
}

int main(void) {
    RunAllTests();
    return 0;
}