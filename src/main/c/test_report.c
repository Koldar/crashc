/**
 * This file contains the function used by CrashC to generate and handle the test reports
 * TODO: Fix strdup unsafe call, add dimension limit to avoid overflows
 *
 * Author: Lorenzo Nodari
 *
 */

#include <string.h>

#include "test_report.h"
#include "list.h"
#include "assertions.h"

ct_test_report_t* ct_init_test_report(struct ct_snapshot* tc_snapshot) {
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

struct ct_snapshot* ct_init_section_snapshot(struct ct_section* section) {
	struct ct_snapshot* ret_val = malloc(sizeof(struct ct_snapshot));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->description   = strdup(section->description);
	ret_val->tags          = section->tags;
	ret_val->type          = section->type;
	ret_val->status        = CT_SNAPSHOT_OK;
	ret_val->elapsed_time  = 0;
	ret_val->assertion_reports = ct_list_init();
	ret_val->parent = NULL;
	ret_val->next_sibling = NULL;
	ret_val->first_child = NULL;

	return ret_val;
}

void ct_destroy_snapshot_tree(struct ct_snapshot* snapshot) {
	free(snapshot->description);
	ct_list_destroy_with_elements(snapshot->assertion_reports, (ct_destructor_t) ct_destroy_assert_report);

	struct ct_snapshot* next_child = snapshot->first_child;
	while (next_child != NULL) {
		struct ct_snapshot* tmp = next_child->next_sibling;
		ct_destroy_snapshot_tree(next_child);
		next_child = tmp;
	}

	free(snapshot);
}

struct ct_snapshot* ct_add_snapshot_to_tree(struct ct_snapshot* to_add, struct ct_snapshot* tree) {
	to_add->parent = tree;

	struct ct_snapshot* tmp = tree->first_child;
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

void ct_update_snapshot_status(struct ct_section* section, struct ct_snapshot* snapshot) {
	if (section->status == CT_SECTION_SIGNAL_DETECTED) {
		snapshot->status = CT_SNAPSHOT_SIGNALED;
	}
}

void ct_update_test_outcome(ct_test_report_t* report, struct ct_snapshot* last_snapshot) {
	if (last_snapshot->status != CT_SNAPSHOT_OK) {
		report->testcase_snapshot->status = last_snapshot->status;
		report->outcome = TEST_FAILURE;
	}
}


