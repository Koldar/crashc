/*
 * ct_assert.h
 *
 *  Created on: Nov 15, 2017
 *      Author: noodles
 */

#ifndef CT_ASSERT_H_
#define CT_ASSERT_H_

#include <stdbool.h>
#include <stdlib.h>

#include "model.h"
#include "main_model.h"

/**
 * This macro represents the integer code passed to the siglongjmp code when jumping back to the start of
 * a testcase due to a failed assertion. This lets us distinguish when we do this due to a signal or due to
 * an assertion ad thus let us handle the two cases differently
 */
#define ASSERT_JUMP_CODE 2

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
	char* asserted;

	/**
	 * Pointer to the value that the assertion is expected to assume.
	 * This is a void* in order to be able to handle any type of data, even custom structures
	 */
	void* expected_value;

	/**
	 * A string containing the textual representation of the expected_value.
	 */
	char* expected_str;

	/**
	 * Pointer to the value that the assertion actually assumed.
	 */
	void* actual_value;

	/**
	 * String representation of the actual value assumed by the assertion
	 */
	char* actual_str;

	/**
	 * Pointer to a string containing the file name in which the assertion was
	 */
	char* file_name;

	/**
	 * The line number on which the assertion was located in the source code
	 */
	unsigned int line_number;

} ct_assert_report_t;

/**
 * This type represents a comparator function, used by sophisticated versions of assertions to compare arbitraty types, like user defined structures
 * A function used in this sense is expected by the framework to return:
 * -1 when elem1 is "lesser" than elem2
 *  0 when elem1 is equals to elem2
 * +1 when elem1 is "greater" to elem2
 *
 * UNUSED AT THE MOMENT
 */
//typedef int (*ct_comparator_t)(void* elem1, void* elem2);

/**
 * This type represents a stringer function, used by sophisticated versions of assertions to create strings representing the type of data on which the assertion
 * is being made, for example user-defined structures.
 * Note that the strings returned by the function are expected to be malloc'd, as he test report destructor will try to free them
 * It is possible that in future versions of the framework, the destructor will be parametrized
 *
 * UNUSED AT THE MOMENT
 */
//typedef void (*ct_stringer_t)(void* elem);

/**
 * This type represents the function pointers used as callbacks to handle the behaviour of an assertion implementation.
 * The 3 parameters are:
 * - model: the CrashC model being used
 * - type_size: the size, in bytes, of the type on which the assertion is being made. This is needed as once the original pointer is cast to void *, its real type cannot be
 * 				recovered anymore, so we need this to do some boundaries check
 * - asserted: pointer to the asserted variable/value
 * - stringer: the stringer to be used to convert raw bytes into a string
 */
typedef void (*ct_assert_callback_t)(crashc_model* model);


/**
 * The general CrashC assertion macro.
 * This macro is not used directly, but it is masked by other macros which actually implement
 * a specific assertion type.
 */
#define CT_ASSERTION(model, is_mandatory, asserted, passed_callback, failed_callback)														\
	addTailInList((model)->currentSnapshot->assertion_reports, ct_init_assert_report(is_mandatory, #asserted, __FILE__, __LINE__));			\
	if ((asserted) != true) {																												\
		failed_callback((model));																											\
	}																																		\
	else {																																	\
		passed_callback((model));																											\
	}																																		\

/**
 * This macro is the basic form of a CrashC assertion.
 * It takes as an argument a boolean expression and fails if it evalues to false.
 */
#define ASSERT(assertion) CT_ASSERTION(&cc_model, true, assertion, ct_assert_do_nothing, ct_general_assert_failed)

/**
 * Initializes an assertion report and returns a pointer to it.
 * Note that expected_value, actual_value, expected_stringed and actual_stringed all default to NULL: their initialization is demanded to the caller in a later moment
 */
ct_assert_report_t* ct_init_assert_report(bool is_mandatory, char* asserted_text, char* file, unsigned int line);

/**
 * Frees the memory allocated for a particular assertion report
 */
void ct_destroy_assert_report(ct_assert_report_t * report);

/**
 * Function used as a callback when an assertion doesn't need to do anything
 */
void ct_assert_do_nothing(crashc_model * model);

/**
 * Function used by the general ASSERT macro to handle its failure
 */
void ct_general_assert_failed(crashc_model * model);

#endif /* CT_ASSERT_H_ */
