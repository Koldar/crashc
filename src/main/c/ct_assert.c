/*
 * ct_assert.c
 *
 *  Created on: Nov 15, 2017
 *      Author: noodles
 */

#include <string.h>

#include "ct_assert.h"
#include "errors.h"

ct_assert_report_t *  ct_generate_assert_report(bool passed, bool is_mandatory, char * text, char * expected, char * actual, char * file, char * line) {
	ct_assert_report_t * ret_val = malloc(sizeof(ct_assert_report_t));

	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->passed = passed;
	ret_val->is_mandatory = is_mandatory;
	ret_val->asserted = text;
	ret_val->expected = expected;
	ret_val->actual = actual;
	ret_val->file_name = file;
	ret_val->line_number = line;

	return ret_val;
}

void ct_destroy_assert_report(ct_assert_report_t * report) {
	free(report->actual);
	free(report);
}

void ct_assert_passed(crashc_model * model, char * asserted, char * expected, char * file, char * line) {
	//Create the report related to this assertion
	ct_assert_report_t * report = ct_generate_assert_report(true, true, asserted, "true", strdup("false"), __FILE__, __LINE__);

	//Add the report to the report list in the section snapshot
	addTailInList(model->currentSnapshot->assertion_reports, report);
}

void ct_assert_failed(crashc_model * model, char * asserted, char * expected, char * file, char * line) {

}


