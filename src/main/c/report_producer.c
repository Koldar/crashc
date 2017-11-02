/*
 * report_producer.c
 *
 *  Created on: Oct 27, 2017
 *      Author: Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#include "report_producer.h"

//TODO: Check for failure, e.g file is not open etc
void ct_fprint_report(FILE * file, TestReport * report) {
	fprintf(file, " ---------- TEST REPORT ---------\n");
	fprintf(file, "File: %s\n\n", report->filename);
	ct_print_snapshot_tree(file, report->testcase_snapshot, 0);
	fprintf(file, "\nOutcome: %s\n", (report->outcome == TEST_SUCCESS) ? "SUCCESS" : "FAILURE");
	fprintf(file, " --------------------------------\n");
	fprintf(file, "\n\n");
}

void ct_stdout_report(TestReport * report) {
	ct_fprint_report(stdout, report);
}

char * ct_snapshot_status_to_string(snapshot_status s) {
	switch (s) {
		case SNAPSHOT_OK: return "OK";
		case SNAPSHOT_SIGNALED: return "SIGNALED";
		case SNAPSHOT_FAILED: return "FAILED";
		default: 	printf("\nERROR: Unrecognized snapshot status, exiting.\n");
					exit(1);
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
					exit(1);
	}
}

void ct_print_snapshot_tree(FILE * file, SectionSnapshot * snapshot, int level) {

	char * type_str = ct_section_type_to_string(snapshot->type);
	char * status_str = ct_snapshot_status_to_string(snapshot->status);
	printf("%s : %s -> %s\n", type_str, snapshot->description, status_str);

	SectionSnapshot * child = snapshot->first_child;
	while (child != NULL) {
		for (int i = 0; i < level; i++) {
			putchar('\t');
		}
		ct_print_snapshot_tree(file, child, level + 1);
		child = child->next_sibling;
	}

}


