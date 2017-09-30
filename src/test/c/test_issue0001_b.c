#ifdef TEST_0001

#include "crashC.h"
#include "test_checker.h"

TESTSUITE(1) {
	clearTestChecker();
	TESTCASE("testcase1", "") {
		addCharacter('a');
		WHEN("test", "") {
			addCharacter('b');
			WHEN("inner when", "") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("inner when 2", "") {
				addCharacter('e');
			}
			addCharacter('f');
			THEN("inner then", "") {
				addCharacter('g');
			}
			addCharacter('h');
		}
		addCharacter('i');
		THEN("test", "") {
			addCharacter('j');
		}
		addCharacter('k');
		WHEN("test2", "") {
			addCharacter('l');
		}
		addCharacter('m');
		THEN("test2", "") {
			addCharacter('n');
		}
		addCharacter('o');
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdfghijkmno abdefghijkmno aijklmno ");
}

#endif
