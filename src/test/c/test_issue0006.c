#ifdef TEST_0006

#include "crashC.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(1);
REG_SUITE(2);
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
        addCharacter('\0');
    }
    assertTestCheckerAndReset("ab ");
}

TESTSUITE(2) {
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
			printf("a/b is %d\n", a/b);
			addCharacter('c');
        }
    }

    addCharacter(' ');

    TESTCASE("testcase2", "") {
    	addCharacter('a');
		WHEN("when 1", "") {
			addCharacter('b');
		}
		addCharacter('c');
		WHEN("when 2", "") { //we enter here the first time. We then detect a signal and we mark that section are "signaled"
			addCharacter('d');
		}
		addCharacter('e');
    }
    assertTestCheckerAndReset("ab abceacde");
}

#endif
