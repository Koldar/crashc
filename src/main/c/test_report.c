/**
 * This file contains the function used by CrashC to generate and handle the test reports
 * TODO: Fix strdup unsafe call, add dimension limit to avoid overflows
 *
 * Author: Lorenzo Nodari
 *
 */

#include <string.h>
#include <test_report.h>

#include "list.h"
#include "ct_assert.h"

ct_test_report_t* ct_init_test_report(SectionSnapshot* tc_snapshot) {
	ct_test_report_t* ret_val = malloc(sizeof(ct_test_report_t));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->filename = NULL;
	ret_val->execution_time = 0;
	ret_val->outcome = TEST_SUCCESS;
	ret_val->testcase_snapshot = tc_snapshot;

	return ret_val;
}

void ct_destroy_test_report(ct_test_report_t* report) {
	free(report->filename);
	ct_destroy_snapshot_tree(report->testcase_snapshot);
	free(report);

}

SectionSnapshot* ct_init_section_snapshot(Section* section) {
	SectionSnapshot* ret_val = malloc(sizeof(SectionSnapshot));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->description   = strdup(section->description);
	ret_val->tags          = section->tags;
	ret_val->type          = section->type;
	ret_val->status        = SNAPSHOT_OK;
	ret_val->elapsed_time  = 0;
	ret_val->assertion_reports = initList();
	ret_val->parent = NULL;
	ret_val->next_sibling = NULL;
	ret_val->first_child = NULL;

	return ret_val;
}

void ct_destroy_snapshot_tree(SectionSnapshot* snapshot) {
	free(snapshot->description);
	destroyListWithElement(snapshot->assertion_reports, ct_destroy_assert_report);

	SectionSnapshot* next_child = snapshot->first_child;
	while (next_child != NULL) {
		SectionSnapshot* tmp = next_child->next_sibling;
		ct_destroy_snapshot_tree(next_child);
		next_child = tmp;
	}

	free(snapshot);
}

SectionSnapshot* ct_add_snapshot_to_tree(SectionSnapshot* to_add, SectionSnapshot* tree) {
	to_add->parent = tree;

	SectionSnapshot* tmp = tree->first_child;
	if (tmp == NULL) {
			tree->first_child = to_add;
			return to_add;
	}
	while (true) {
		if (tmp->next_sibling != NULL) {
			tmp = tmp->next_sibling;
		}
		else {
			tmp->next_sibling = to_add;
			return to_add;
		}
	}
}

void ct_update_snapshot_status(Section* section, SectionSnapshot* snapshot) {
	if (section->status == SECTION_SIGNAL_DETECTED) {
		snapshot->status = SNAPSHOT_SIGNALED;
	}
}

void ct_update_test_outcome(ct_test_report_t* report, SectionSnapshot* last_snapshot) {
	if (last_snapshot->status != SNAPSHOT_OK) {
		report->testcase_snapshot->status = last_snapshot->status;
		report->outcome = TEST_FAILURE;
	}
}


