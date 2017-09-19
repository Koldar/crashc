#include "crashC.h"

TESTSUITE(1) {
TESTCASE("testcase1", "") {
    WHEN("test", "") {
        printf("WHEN 1\n");
        WHEN("inner when", "") {
            printf("\tINNER WHEN\n");
        }
        WHEN("inner when 2", "") {
            printf("\tINNER WHEN 2\n");
        }
        THEN("inner then", "") {
            printf("\tINNER THEN\n");
        }
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
