#ifdef TEST_0006_000

#include "crashC.h"
#include "test_checker.h"

TESTS_START
REG_TEST(1);
TESTS_END

TESTSUITE(1) {
	clearTestChecker();
    TESTCASE("testcase1", "") {
    	addCharacter('a');
        WHEN("when 1", "") { //we enter here the first time. We then detect a signal and we mark that section are "signaled"
        	addCharacter('b');
            int a = 6;
			int b = 0;
			printf("a/b is %d\n", a/b);
			addCharacter('c');
        }
        addCharacter('d');
        THEN("test", "") {
        	addCharacter('e');
        }
        addCharacter('f');
        WHEN("test2", "") {
        	addCharacter('g');
        }
        addCharacter('h');
        THEN("test2", "") {
        	addCharacter('i');
        }
        addCharacter('j');
        addString("");
    }
    assertTestCheckerAndReset("abadefghij");
}

#endif
