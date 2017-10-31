/*
 * test_issue14.c
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#ifdef TEST_0014

#include "crashC.h"
#include "test_checker.h"
#include <assert.h>

TESTS_START
REG_SUITE(1);
REG_SUITE(2);
REG_SUITE(3);
REG_SUITE(4);
TESTS_END

TESTSUITE(1) {
	clearTestChecker();

	TESTCASE("testcase1", "") {
		addCharacter('a');
		WHEN("test", "") {
			addCharacter('b');
			WHEN("inner when", "") {addCharacter('c');}
			addCharacter('d');
			WHEN("inner when 2", "") {addCharacter('e');}
			addCharacter('f');
			THEN("inner then", "") {addCharacter('g');}
			addCharacter('h');
		}
		addCharacter('i');
		THEN("test", "") {addCharacter('j');}
		addCharacter('k');
		WHEN("test2", "") {addCharacter('l');}
		addCharacter('m');
		THEN("test2", "") {addCharacter('n');}
		addCharacter('o');
		addString("");
	}

	assertTestCheckerAndReset("abcdfghijkmno abdefghijkmno aijklmno ");
}

TESTSUITE(2) {
	clearTestChecker();

	TESTCASE("1", "") {
		addCharacter('a');
	}

	assertTestCheckerAndReset("a");
}

TESTSUITE(3) {
	clearTestChecker();

	TESTCASE("1", "") {
		addCharacter('a');
		WHEN("2", "") {
			addCharacter('b');
		}
		addCharacter('c');
	}

	assertTestCheckerAndReset("abc");
}

TESTSUITE(4) {
	clearTestChecker();

	TESTCASE("1", "") {
		addCharacter('a');
		THEN("2", "") {
			addCharacter('b');
		}
		addCharacter('c');
	}

	assertTestCheckerAndReset("abc");
}


#endif
