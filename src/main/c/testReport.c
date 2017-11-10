/**
 * This file contains the function used by CrashC to generate and handle the test reports
 * TODO: Fix strdup unsafe call, add dimension limit to avoid overflows
 *
 * Author: Lorenzo Nodari
 *
 */

#include <string.h>

#include "testReport.h"

TestReport * initTestReport(Section * testcase) {
	TestReport * retVal = malloc(sizeof(TestReport));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->filename = NULL;
	retVal->execution_time = 0;
	retVal->outcome = TEST_SUCCESS;
	retVal->testcase_snapshot = NULL;

	return retVal;
}

void destroyTestReport(TestReport * report) {
	free(report->filename);
	destroySnapshotTree(report->testcase_snapshot);
	free(report);

}

SectionSnapshot * initSectionSnapshot(Section * section) {
	SectionSnapshot * retVal = malloc(sizeof(SectionSnapshot));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->description   = strdup(section->description);
	retVal->tags          = section->tags;
	retVal->type          = section->type;
	retVal->status        = SNAPSHOT_OK;
	retVal->elapsed_time  = 0;
	retVal->levelId 	  = section->levelId;
	retVal->parent = NULL;
	retVal->next_sibling = NULL;
	retVal->first_child = NULL;

	return retVal;
}

void destroySnapshotTree(SectionSnapshot * snapshot) {
	free(snapshot->description);

	SectionSnapshot * next_child = snapshot->first_child;
	while (next_child != NULL) {
		SectionSnapshot * tmp = next_child->next_sibling;
		destroySnapshotTree(next_child);
		next_child = tmp;
	}

	free(snapshot);
}

SectionSnapshot * addSnapshotToTree(SectionSnapshot * to_add, SectionSnapshot * tree) {
	to_add->parent = tree;

	SectionSnapshot * tmp = tree->first_child;
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

void updateSnapshotStatus(Section * section, SectionSnapshot * snapshot) {
	if (section->status == SECTION_SIGNAL_DETECTED) {
		snapshot->status = SNAPSHOT_SIGNALED;
	}
}

void updateTestOutcome(TestReport * report, SectionSnapshot * last_snapshot) {
	if (last_snapshot->status != SNAPSHOT_OK) {
		report->testcase_snapshot->status = last_snapshot->status;
		report->outcome = TEST_FAILURE;
	}
}


