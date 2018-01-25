/**
 * @file
 *
 * Represents the model of @crashc.
 *
 * All the data needed by a **single instance of the framework** should be defined in the model.
 * With this design, one can potentially run several, different, instances of @crashc within the same process,
 * since they would use different struct ct_model instances.
 *
 * @author koldar
 * @date Oct 11, 2017
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <signal.h>
#include <setjmp.h>

#include "typedefs.h"
#include "section.h"
#include "report_producer.h"
#include "list.h"

/**
 * The maximum number of registrable suites
 *
 * This macro is used to set the initial size of the internal array used to contain the tests functions pointers
 */
#ifndef MAX_TESTS
#   define MAX_TESTS 256
#endif

/**
 * A collection of required variables used by a run of @crashc to soundly operate
 *
 * @definition @crashc model
 * It's a collection of all the needed variables a single thread of @crashc needs
 */
struct ct_model {
	/**
	 * Array containing the pointers to the testsuites functions
	 */
	ct_test_c tests_array[MAX_TESTS];
	/**
	 * The pointer to the global teardown function
	 *
	 * This is @null by default, and gets populated with a function pointer whenever the user uses the AFTER_TESTS macro.
	 *
	 * @definition
	 * The global teardown function is the last piece of code to execute before the struct ct_model model used by the framework is tore down.
	 * It's basically the last place where you can put custom code
	 */
	ct_teardown_c ct_teardown;
	/**
	 * Variablee used to keep track of the @testsuite registered
	 *
	 * Whilst registering @testsuite, the field represents the number of @testsuite you've registered so far.
	 * After registered all the @testsuite, the field represents the numbr of @testsuite registered in the run:
	 * you can use the values to understand where the field ct_model::tests_array ends to have meanings
	 *
	 * @see REGISTER_SUITE
	 */
	int suites_array_index;
	/**
	 * The list of the test reports generated by this instance of crash c test
	 *
	 * The payloads within the list are of type ::ct_test_report.
	 * Each of them represent the outcome of the test of a run within a @testcase loop.
	 */
	ct_list_o* test_reports_list;
	/**
	 * field representing the root of the section tree
	 *
	 * @ref section_tree
	 */
	struct ct_section* root_section;
	/**
	 * Represents the @containablesection we're analyzing right now in a given time when we're running test code.
	 *
	 * Suppose you have the following code:
	 *
	 * @include exampleOfTestCase.dox
	 *
	 * which translate to the following section tree:
	 *
	 * @dotfile exampleOfTestCase.dot
	 *
	 * Suppose the program is inside "when 2": you need something that allows you to synchronize:
	 * \li where @crashc thinks the code is running;
	 * \li where the code is actually running;
	 *
	 * This means you need something that points which @containablesection you're actually in. This variable is that pointer.
	 *
	 * When you're in "when 2" this variable is set to the node in the tree representing "when 2". When you enter inside the code of "then1",
	 * this variable is reset to point "then 1" ::ct_section. Then, when you return to "when 2" to execute the code between "then 1" and "then 2", this variable
	 * is set again in a way to point "when2".
	 */
	struct ct_section* current_section;
	/**
	 * Pointer to the current section snapshot of ct_model_ct::current_section
	 *
	 * This is used by test reports to store the flow of execution followed by the tests
	 */
	struct ct_snapshot* current_snapshot;
	/**
	 * The ::TESTCASE @crashc is handling right now
	 *
	 * @crashc can handle at most 1 test case per time
	 */
	struct ct_section* jump_source_testcase;
	/**
	 * Represents the tags the user has specified as the only ones that should be consider
	 *
	 * A test is run only if it declares at least one tag inside this container.
	 * If the test is in conflict with ct_model::excludeTags , ct_model::excludeTags has the precedence.
	 *
	 * If this hashtable is empty, then we consider as if the check does't need to happen
	 *
	 * @see struct ct_model::excludeTags
	 */
	ct_tag_hashtable_o* run_only_if_tags;
	/**
	 * Represents the tags the user has specified as the ones that excludes tests
	 *
	 * A test is skipped if it declares at least one tag inside this container.
	 * If the test is in conflict with ct_model::run_only_if_tags, ct_model::excludeTags has the precedence
	 *
	 * If this hashtable is empty, then we consider as if the check does't need to happen
	 *
	 * @see struct ct_model::run_only_if_tags
	 */
	ct_tag_hashtable_o* exclude_tags;
	/**
	 * Used to store the execution state to be restored thanks to setjmp and longjmp when needed during faulty test execution.
	 *
	 * By faulty test we mean tests which generate the following scenarios:
	 * \li some signal is detected (i.e. SIGSEGV or SIGFPE);
	 * \li an assertion failed;
	 */
	jmp_buf jump_point;
	/**
	 * Represents the sigaction flag required for intercepting signals
	 *
	 * @see struct ct_model::jump_point
	 */
	struct sigaction _crashc_sigaction;
	/**
	 * contains severla  statistical informations about the tests.
	 *
	 * Example may be:
	 * 	\li the number of total tests;
	 * 	\li the number of failed tests;
	 * 	\li and so on...;
	 */
	struct ct_test_stats* statistics;
	/**
	 * The implementation of the report producer used by the model.
	 *
	 * This contains the functions used to create test reports for the user to overview
	 */
	struct ct_report_producer* report_producer_implementation;
	/**
	 * The file where to write the report on
	 */
	FILE* output_file;
};

/**
 * Create a model correctly initialized
 *
 * @return a setupped model
 */
struct ct_model* ct_setup_default_model();

/**
 * Destroy every memory allocated by the model
 *
 * @param[in] ccm the model to destroy
 */
void ct_teardown_default_model(struct ct_model* ccm);

#endif /* MODEL_H_ */
