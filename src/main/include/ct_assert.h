/*
 * ct_assert.h
 *
 *  Created on: Nov 15, 2017
 *      Author: noodles
 */

#ifndef CT_ASSERT_H_
#define CT_ASSERT_H_

#include <stdbool.h>

#include "model.h"
#include "main_model.h"

/**
 * This struct contains the informations related to a specific assertion
 */
typedef struct {

	/**
	 * Represents the outcome of the assertion. True if succeded
	 */
	bool passed;

	/**
	 * Tells whenever this assertion is mandatory or not.
	 * Mandatory assertions will cause a test abortion on failure, non-mandatory assertions
	 * will simply register the failure and continue the test
	 */
	bool is_mandatory;

	/**
	 * A string containing the text of the assertion, useful for reporting
	 */
	char * asserted;

	/**
	 * A string containing the expected result of the assertion. Note that this is stored as a string
	 * in order not to limit the information to a particular kind of assertion
	 */
	char * expected;

	/**
	 * A string containing the actual result of the assertion. As for the "expected" field, this is a pointer
	 * to a string in order not to lose generality
	 */
	char * actual;

	/**
	 * Pointer to a string containing the file name in which the assertion was
	 */
	char * file_name;

	/**
	 * Pointer to a string containing the line number on which the assertion was
	 */
	char * line_number;

} ct_assert_report_t;

/**
 * This type represents the function pointers used as callbacks to handle an assertion positive result
 */
typedef void (*ct_assert_ok_callback_t)(crashc_model * model, char * assert_text, char * expected, char * file_name, char * line_number);

/**
 * This type represents the function pointers used as callbacks to handle the failure of an assertion
 */
typedef void (*ct_assert_fail_callback_t)(crashc_model * model, char * assert_text, char * expected, char * file_name, char * line_number);

/**
 * The general CrashC assertion macro.
 * This macro is not used directly, but it is masked by other macros which actually implement
 * a specific assertion type.
 */
#define CT_ASSERTION(model, assertion, expected, passed_callback, failed_callback)				\
	if ((assertion) != (expected)) {															\
		failed_callback((model), #assertion, #expected, __FILE__, __LINE__);					\
	}																							\
	else {																						\
		passed_callback((model), #assertion, #expected, __FILE__, __LINE__);					\
	}																							\

/**
 * This macro is the basic form of a CrashC assertion.
 * It takes as an argument a boolean expression and fails if it evalues to false.
 */
#define ASSERT(assertion) CT_ASSERTION(&cc_model, assertion, true, ct_assert_passed, ct_assert_failed)

/**
 * Initializes an assertion report and returns a pointer to it.
 */
ct_assert_report_t *  ct_generate_assert_report(bool passed, bool is_mandatory, char * text, char * expected, char * actual, char * file, char * line);

/**
 * Frees the memory allocated for a particular assertion report
 */
void ct_destroy_assert_report(ct_assert_report_t * report);

void ct_assert_passed(crashc_model * model, char * asserted, char * expected, char * file, char * line);
void ct_assert_failed(crashc_model * model, char * asserted, char * expected, char * file, char * line);

#endif /* CT_ASSERT_H_ */
