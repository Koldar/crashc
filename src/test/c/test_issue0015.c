#ifdef TEST_0015

#include "crashc.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(1);
TESTS_END

TESTSUITE(1) {

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

	TESTCASE("failed assertion simple", "") {
		addCharacter('a');
		WHEN("W1", "") {
			addCharacter('b');
		}
		WHEN("W2", "") {
			addCharacter('c');
			ASSERT(false);
			addCharacter('x');
		}
		WHEN("W3", "") {
			addCharacter('d');
		}
		THEN("T", "") {
			addCharacter('e');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abe ac");

	TESTCASE("nested sections failed assertion", "") {
		addCharacter('a');
		WHEN("OW", "") {
			addCharacter('b');
			WHEN("IW1", "") {
				addCharacter('c');
			}
			WHEN("IW2", "") {
				addCharacter('d');
				ASSERT(false);
				addCharacter('g');
			}
			THEN("IT", "") {
				addCharacter('e');
			}
		}
		THEN("OT", "") {
			addCharacter('f');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcef abd");

	TESTCASE("nested sections passed assertion", "") {
		addCharacter('a');
		WHEN("OW", "") {
			addCharacter('b');
			WHEN("IW1", "") {
				addCharacter('c');
			}
			WHEN("IW2", "") {
				addCharacter('d');
				ASSERT(true);
				addCharacter('g');
			}
			THEN("IT", "") {
				addCharacter('e');
			}
		}
		THEN("OT", "") {
			addCharacter('f');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcef abdgef ");

	TESTCASE("only assertion passed", "") {
		addCharacter('a');
		ASSERT(true);
		addCharacter(' ');
	}

	assertTestCheckerAndReset("a ");

	TESTCASE("only assertion failed", "") {
		addCharacter('a');
		ASSERT(false);
		addCharacter(' ');
	}

	assertTestCheckerAndReset("a");
	
}

#endif