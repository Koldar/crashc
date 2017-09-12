#include "crashC.h"

TESTSUITE(1) {
    TESTCASE("testcase1", "") {
        WHEN("when 1", "") {
            printf("WHEN 1\n");

            int a = 6;
			int b = 0;

			printf("a/b is %d\n", a/b);

//            int i = 0;
//            if (i == 0) {
//                i++;
//                int a[19];
//                a[100043278] = 7;
//            }
//            else {
//                int a = 7 / 0;
//                printf("-- %d --\n", a);
//            }
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

    printf("FINISHED!\n");
}
