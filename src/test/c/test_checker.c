/*
 * \file test_checker.c
 *
 * A naive module used for internal automated testing
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#include "test_checker.h"
#include "report_producer.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

static char TEST_CHECKER_BUFFER[1000];
static int next_position;
static char split = ' ';
static void ct_testing_snapshot_tree_report(struct ct_model* model, struct ct_snapshot* snapshot, int level);
static void ct_testing_summary_producer(struct ct_model * model);
static void ct_testing_report(struct ct_model * model);
static void ct_testing_test_report(struct ct_model * model, struct ct_test_report* report);
static char* ct_testing_outcome_to_s(enum ct_test_outcome outcome);
static char* ct_testing_section_type_to_c(enum ct_section_type type);
static char* ct_testing_snapshot_status_to_s(enum ct_snapshot_status status);

void clear_test_checker() {
	next_position = 0;
	TEST_CHECKER_BUFFER[next_position] = '\0';
}

void set_separator(char s) {
	split = s;
}

void add_char(char ch) {
	TEST_CHECKER_BUFFER[next_position] = ch;
	next_position += 1;
	TEST_CHECKER_BUFFER[next_position] = '\0';
}

void add_str(const char* str, bool add_split_char) {
	strcpy(&TEST_CHECKER_BUFFER[next_position], str);
	next_position += strlen(str);

	if (add_split_char) {
		add_char(split);
	}
}

bool check_test_checker(const char* expected) {
	return (strcmp(TEST_CHECKER_BUFFER, expected) == 0);
}

bool check_and_reset_test_checker(const char* expected) {
	bool ret_val = (strcmp(TEST_CHECKER_BUFFER, expected) == 0);
	clear_test_checker();
	return ret_val;
}

void assert_and_reset_test_checker(const char* expected) {
	if (!check_test_checker(expected)) {
		fprintf(stdout, "KO!\n");
		fprintf(stderr, "EXPECTED: %s\nACTUAL:   %s\n", expected, TEST_CHECKER_BUFFER);
		assert(false);
	} else {
		fprintf(stdout, "OK!\n");
	}
	clear_test_checker();
}

static char* ct_testing_outcome_to_s(enum ct_test_outcome outcome) {
	if (outcome == CT_TEST_SUCCESS) {
		return "OK";
	}
	else {
		return "NO";
	}
}

static char* ct_testing_section_type_to_s(enum ct_section_type type) {
	switch (type) {
		case CT_ROOT_SECTION: return "ROOT";
		case CT_TESTSUITE_SECTION: return "SUITE";
		case CT_TESTCASE_SECTION: return "TESTCASE";
		case CT_WHEN_SECTION: return "WHEN";
		case CT_THEN_SECTION: return "THEN";
		default: return "???";
	}
}

static char* ct_testing_snapshot_status_to_s(enum ct_snapshot_status status) {
	switch (status) {
		case CT_SNAPSHOT_OK: return "OK";
		case CT_SNAPSHOT_SIGNALED: return "SIG";
		case CT_SNAPSHOT_FAILED: return "FAIL";
		default: return "???";
	}
}

static void ct_testing_snapshot_tree_report(struct ct_model* model, struct ct_snapshot* snapshot, int level) {
	char level_str[12];
	sprintf(level_str, "%d", level);
	add_str(level_str, false);
	add_char('|');
	add_str(snapshot->description, false);
	add_char('|');
	add_str(ct_testing_snapshot_status_to_s(snapshot->status), false);
	add_char('_');

	struct ct_snapshot* child = snapshot->first_child;
	while (child != NULL) {
		ct_testing_snapshot_tree_report(model, child, level + 1);
		child = child->next_sibling;
	}
}

static void ct_testing_summary_producer(struct ct_model* model) {
	
}

static void ct_testing_report(struct ct_model* model) {

	ct_list_o* report_list = model->test_reports_list;

	ITERATE_ON_LIST(report_list, report_cell, report, struct ct_test_report*) {
		ct_testing_test_report(model, report);
	}
	
}

static void ct_testing_test_report(struct ct_model* model, struct ct_test_report* report) {
	add_str(ct_testing_outcome_to_s(report->outcome), false);
	add_char('-');
	ct_testing_snapshot_tree_report(model, report->testcase_snapshot, 1);
	add_char(split);
}

void setup_testing_producer(struct ct_model* model) {
	struct ct_report_producer* old_producer = model->report_producer_implementation;

	old_producer->snapshot_tree_reporter = ct_testing_snapshot_tree_report;
	old_producer->test_reporter			 = ct_testing_test_report;
	old_producer->summary_producer		 = ct_testing_summary_producer;
	old_producer->report_producer		 = ct_testing_report;
	 
}
