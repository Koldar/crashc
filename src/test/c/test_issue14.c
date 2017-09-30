/*
 * test_issue14.c
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#ifdef TEST_00014_000

#include "crashC.h"
#include "test_checker.h"
#include <assert.h>

TESTS_START
REG_TEST(1);
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

#endif
