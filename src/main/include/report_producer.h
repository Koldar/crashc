/*
 * report_producer.h
 *
 *  Created on: Oct 27, 2017
 *      Author: Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#ifndef REPORT_PRODUCER_H_
#define REPORT_PRODUCER_H_

#include <stdio.h>
#include <test_report.h>

#include "section.h"
#include "list.h"
#include "typedefs.h"
#include "model.h"

/**
 * This struct is used to contain the statistics used by CrashC to give the user
 * additional info on the run tests
 */
struct ct_test_statistics_t {

	/**
	 * The number of total tests
	 */
	unsigned int total_tests;

	/**
	 * The number of passed tests
	 */
	unsigned int successful_tests;

	/**
	 * The number of  failed tests
	 */
	unsigned int failed_tests;

};

/**
 * This structure contains the function pointers to the implementations
 * of the function used to produce the report of the particular parts of the tests.
 * This allows for easy customization and high code maintainability
 */
struct ct_report_producer_t {

	ct_test_reporter_t test_reporter;

	ct_snapshot_tree_reporter_t snapshot_tree_reporter;

	ct_summary_reporter_t summary_producer;

	ct_assert_reporter_t assert_reporter;

	ct_reporter_t report_producer;

};

/**
 * Prints the string representation of a snapshot status
 */
char * ct_snapshot_status_to_string(snapshot_status status);

/**
 * Prints the string representation of a section type
 */
char * ct_section_type_to_string(section_type type);

/**
 * Prints the report of the executed tests in a default format
 */
void ct_default_report(struct crashc_model * model);
/**
 * Prints the tests summary in a default format
 */
void ct_default_report_summary(crashc_model * model);

/**
 * Prints a the report of a single test with a default format
 */
void ct_default_test_report(crashc_model * model, ct_test_report_t * report);

/**
 * Prints a representation of a snapshot tree in a default format
 */
void ct_default_snapshot_tree_report(crashc_model * model, SectionSnapshot * snapshot, int level);

/**
 * Prints a default report for the assertions related to a specific snapshot
 */
void ct_default_assertions_report(crashc_model* model, SectionSnapshot* snapshot, int level);

/**
 * Initializes and return a structure containing the statistics data related to the tests
 */
ct_test_statistics_t * initStatistics();

/**
 * Initializes the default report producer structure
 */
ct_report_producer_t * initDefaultReportProducer();

/**
 * Frees the memory occupied by a ct_test_statistics_t struct
 */
void destroyStatistics(ct_test_statistics_t * stats);

/**
 * Frees the memory allocated for the default report producer
 */
void destroyDefaultReportProducer(ct_report_producer_t * producer);

#endif /* REPORT_PRODUCER_H_ */
