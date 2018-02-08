/*
 * test_issue14.c
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#ifdef TEST_0014

#include "crashc.h"
#include "test_checker.h"
#include <assert.h>

TESTS_START
REG_SUITE(1);
REG_SUITE(2);
REG_SUITE(3);
REG_SUITE(4);
TESTS_END

TESTSUITE(1) {
	clear_test_checker();

	TESTCASE("testcase1", "") {
		add_char('a');
		WHEN("test", "") {
			add_char('b');
			WHEN("inner when", "") {add_char('c');}
			add_char('d');
			WHEN("inner when 2", "") {add_char('e');}
			add_char('f');
			THEN("inner then", "") {add_char('g');}
			add_char('h');
		}
		add_char('i');
		THEN("test", "") {add_char('j');}
		add_char('k');
		WHEN("test2", "") {add_char('l');}
		add_char('m');
		THEN("test2", "") {add_char('n');}
		add_char('o');
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdfghijkmno abdefghijkmno aijklmno ");
}

TESTSUITE(2) {
	clear_test_checker();

	TESTCASE("1", "") {
		add_char('a');
	}

	assert_and_reset_test_checker("a");
}

TESTSUITE(3) {
	clear_test_checker();

	TESTCASE("1", "") {
		add_char('a');
		WHEN("2", "") {
			add_char('b');
		}
		add_char('c');
	}

	assert_and_reset_test_checker("abc");
}

TESTSUITE(4) {
	clear_test_checker();

	TESTCASE("1", "") {
		add_char('a');
		THEN("2", "") {
			add_char('b');
		}
		add_char('c');
	}

	assert_and_reset_test_checker("abc");
}


#endif
