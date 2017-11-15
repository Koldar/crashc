#ifdef TEST_0015

#include "crashC.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(execution_flow);
TESTS_END

TESTSUITE(execution_flow) {

	clearTestChecker();
	
	TESTCASE("passed assertion simple", "") {
		addCharacter('a');
		WHEN("", "") {
			addCharacter('b');
			ASSERT(true);
			addCharacter('d');
		}
		WHEN("", "") {
			addCharacter('c');
			ASSERT(true);
			addCharacter('e');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abd ace ");
}

#endif