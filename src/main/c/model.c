/*
 * model.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#include <stdlib.h>

#include "test_report.h"
#include "report_producer.h"
#include "errors.h"
#include "model.h"

struct ct_model* ct_setup_default_model() {
	struct ct_model* ret_val = malloc(sizeof(struct ct_model));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->current_section = NULL;
	ret_val->current_snapshot = NULL;
	ret_val->ct_teardown = NULL;
	ret_val->test_reports_list = ct_list_init();
	ret_val->jump_source_testcase = NULL;
	ret_val->suites_array_index = 0;
	ret_val->run_only_if_tags = ct_ht_init();
	ret_val->exclude_tags = ct_ht_init();
	ret_val->_crashc_sigaction = (struct sigaction) { 0 };
	ret_val->root_section = ct_section_init(CT_ROOT_SECTION, "root", "");
	ret_val->statistics = ct_init_stats();
	ret_val->report_producer_implementation = ct_init_default_report_producer();
	ret_val->output_file = stdout;

	return ret_val;
}

void ct_teardown_default_model(struct ct_model* ccm) {
	ct_section_destroy(ccm->root_section);
	ct_ht_destroy_with_elements(ccm->exclude_tags, (ct_destroyer_c)ct_tag_destroy);
	ct_ht_destroy_with_elements(ccm->run_only_if_tags, (ct_destroyer_c)ct_tag_destroy);
	ct_list_destroy_with_elements(ccm->test_reports_list, (ct_destroyer_c)ct_destroy_test_report);
	ct_destroy_stats(ccm->statistics);
	ct_destroy_default_report_producer(ccm->report_producer_implementation);
	fclose(ccm->output_file);
	free(ccm);
}
