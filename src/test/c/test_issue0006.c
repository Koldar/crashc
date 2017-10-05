#ifdef TEST_0006

#include "crashC.h"
#include "test_checker.h"

TESTS_START
REG_TEST(1);
TESTS_END

TESTSUITE(1) {
	clearTestChecker();
	int i=0;
    TESTCASE("testcase1", "") {
    	i += 1;
    	//drawSectionTree(&rootSection, "issue14_%04d", i);
    	addCharacter('a');
        WHEN("when 1", "") { //we enter here the first time. We then detect a signal and we mark that section are "signaled"
        	addCharacter('b');
            int a = 6;
			int b = 0;
			addCharacter(' ');
			printf("a/b is %d\n", a/b);
			addCharacter('c');
        }
        addCharacter('d');
        THEN("then1", "") {
        	addCharacter('e');
        }
        addCharacter('f');
        WHEN("when2", "") {
        	addCharacter('g');
        }
        addCharacter('h');
        THEN("then2", "") {
        	addCharacter('i');
        }
        addCharacter('j');
        addString("");
    }
    assertTestCheckerAndReset("ab adefghij");
}

#endif
