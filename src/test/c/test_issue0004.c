#include "crashC.h"
#include "test_checker.h"

/**
 * This in an example on how to use the testing report producer expected output.
 * The testing producer will output section reports in the format:
 * <section_level> | <section_description> | <section_status>
 * followed by a trailing underscore to separate the different sections.
 * Different tests are separated by a single whitespace and are preceded by a their outcome and
 * a hypen, e.g OK-
 * Note that the trailing underscore is needed also after the last section of a test, due to 
 * code simplicity
 *
 * This clearly isn't the best implementation ever, but it works. We will eventually update it in the future
 *
 */
void check_result() {
	//Here we check that the reports are what we expect
	assertTestCheckerAndReset(											   	\
		"OK-1|TC1|OK_2|OW|OK_3|IW1|OK_3|IT1|OK_2|OT|OK_3|IW2|OK_3|IT2|OK_ " \
		"OK-1|TC2|OK_2|W1|OK_2|T|OK_ "										\
		"NO-1|TC2|SIG_2|W2|SIG_ "
	);
}

TESTS_START

REG_SUITE(1);
AFTER_TESTS(check_result);

TESTS_END

TESTSUITE(1) {

	//Here we set up the special report producer used for testing
	ct_setup_testing_producer(&cc_model);

	//Now we have the usual test code

	//Nested sections
	TESTCASE("TC1", "") {
		WHEN("OW", "") {
			WHEN("IW1", "") {}
			THEN("IT1", "") {}
		}
		THEN("OT", "") {
			WHEN("IW2", "") {}
			THEN("IT2", "") {}
		}
	}

	//Signaled testcase
	TESTCASE("TC2", "") {
		WHEN("W1", "") {}
		WHEN("W2", "") {int a = 7 / 0;}
		WHEN("W3", "") {}
		THEN("T", "") {}
	}	
	
}