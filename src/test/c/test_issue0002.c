/*
 * test_issue0002.c
 *
 *  Created on: Oct 8, 2017
 *      Author: koldar
 */

#ifdef TEST_0002

#include "crashc.h"
#include "test_checker.h"
#include "tag.h"
#include <stdio.h>

TESTS_START

//add the tags we want to use
ct_tag_ht_put(ct_model->run_only_if_tags, "RUN");
ct_tag_ht_put(ct_model->exclude_tags, "NORUN");

REG_SUITE(1);
REG_SUITE(2);
REG_SUITE(3);
REG_SUITE(4);
REG_SUITE(5);
REG_SUITE(6);
REG_SUITE(7);

REG_SUITE(102);
REG_SUITE(103);
REG_SUITE(104);
REG_SUITE(105);
REG_SUITE(106);
REG_SUITE(107);

REG_SUITE(201);

TESTS_END

// ************************ STANDARD TEST ************************************

TESTSUITE(1) {

	clear_test_checker();
	TESTCASE("1", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdefghlm abcdhijklm amnopqrst ");
}

// ************************ TESTS WITH RUN/NORUN *****************************

TESTSUITE(2) {

	clear_test_checker();
	TESTCASE("2", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "NORUN") { //no run!
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdeghlm abcdhijklm amnopqrst ");
}

TESTSUITE(3) {

	clear_test_checker();
	TESTCASE("3", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "NORUN") { //no run!
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdhijklm amnopqrst ");
}

TESTSUITE(4) {

	clear_test_checker();
	TESTCASE("4", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "NORUN") { //no run!
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abdefghlm abdhijklm amnopqrst ");
}

TESTSUITE(5) {

	clear_test_checker();
	TESTCASE("5", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "NORUN") { //no run!
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdefghlm abcdhijklm amnopqst ");
}

TESTSUITE(6) {

	clear_test_checker();
	TESTCASE("6", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "NORUN") { //NORUN!
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdefghlm abcdhijklm ");
}

TESTSUITE(7) {

	clear_test_checker();
	TESTCASE("7", "NORUN") { //NORUN!
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("");
}

// ************************ TESTS WITH RUN *****************************

TESTSUITE(102) {

	clear_test_checker();
	TESTCASE("102", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "") { //no run!
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdeghlm abcdhijklm amnopqrst ");
}

TESTSUITE(103) {

	clear_test_checker();
	TESTCASE("103", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "") { //no run!
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdhijklm amnopqrst ");
}

TESTSUITE(104) {

	clear_test_checker();
	TESTCASE("104", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "") { //no run!
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abdefghlm abdhijklm amnopqrst ");
}

TESTSUITE(105) {

	clear_test_checker();
	TESTCASE("105", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "") { //no run!
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdefghlm abcdhijklm amnopqst ");
}

TESTSUITE(106) {

	clear_test_checker();
	TESTCASE("106", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "") { //NORUN!
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdefghlm abcdhijklm ");
}

TESTSUITE(107) {

	clear_test_checker();
	TESTCASE("107", "") { //NORUN!
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("");
}

// ************************** CONFLICT BETWEEN EXCLUDE AND INCLUDE *****************************

TESTSUITE(201) {

	clear_test_checker();
	TESTCASE("201", "RUN") {
		add_char('a');
		WHEN("", "RUN") {
			add_char('b');
			THEN("", "RUN") {
				add_char('c');
			}
			add_char('d');
			WHEN("", "RUN NORUN") {
				add_char('e');
				THEN("", "RUN") {
					add_char('f');
				}
				add_char('g');
			}
			add_char('h');
			WHEN("", "RUN") {
				add_char('i');
				THEN("", "RUN") {
					add_char('j');
				}
				add_char('k');
			}
			add_char('l');
		}

		add_char('m');
		WHEN("", "RUN") {
			add_char('n');
			THEN("", "RUN") {
				add_char('o');
			}
			add_char('p');
			THEN("", "RUN") {
				add_char('q');
				WHEN("", "RUN") {
					add_char('r');
				}
				add_char('s');
			}
			add_char('t');
		}
		add_char(' ');
	}

	assert_and_reset_test_checker("abcdhijklm amnopqrst ");
}

#endif
