/*
 * model.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#include <stdlib.h>
#include <test_report.h>

#include "report_producer.h"
#include "errors.h"
#include "model.h"

ct_model_t* ct_setup_default_model() {
	ct_model_t* ret_val = malloc(sizeof(ct_model_t));

	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->current_section = NULL;
	ret_val->current_snapshot = NULL;
	ret_val->ct_teardown = NULL;
	ret_val->test_reports_list = ct_init_list();
	ret_val->jump_source_testcase = NULL;
	ret_val->suites_array_index = 0;
	ret_val->run_only_if_tags = initHT();
	ret_val->exclude_tags = initHT();
	ret_val->_crashc_sigaction = (struct sigaction) { 0 };
	ret_val->root_section = initSection(ST_ROOT, 0, "root", "");
	ret_val->statistics = initStatistics();
	ret_val->report_producer_implementation = initDefaultReportProducer();
	ret_val->output_file = stdout;

	return ret_val;
}

void ct_teardown_default_model(ct_model_t* ccm) {
	destroySection(ccm->root_section);
	destroyHTWithElements(ccm->exclude_tags, (ct_destructor_t)destroyTag);
	destroyHTWithElements(ccm->run_only_if_tags, (ct_destructor_t)destroyTag);
	ct_destroy_list_with_elements(ccm->test_reports_list, (ct_destructor_t)ct_destroy_test_report);
	destroyStatistics(ccm->statistics);
	destroyDefaultReportProducer(ccm->report_producer_implementation);
	fclose(ccm->output_file);
	free(ccm);
}
