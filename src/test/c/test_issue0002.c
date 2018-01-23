/*
 * test_issue0002.c
 *
 *  Created on: Oct 8, 2017
 *      Author: koldar
 */

#ifdef TEST_0002

#include "crashC.h"
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

	clearTestChecker();
	TESTCASE("1", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdefghlm abcdhijklm amnopqrst ");
}

// ************************ TESTS WITH RUN/NORUN *****************************

TESTSUITE(2) {

	clearTestChecker();
	TESTCASE("2", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "NORUN") { //no run!
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdeghlm abcdhijklm amnopqrst ");
}

TESTSUITE(3) {

	clearTestChecker();
	TESTCASE("3", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "NORUN") { //no run!
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdhijklm amnopqrst ");
}

TESTSUITE(4) {

	clearTestChecker();
	TESTCASE("4", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "NORUN") { //no run!
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abdefghlm abdhijklm amnopqrst ");
}

TESTSUITE(5) {

	clearTestChecker();
	TESTCASE("5", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "NORUN") { //no run!
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdefghlm abcdhijklm amnopqst ");
}

TESTSUITE(6) {

	clearTestChecker();
	TESTCASE("6", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "NORUN") { //NORUN!
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdefghlm abcdhijklm ");
}

TESTSUITE(7) {

	clearTestChecker();
	TESTCASE("7", "NORUN") { //NORUN!
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("");
}

// ************************ TESTS WITH RUN *****************************

TESTSUITE(102) {

	clearTestChecker();
	TESTCASE("102", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "") { //no run!
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdeghlm abcdhijklm amnopqrst ");
}

TESTSUITE(103) {

	clearTestChecker();
	TESTCASE("103", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "") { //no run!
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdhijklm amnopqrst ");
}

TESTSUITE(104) {

	clearTestChecker();
	TESTCASE("104", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "") { //no run!
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abdefghlm abdhijklm amnopqrst ");
}

TESTSUITE(105) {

	clearTestChecker();
	TESTCASE("105", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "") { //no run!
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdefghlm abcdhijklm amnopqst ");
}

TESTSUITE(106) {

	clearTestChecker();
	TESTCASE("106", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "") { //NORUN!
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdefghlm abcdhijklm ");
}

TESTSUITE(107) {

	clearTestChecker();
	TESTCASE("107", "") { //NORUN!
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("");
}

// ************************** CONFLICT BETWEEN EXCLUDE AND INCLUDE *****************************

TESTSUITE(201) {

	clearTestChecker();
	TESTCASE("201", "RUN") {
		addCharacter('a');
		WHEN("", "RUN") {
			addCharacter('b');
			THEN("", "RUN") {
				addCharacter('c');
			}
			addCharacter('d');
			WHEN("", "RUN NORUN") {
				addCharacter('e');
				THEN("", "RUN") {
					addCharacter('f');
				}
				addCharacter('g');
			}
			addCharacter('h');
			WHEN("", "RUN") {
				addCharacter('i');
				THEN("", "RUN") {
					addCharacter('j');
				}
				addCharacter('k');
			}
			addCharacter('l');
		}

		addCharacter('m');
		WHEN("", "RUN") {
			addCharacter('n');
			THEN("", "RUN") {
				addCharacter('o');
			}
			addCharacter('p');
			THEN("", "RUN") {
				addCharacter('q');
				WHEN("", "RUN") {
					addCharacter('r');
				}
				addCharacter('s');
			}
			addCharacter('t');
		}
		addCharacter(' ');
	}

	assertTestCheckerAndReset("abcdhijklm amnopqrst ");
}

#endif
