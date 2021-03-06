/*
 * report_producer.c
 *
 *  Created on: Oct 27, 2017
 *      Author: Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#include <stdlib.h>

#include "report_producer.h"
#include "assertions.h"
#include "errors.h"

char* ct_snapshot_status_to_string(enum ct_snapshot_status s) {

	switch (s) {
		case CT_SNAPSHOT_OK: return "OK";
		case CT_SNAPSHOT_SIGNALED: return "SIGNALED";
		case CT_SNAPSHOT_FAILED: return "FAILED";
		default: 	printf("\nERROR: Unrecognized snapshot status, exiting.\n");
					exit(1); //TODO: Fix error exit
	}

}

char* ct_section_type_to_string(enum ct_section_type t) {

	switch (t) {
		case CT_WHEN_SECTION: return "WHEN";
		case CT_THEN_SECTION: return "THEN";
		case CT_TESTCASE_SECTION: return "TESTCASE";
		case CT_ROOT_SECTION: return "ROOT";
		case CT_TESTSUITE_SECTION: return "SUITE";
		default: 	printf("\nERROR: Unrecognized section type, exiting.\n");
					exit(1); //TODO: Fix error exit
	}

}


//TODO: Check for errors due to FILE null pointer etc...
void ct_default_snapshot_tree_report(struct ct_model* model, struct ct_snapshot* snapshot, int level) {

	FILE* file = model->output_file;

	char* type_str = ct_section_type_to_string(snapshot->type);
	char* status_str = ct_snapshot_status_to_string(snapshot->status);
	fprintf(file, "%s : %s -> %s\n", type_str, snapshot->description, status_str);
	ct_default_assertions_report(model, snapshot, level);

	struct ct_snapshot* child = snapshot->first_child;
	while (child != NULL) {
		for (int i = 0; i < level; i++) {
			putchar('\t');
		}
		ct_default_snapshot_tree_report(model, child, level + 1);
		child = child->next_sibling;
	}

}

void ct_default_test_report(struct ct_model* model, struct ct_test_report* report) {

	FILE* file = model->output_file;

	fprintf(file, " ---------- TEST REPORT ----------\n\n");
	//fprintf(file, "File: %s\n\n", report->filename);
	ct_default_snapshot_tree_report(model, report->testcase_snapshot, 1);
	fprintf(file, "\nOutcome: %s\n", (report->outcome == CT_TEST_SUCCESS) ? "SUCCESS" : "FAILURE");
	fprintf(file, "\n --------------------------------\n");
	fprintf(file, "\n\n");

}

void ct_default_report_summary(struct ct_model* model) {

	FILE* file = model->output_file;
	struct ct_test_stats * stats = model->statistics;

	fprintf(file, "Total tests: %d\n", stats->total_tests);
	fprintf(file, "Successful tests: %d\n", stats->successful_tests);
	fprintf(file, "Failed tests: %d\n", stats->failed_tests);
	fprintf(file, "Percentage of successful tests: %.2f%%\n", ((double) stats->successful_tests / stats->total_tests) * 100);

}

void ct_default_assertions_report(struct ct_model* model, struct ct_snapshot* snapshot, int level) {

	FILE* file = model->output_file;
	ct_list_o* assertion_reports = snapshot->assertion_reports;

	CT_ITERATE_ON_LIST(assertion_reports, report_cell, report, struct ct_assert_report*) {
		for (int i = 0; i < level; i++) {
			putchar('\t');
		}

		if (report->passed) {
			fprintf(file, "Assertion \"%s\" - OK\n", report->asserted);
		}
		else {
			fprintf(file, "Assertion \"%s\" - FAILED - Expected: %s, Actual: %s\n", report->asserted, report->expected_str, report->actual_str);
		}
	}

}

void ct_default_report(struct ct_model* model) {

	ct_list_o* report_list = model->test_reports_list;

	//TODO: Move the stats-acquiring code away from the report producer
	CT_ITERATE_ON_LIST(report_list, report_cell, report, struct ct_test_report*) {
		if (report->outcome == CT_TEST_SUCCESS) {
			model->statistics->successful_tests++;
		}
		else {
			model->statistics->failed_tests++;
		}
		ct_default_test_report(model, report);
	}

	model->statistics->total_tests = ct_list_size(report_list);
	ct_default_report_summary(model);

}

struct ct_test_stats* ct_init_stats() {

	struct ct_test_stats* ret_val = malloc(sizeof(struct ct_test_stats));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->total_tests = 0;
	ret_val->successful_tests = 0;
	ret_val->failed_tests = 0;

	return ret_val;
}

struct ct_report_producer* ct_init_default_report_producer() {

	struct ct_report_producer* ret_val = malloc(sizeof(struct ct_report_producer));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->test_reporter = ct_default_test_report;
	ret_val->snapshot_tree_reporter = ct_default_snapshot_tree_report;
	ret_val->summary_producer = ct_default_report_summary;
	ret_val->assert_reporter = ct_default_assertions_report;
	ret_val->report_producer = ct_default_report;

	return ret_val;

}

void ct_destroy_stats(struct ct_test_stats* stats) {

	free(stats);

}

void ct_destroy_default_report_producer(struct ct_report_producer* producer) {

	free(producer);

}


