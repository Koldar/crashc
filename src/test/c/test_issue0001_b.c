#ifdef TEST_0001

#include "crashc.h"
#include "test_checker.h"

TESTSUITE(1) {
	clear_test_checker();
	TESTCASE("testcase1", "") {
		add_char('a');
		WHEN("test", "") {
			add_char('b');
			WHEN("inner when", "") {
				add_char('c');
			}
			add_char('d');
			WHEN("inner when 2", "") {
				add_char('e');
			}
			add_char('f');
			THEN("inner then", "") {
				add_char('g');
			}
			add_char('h');
		}
		add_char('i');
		THEN("test", "") {
			add_char('j');
		}
		add_char('k');
		WHEN("test2", "") {
			add_char('l');
		}
		add_char('m');
		THEN("test2", "") {
			add_char('n');
		}
		add_char('o');
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdfghijkmno abdefghijkmno aijklmno ");
}

#endif
