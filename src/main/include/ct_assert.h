/**
 * @file
 *
 * The module where @crashc assertions API are defined
 *
 * Asserts are the base foundation of every unit testing
 *
 * @author noodles
 * @date Nov 15, 2017
 */

#ifndef CT_ASSERT_H_
#define CT_ASSERT_H_

#include <stdbool.h>
#include <stdlib.h>

#include "model.h"
#include "main_model.h"

//TODO create an enum for these codes. It's better to centralized similar magic numbers into enums imho
/**
 * Magic number to pass to \c siglongjmp code when jumping back to the start of a testcase due to a failed assertion
 *
 * This macro represents the integer code passed to the siglongjmp code when jumping back to the start of
 * a testcase due to a failed assertion. This lets us distinguish when we do this due to a signal or due to
 * an assertion ad thus let us handle the two cases differently.
 */
#define ASSERT_JUMP_CODE 2

/**
 * This struct contains the informations related to a specific assertion
 */
typedef struct {

	/**
	 *  The outcome of the assertion. True if succeded
	 */
	bool passed;

	/**
	 * Whenever this assertion is mandatory or not
	 *
	 * Mandatory assertions will cause a test abortion on failure, non-mandatory assertions
	 * will simply register the failure and continue the test
	 */
	bool is_mandatory;

	/**
	 * String containing the text of the assertion, useful for reporting
	 */
	char* asserted;

	/**
	 * Pointer to the value expected to have from the assertion.
	 *
	 * This is a void* in order to be able to handle any type of data, even custom ones
	 */
	void* expected_value;

	/**
	 * A string containing the textual representation of the expected_value.
	 */
	char* expected_str;

	/**
	 * Pointer to the value obtained from the function to test.
	 *
	 * This is a void* in order to be able to handle any type of data, even custom ones
	 */
	void* actual_value;

	/**
	 * String representation of the actual_value assumed by the assertion
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
 * function prototype for function comparing elements
 *
 * This type represents a comparator function, used by sophisticated versions of assertions to compare arbitraty types, like user defined structures
 *
 * @param[in] elem1 the first element to compare
 * @param[in] elem2 the second element to compare
 * @return
 * 	\li -1 when \c elem1 is "lesser" than \c elem2
 *  \li 0 when \c elem1 is equal to \c elem2
 *  \li +1 when \c elem1 is "greater" to \c elem2
 *
 * UNUSED AT THE MOMENT
 */
//typedef int (*ct_comparator_t)(void* elem1, void* elem2);

/**
 * function prototype for functions computing string representation of elements
 *
 * This type represents a stringer function, used by sophisticated versions of assertions to create strings representing the type of data on which the assertion
 * is being made, for example user-defined structures.
 * Note that the strings returned by the function are expected to be malloc'd, as he test report destructor will try to free them
 * It is possible that in future versions of the framework, the destructor will be parametrized
 *
 * @param[in] elem the elements which we want to obtain a string representation of
 *
 * UNUSED AT THE MOMENT
 */
//TODO I think the best prototype for this function pointer would be int(void* element, int buffer size, char* buffer) where the return value is the number of byte you used in the buffer.
//typedef void (*ct_stringer_t)(void* elem);

/**
 * Represents the function pointers used as callbacks to handle the behaviour of an assertion implementation.
 * The 3 parameters are:
 * @param[in] model the @crashc model being used
 */
/*
 * - type_size: the size, in bytes, of the type on which the assertion is being made. This is needed as once the original pointer is cast to void *, its real type cannot be
 * 				recovered anymore, so we need this to do some boundaries check
 * - asserted: pointer to the asserted variable/value
 * - stringer: the stringer to be used to convert raw bytes into a string
 */
typedef void (*ct_assert_callback_t)(ct_model_t* model);


/**
 * The general CrashC assertion macro
 *
 * This macro is not used directly, but it is masked by other macros which actually implement
 * a specific assertion type.
 *
 * @param[in] model a pointer to ct_crashc_t model used
 * @param[in] is_mandatory @true if the assertion needs to be surpassed; @false if the assertion is actually optional
 * @param[in] asserted C code representing the whole content of the assertion. This is likely to be something like <tt>someStuff == someOtherStuff</tt>.
 * 	The code is **required** to be a boolean expression.
 * @param[in] passed_callback function of type ct_assert_callback_t called if the assertion passes;
 * @param[in] failed_callback function of type ct_assert_callback_t called if the assertion doesn't pass;
 */
#define CT_ASSERTION(model, is_mandatory, asserted, passed_callback, failed_callback)														\
	ct_add_tail_in_list((model)->current_snapshot->assertion_reports, ct_init_assert_report(is_mandatory, #asserted, __FILE__, __LINE__));		\
	if ((asserted) != true) {																												\
		failed_callback((model));																											\
	}																																		\
	else {																																	\
		passed_callback((model));																											\
	}																																		\

//TODO complete the documentation here
/**
 * This macro is the basic form of a CrashC assertion.
 *
 * @param[in] assertion a C code reprsenting the assertion to express. The code is **required** to be a boolean expression.
 */
#define ASSERT(assertion) CT_ASSERTION(ct_model, true, assertion, ct_assert_do_nothing, ct_general_assert_failed)

/**
 * Initializes an assertion report and returns a pointer to it.
 *
 * \note
 * \c expected_value, \c actual_value, \c expected_stringed and \c actual_stringed all default to @null: their initialization is demanded to the caller in a later moment
 *
 * @param[in] is_mandatory @true if the assertion needs to be surpassed; @false if the assertion is actually optional
 * @param[in] asserted_text a string representing the actual C code of assertion
 * @param[in] file a string representing the file where the assertion is positioned
 * @param[in] line the line number where the assertion is located in the file \c file
 * @return a structure representing the performance of a given assertion
 */
ct_assert_report_t* ct_init_assert_report(bool is_mandatory, char* asserted_text, char* file, unsigned int line);

/**
 * Frees the memory allocated for a particular assertion report
 *
 * @param[in] report the report to release from the memory
 */
void ct_destroy_assert_report(ct_assert_report_t* report);

/**
 * @defgroup assertCallbacks Assert Outcome Callbacks
 * @brief represents a group of callbacks designed to be passed to ::CT_ASSERTION macro
 * @{
 */

/**
 * Function used as a callback when an assertion doesn't need to do anything
 *
 * @param[in] model the model to handle
 */
void ct_assert_do_nothing(ct_model_t* model);

/**
 * Function used by the general ASSERT macro to handle its failure
 *
 * @param[in] model the model to handle
 */
void ct_general_assert_failed(ct_model_t* model);

///@}

#endif /* CT_ASSERT_H_ */
