/*
 * ct_assert.c
 *
 *  Created on: Nov 15, 2017
 *      Author: noodles
 */

#include <string.h>
#include <setjmp.h>
#include <test_report.h>

#include "ct_assert.h"
#include "errors.h"
#include "section.h"

ct_assert_report_t* ct_init_assert_report(bool is_mandatory, char* asserted_text, char* file, unsigned int line) {
	ct_assert_report_t * ret_val = malloc(sizeof(ct_assert_report_t));

	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->passed = true;
	ret_val->is_mandatory = is_mandatory;
	ret_val->asserted = asserted_text;
	ret_val->expected_value = NULL;
	ret_val->expected_str = NULL;
	ret_val->actual_value = NULL;
	ret_val->actual_str = NULL;
	ret_val->file_name = file;
	ret_val->line_number = line;

	return ret_val;
}

//TODO: We might add a field to the assert_report struct to point at a specific destructor in order to be able to
//		precisely control how to destroy a specific report, which might have been generated by a different type of assertion
//		which, for example, needs to malloc the memory for its strings
void ct_destroy_assert_report(ct_assert_report_t * report) {
	free(report);
}

void ct_assert_do_nothing(ct_model_t * model) {

}

void ct_general_assert_failed(ct_model_t * model) {

	SectionSnapshot* snapshot = model->current_snapshot;
	ct_assert_report_t* report = getTailOfList(snapshot->assertion_reports);
	ct_test_report_t* test_report = getTailOfList(model->test_reports_list);

	//Update the assertion report
	report->passed = false;
	report->expected_str = "true";
	report->actual_str = "false";

	//Update the status of the snapshot which contained this assertion and of the test
	snapshot->status = SNAPSHOT_FAILED;
	ct_update_test_outcome(test_report, snapshot);

	//We then need to reset the current snapshot to start a new snapshot tree
	model->current_snapshot = NULL;

	//We jump out of this flow of execution to go back at the beginning of the test case and go on with the execution of the other tests
	siglongjmp(model->jump_point, ASSERT_JUMP_CODE);
}


