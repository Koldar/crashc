/**
 * This file contains the function used by CrashC to generate and handle the test reports
 * TODO: Fix strdup unsafe call, add dimension limit to avoid overflows
 *
 * Author: Lorenzo Nodari
 *
 */

#include "testReport.h"

TestReport * initTestReport(char * filename, Section * testcase) {
	TestReport * retVal = malloc(sizeof(TestReport));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->filename = strdup(filename);
	retVal->execution_time = 0;
	retVal->testcase_snapshot = takeSectionSnapshot(testcase);

	return retVal;
}

SectionSnapshot * initSectionSnapshot(Section * section) {
	SectionSnapshot * retVal = malloc(sizeof(SectionSnapshot));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->description  = strdup(section->description);
	retVal->tags         = section->tags;
	retVal->type         = section->type;
	retVal->status       = SNAPSHOT_EXEC;
	retVal->elapsed_time = 0;
	retVal->parent       = NULL;
	retVal->firstChild   = NULL;
	retVal->nextSibling  = NULL;

}


