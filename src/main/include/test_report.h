/**
 * @file
 *
 * Contains the definitions used by @crashc to handle the test reports
 *
 * @author koldar
 * @date Feb 16, 2017
 *
 */

#ifndef TEST_REPORT_H_
#define TEST_REPORT_H_

#include <string.h>
#include <stdlib.h>

#include "section.h"
#include "errors.h"
#include "tag.h"
#include "typedefs.h"

/**
 * Represents the possible outcomes of a single test:
 */
typedef enum {
	/**
	 * A test passed
	 *
	 * @definition Passed Test
	 * It's a **test** containing no failed assertions
	 */
	TEST_SUCCESS,
	/**
	 * A test didn't pass
	 *
	 * @definition Failed Test
	 * It's a **test** which is not a **passed test**.
	 */
	TEST_FAILURE
} ct_test_outcome_t;

/**
 * Represents the actual test report.
 *
 * It contains all the informations regarding the status of an executed test.
 * Note that @crashc considers a test to be a single flow of execution of a ::LOOPER (i.e. A single iteration of a @testcase)
 * E.g:
 * Let's suppose we have this @testcase:
 *
 * @code
 * TESTCASE("...", "...") {
 *     WHEN("outer when", "") {
 *         WHEN("inner when 1", "") {
 *             ...some code...
 *         }
 *         WHEN("inner when 2", "") {
 *             ...other code...
 *         }
 *     THEN("then", "") {
 *         ...then code...
 *     }
 * }
 * @endcode
 *
 * The flow of execution of this @testcase would be:
 * -# inner when 1 -> then
 * -# inner when 2 -> then
 *
 * These are what @crashc considers to be tests.
 * This implies that the informations stored in the ct_test_report_t struct are related to the flow of execution, not to a given section.
 * This is the reason why the ct_test_report_t struct needs an auxiliary struct, ::struct ct_snapshot, to hold the information on the status of
 * the sections involved in the test at the moment they were executed.
 */
struct ct_test_report {
	/**
	 * The name of the file that contained this test
	 *
	 * @notnull
	 */
	char * filename;
	/**
	 * The snapshot of the testcase that contained this test.
	 * This is actually the root of a tree which contains the section path followed by the test
	 *
	 *
	 * @notnull
	 */
	 //TODO is this field always not null? Ask lorenzo
	 struct ct_snapshot * testcase_snapshot;
	 /**
	  * The outcome of the test.
	  */
	 ct_test_outcome_t outcome;
	/**
	 * The time that it took to complete the test
	 *
	 * Note that execution times might be higher than expected due to the necessary
	 * overhead introduced by the internal code created by @crashc to properly
	 * guide the tests' execution flow
	 */
	long execution_time;
};

/**
 * create in memory a new test report
 *
 * \note
 * the test report created is assumed to have state ::TEST_SUCCESS
 *
 * @param[in] tc_snapshot the snapshot tree associated to the test report
 * @return the test report desired;
 */
ct_test_report_t* ct_init_test_report(struct ct_snapshot* tc_snapshot);
/**
 * release from the memory a ct_test_report_t
 *
 * @param[inout] report the report to dispose of. @notnull
 */
void ct_destroy_test_report(ct_test_report_t* report);

//TODO why the init section snapshot is defined here while the struct is defined in section.h?
/**
 * Initialize in the heap a new section snapshot
 *
 * @param[in] section the section represented by the ::struct ct_snapshot you want to build
 * @return the requested ::struct ct_snapshot
 */
struct ct_snapshot* ct_init_section_snapshot(struct ct_section* section);
/**
 * Release from memory a section snapshot
 *
 * @param[inout] snapshot the snapshot to remove from the memory
 */
void ct_destroy_snapshot_tree(struct ct_snapshot* snapshot);

/**
 * Adds the given section to the section tree
 *
 * \note
 * It also automatically reorganize the tree's children
 *
 * @param[in] to_add snapshot to add
 * @param[inout] tree root of the snapshot tree to rearrange
 * @return \c to_add value
 */
struct ct_snapshot* ct_add_snapshot_to_tree(struct ct_snapshot* to_add, struct ct_snapshot* tree);

/**
 * Checks the status of the section associated to a given snapshot in order to change coherently the status of the snapshot.
 *
 * This is needed due to the fact
 * that every snapshot is created when entering at the beginning of the section and by default
 * is set as ::CT_SNAPSHOT_OK, so it discovers if any error occourred only at the end, by checking the section status.
 *
 * @param[in] section to status to check
 * @param[inout] snapshot whose status we need to alter
 */
void ct_update_snapshot_status(struct ct_section* section, struct ct_snapshot* snapshot);

/**
 * Checks the status of the last executed snapshot in the test tree and sets the status of the test as needed
 *
 * @param[inout] report the report to update
 * @param[in] last_snapshot the snapshot whose result we need to integrate to \c report
 */
void ct_update_test_outcome(ct_test_report_t* report, struct ct_snapshot* last_snapshot);

#endif /* TEST_REPORT_H_ */
