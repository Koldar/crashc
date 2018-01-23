/*
 * \file test_checker.c
 *
 * A naive module us
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#include "test_checker.h"
#include "report_producer.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

static char __TEST_CHECKER_BUFFER[1000];
static int nextPosition;
static char split = ' ';
static void ct_testing_snapshot_tree_report(ct_model_t * model, struct ct_snapshot * snapshot, int level);
static void ct_testing_summary_producer(ct_model_t * model);
static void ct_testing_report(ct_model_t * model);
static void ct_testing_test_report(ct_model_t * model, ct_test_report_t* report);
static char * ct_testing_outcome_to_s(ct_test_outcome_t outcome);
static char * ct_testing_section_type_to_c(enum ct_section_type type);
static char * ct_testing_snapshot_status_to_s(enum ct_snapshot_status status);

void clearTestChecker() {
	nextPosition = 0;
	__TEST_CHECKER_BUFFER[nextPosition] = '\0';
}

void setSplit(char s) {
	split = s;
}

void addCharacter(char ch) {
	__TEST_CHECKER_BUFFER[nextPosition] = ch;
	nextPosition += 1;
	__TEST_CHECKER_BUFFER[nextPosition] = '\0';
}

void addString(const char* str, bool add_split_char) {
	strcpy(&__TEST_CHECKER_BUFFER[nextPosition], str);
	nextPosition += strlen(str);

	if (add_split_char) {
		addCharacter(split);
	}
}

bool checkTestChecker(const char* expected) {
	return (strcmp(__TEST_CHECKER_BUFFER, expected) == 0);
}

bool checkTestCheckerAndReset(const char* expected) {
	bool retVal = (strcmp(__TEST_CHECKER_BUFFER, expected) == 0);
	clearTestChecker();
	return retVal;
}

void assertTestCheckerAndReset(const char* expected) {
	if (!checkTestChecker(expected)) {
		fprintf(stdout, "KO!\n");
		fprintf(stderr, "EXPECTED: %s\nACTUAL:   %s\n", expected, __TEST_CHECKER_BUFFER);
		assert(false);
	} else {
		fprintf(stdout, "OK!\n");
	}
	clearTestChecker();
}

static char * ct_testing_outcome_to_s(ct_test_outcome_t outcome) {
	if (outcome == TEST_SUCCESS) {
		return "OK";
	}
	else {
		return "NO";
	}
}

static char * ct_testing_section_type_to_s(enum ct_section_type type) {
	switch (type) {
		case CT_ROOT_SECTION: return "ROOT";
		case CT_TESTSUITE_SECTION: return "SUITE";
		case CT_TESTCASE_SECTION: return "TESTCASE";
		case CT_WHEN_SECTION: return "WHEN";
		case CT_THEN_SECTION: return "THEN";
		default: return "???";
	}
}

static char * ct_testing_snapshot_status_to_s(enum ct_snapshot_status status) {
	switch (status) {
		case CT_SNAPSHOT_OK: return "OK";
		case CT_SNAPSHOT_SIGNALED: return "SIG";
		case CT_SNAPSHOT_FAILED: return "FAIL";
		default: return "???";
	}
}

static void ct_testing_snapshot_tree_report(ct_model_t * model, struct ct_snapshot * snapshot, int level) {
	char level_str[12];
	sprintf(level_str, "%d", level);
	addString(level_str, false);
	addCharacter('|');
	addString(snapshot->description, false);
	addCharacter('|');
	addString(ct_testing_snapshot_status_to_s(snapshot->status), false);
	addCharacter('_');

	struct ct_snapshot * child = snapshot->first_child;
	while (child != NULL) {
		ct_testing_snapshot_tree_report(model, child, level + 1);
		child = child->next_sibling;
	}
}

static void ct_testing_summary_producer(ct_model_t * model) {
	
}

static void ct_testing_report(ct_model_t * model) {

	ct_list_o * report_list = model->test_reports_list;

	ITERATE_ON_LIST(report_list, report_cell, report, ct_test_report_t *) {
		ct_testing_test_report(model, report);
	}
	
}

static void ct_testing_test_report(ct_model_t * model, ct_test_report_t * report) {
	addString(ct_testing_outcome_to_s(report->outcome), false);
	addCharacter('-');
	ct_testing_snapshot_tree_report(model, report->testcase_snapshot, 1);
	addCharacter(split);
}

void ct_setup_testing_producer(ct_model_t * model) {
	ct_report_producer_t * old_producer = model->report_producer_implementation;

	old_producer->snapshot_tree_reporter = ct_testing_snapshot_tree_report;
	old_producer->test_reporter			 = ct_testing_test_report;
	old_producer->summary_producer		 = ct_testing_summary_producer;
	old_producer->report_producer		 = ct_testing_report;
	 
}
