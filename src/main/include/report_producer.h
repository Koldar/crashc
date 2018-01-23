/**
 * @file
 *
 * Represents a entity which produces a report summarizing the test outcomes for the user
 *
 * An example of a report producer is the entity which prints the outcomes of the tests in a file
 *
 * @date Oct 27, 2017
 * @author Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#ifndef REPORT_PRODUCER_H_
#define REPORT_PRODUCER_H_

#include <stdio.h>

#include "test_report.h"
#include "section.h"
#include "list.h"
#include "typedefs.h"
#include "model.h"

/**
 * Struct used to contain the statistics used by @crashc to give the user additional info on the run tests
 */
struct ct_test_statistics_t {
	/**
	 * The number of total tests @crashc have detected
	 *
	 * @definition Test
	 * It's is a single loop of a @testcase
	 */
	unsigned int total_tests;
	/**
	 * The number of passed tests
	 *
	 */
	unsigned int successful_tests;
	/**
	 * The number of  failed tests
	 *
	 */
	unsigned int failed_tests;
};

/**
 * Contains function pointers to the implementations of the function used to produce the report of the particular parts of the tests.
 *
 * @definition Report Producer
 * It's an entity which, given a in-memory representation of the outcome of the tests just performed by @crashc,
 * generates an output (e.g. json, xml, sql db) which nicely summarizes the outcomes of the tests.
 *
 * This allows for easy customization and high code maintainability. Use this struct to generate your custom test reporter.
 * For example, one may want to create a test reporter which save its data ina mysqlite database.
 */
struct ct_report_producer_t {

	ct_test_reporter_t test_reporter;

	ct_snapshot_tree_reporter_t snapshot_tree_reporter;

	ct_summary_reporter_t summary_producer;

	ct_assert_reporter_t assert_reporter;

	ct_reporter_t report_producer;

};

/**
 * Prints the string representation of a ::ct_snapshot_status
 *
 * @param[in] status the status to compute the string representation of
 * @return a string representation of \c status
 */
char* ct_snapshot_status_to_string(enum ct_snapshot_status status);

/**
 * Prints the string representation of a ::ct_section_type
 *
 * @param[in] type the type to compute the string representation of
 * @return a string representation of \c type
 */
char* ct_section_type_to_string(enum ct_section_type type);

//TODO in my opinion these functiosn should be placed in another header called like "default_report_producer.h"

/**
 * @addtogroup defaultReportProducer Default Report Producer
 * @brief a ct_report_producer_t which print the summary of @crashc test outcomes in a file by writing the test correctly indented
 * @{
 */

/**
 * Prints the report of the executed tests in a default format
 *
 * \note
 * The report will be printed in the file specified by ct_model_t::output_file
 *
 * @param[inout] model the model to handle
 */
void ct_default_report(ct_model_t* model);
/**
 * Prints the tests summary in a default format
 *
 * \note
 * The report will be printed in the file specified by ct_model_t::output_file
 *
 * @param[inout] model the model to handle
 */
void ct_default_report_summary(ct_model_t* model);

/**
 * Prints a the report of a single test with a default format
 *
 * \note
 * The report will be printed in the file specified by ct_model_t::output_file
 *
 * @param[inout] model the model to handle
 * @param[inout] report the test report to put in the file
 */
void ct_default_test_report(ct_model_t* model, ct_test_report_t* report);

/**
 * Prints a representation of a snapshot tree in a default format
 *
 * \note
 * The report will be printed in the file specified by ct_model_t::output_file
 *
 * @param[inout] model the model to manage
 * @param[inout] snapshot the snapshot to write into the file
 * @param[in] level the depth level \c snapshot is in the snapshot tree
 */
void ct_default_snapshot_tree_report(ct_model_t* model, struct ct_snapshot* snapshot, int level);

/**
 * Prints a default report for the assertions related to a specific snapshot
 *
 * \note
 * The report will be printed in the file specified by ct_model_t::output_file
 *
 * @param[inout] model the model to manage
 * @param[inout] snapshot the snapshot to write into the file
 * @param[in] level the depth level \c snapshot is in the snapshot tree
 *
 */
void ct_default_assertions_report(ct_model_t* model, struct ct_snapshot* snapshot, int level);

///@}

/**
 * Creates a new initialized structure in the heap
 *
 * @return a structure containing the statistics data related to the tests
 */
ct_test_statistics_t* ct_init_stats();

/**
 * Creates and initializes in memory a new default report producer structure
 *
 * @return a structure representing a naive report producer
 */
ct_report_producer_t* ct_init_default_report_producer();

/**
 * Frees the memory occupied by a ct_test_statistics_t
 *
 * @param[inout] stats the structure to dispose from the memory
 */
void ct_destroy_stats(ct_test_statistics_t* stats);

/**
 * Frees the memory allocated for the default report producer
 *
 * @param[inout] producer the structure to dispose from the mermory
 */
void ct_destroy_default_report_producer(ct_report_producer_t* producer);

#endif /* REPORT_PRODUCER_H_ */
