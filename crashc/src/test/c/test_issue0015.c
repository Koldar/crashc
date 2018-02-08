#ifdef TEST_0015

#include "crashc.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(1);
TESTS_END

TESTSUITE(1) {

	clear_test_checker();
	
	TESTCASE("passed assertion simple", "") {
		add_char('a');
		WHEN("", "") {
			add_char('b');
			ASSERT(true);
			add_char('d');
		}
		WHEN("", "") {
			add_char('c');
			ASSERT(true);
			add_char('e');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abd ace ");

	TESTCASE("failed assertion simple", "") {
		add_char('a');
		WHEN("W1", "") {
			add_char('b');
		}
		WHEN("W2", "") {
			add_char('c');
			ASSERT(false);
			add_char('x');
		}
		WHEN("W3", "") {
			add_char('d');
		}
		THEN("T", "") {
			add_char('e');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abe ac");

	TESTCASE("nested sections failed assertion", "") {
		add_char('a');
		WHEN("OW", "") {
			add_char('b');
			WHEN("IW1", "") {
				add_char('c');
			}
			WHEN("IW2", "") {
				add_char('d');
				ASSERT(false);
				add_char('g');
			}
			THEN("IT", "") {
				add_char('e');
			}
		}
		THEN("OT", "") {
			add_char('f');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcef abd");

	TESTCASE("nested sections passed assertion", "") {
		add_char('a');
		WHEN("OW", "") {
			add_char('b');
			WHEN("IW1", "") {
				add_char('c');
			}
			WHEN("IW2", "") {
				add_char('d');
				ASSERT(true);
				add_char('g');
			}
			THEN("IT", "") {
				add_char('e');
			}
		}
		THEN("OT", "") {
			add_char('f');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcef abdgef ");

	TESTCASE("only assertion passed", "") {
		add_char('a');
		ASSERT(true);
		add_char(' ');
	}

	assert_and_reset_test_checker("a ");

	TESTCASE("only assertion failed", "") {
		add_char('a');
		ASSERT(false);
		add_char(' ');
	}

	assert_and_reset_test_checker("a");
	
}

#endif