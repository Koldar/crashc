/**
 * This file contains the function used by CrashC to generate and handle the test reports
 * TODO: Fix strdup unsafe call, add dimension limit to avoid overflows
 *
 * Author: Lorenzo Nodari
 *
 */

#include "testReport.h"

TestReport * initTestReport(Section * testcase) {
	TestReport * retVal = malloc(sizeof(TestReport));

	retVal->testcase_name = strdup(testcase->description);
	retVal->execution_time = 0;
	retVal->test_sections = initList();

	return retVal;
}

void destroyTestReport(TestReport * report) {
	free(report->testcase_name);
	destroyListWithElement(report->test_sections, destroySection);
	free(report);
}


