#include "crashC.h"

TESTSUITE(1) {
    TESTCASE("testcase1", "") {
        int array[10];
        WHEN("test", "") {
            array[100000] = 7;
            printf("WHEN 1\n");
        }
        THEN("test", "") {
            printf("THEN 1\n");
        }
        WHEN("test2", "") {
            printf("WHEN 2\n");
        }
        THEN("test2", "") {
            printf("THEN 2\n");
        }
    }
}
