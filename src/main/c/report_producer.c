/*
 * report_producer.c
 *
 *  Created on: Oct 27, 2017
 *      Author: Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#include <stdlib.h>

#include "report_producer.h"
#include "errors.h"

char * ct_snapshot_status_to_string(snapshot_status s) {

	switch (s) {
		case SNAPSHOT_OK: return "OK";
		case SNAPSHOT_SIGNALED: return "SIGNALED";
		case SNAPSHOT_FAILED: return "FAILED";
		default: 	printf("\nERROR: Unrecognized snapshot status, exiting.\n");
					exit(1); //TODO: Fix error exit
	}

}

char * ct_section_type_to_string(section_type t) {

	switch (t) {
		case ST_WHEN: return "WHEN";
		case ST_THEN: return "THEN";
		case ST_TESTCASE: return "TESTCASE";
		case ST_ROOT: return "ROOT";
		case ST_SUITECASE: return "SUITE";
		default: 	printf("\nERROR: Unrecognized section type, exiting.\n");
					exit(1); //TODO: Fix error exit
	}

}


//TODO: Check for errors due to FILE null pointer etc...
void ct_default_snapshot_tree_report(crashc_model * model, SectionSnapshot * snapshot, int level) {

	char * type_str = ct_section_type_to_string(snapshot->type);
	char * status_str = ct_snapshot_status_to_string(snapshot->status);
	printf("%s : %s -> %s\n", type_str, snapshot->description, status_str);
	FILE * file = model->output_file;

	SectionSnapshot * child = snapshot->first_child;
	while (child != NULL) {
		for (int i = 0; i < level; i++) {
			putchar('\t');
		}
		ct_default_snapshot_tree_report(model, child, level + 1);
		child = child->next_sibling;
	}

}

void ct_default_test_report(crashc_model * model, TestReport * report) {

	FILE * file = model->output_file;

	fprintf(file, " ---------- TEST REPORT ----------\n\n");
	//fprintf(file, "File: %s\n\n", report->filename);
	ct_default_snapshot_tree_report(model, report->testcase_snapshot, 1);
	fprintf(file, "\nOutcome: %s\n", (report->outcome == TEST_SUCCESS) ? "SUCCESS" : "FAILURE");
	fprintf(file, "\n --------------------------------\n");
	fprintf(file, "\n\n");

}

void ct_default_report_summary(crashc_model * model) {

	FILE * file = model->output_file;
	ct_test_statistics_t * stats = model->statistics;

	fprintf(file, "Total tests: %d\n", stats->total_tests);
	fprintf(file, "Successful tests: %d\n", stats->successful_tests);
	fprintf(file, "Failed tests: %d\n", stats->failed_tests);
	fprintf(file, "Percentage of successful tests: %.2f%%\n", ((double) stats->successful_tests / stats->total_tests) * 100);

}

void ct_default_report(crashc_model * model) {

	list * report_list = model->test_reports_list;

	ITERATE_ON_LIST(report_list, report_cell, report, TestReport *) {
		if (report->outcome == TEST_SUCCESS) {
			model->statistics->successful_tests++;
		}
		else {
			model->statistics->failed_tests++;
		}
		ct_default_test_report(model, report);
	}

	model->statistics->total_tests = getLengthOfList(report_list);
	ct_default_report_summary(model);

}

ct_test_statistics_t * initStatistics() {

	ct_test_statistics_t * retVal = malloc(sizeof(ct_test_statistics_t));

	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->total_tests = 0;
	retVal->successful_tests = 0;
	retVal->failed_tests = 0;

	return retVal;
}

ct_report_producer_t * initDefaultReportProducer() {

	ct_report_producer_t * ret_val = malloc(sizeof(ct_report_producer_t));

	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->test_reporter = ct_default_test_report;
	ret_val->snapshot_tree_reporter = ct_default_snapshot_tree_report;
	ret_val->summary_producer = ct_default_report_summary;
	ret_val->report_producer = ct_default_report;

	return ret_val;

}

void destroyStatistics(ct_test_statistics_t * stats) {

	free(stats);

}

void destroyDefaultReportProducer(ct_report_producer_t * producer) {

	free(producer);

}

