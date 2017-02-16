/*
 * testReport.c
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#include "testReport.h"

TestReport* initTestReport(const char* file, const int lineNo, const char* expr, const bool outcome) {
	TestReport* retVal = malloc(sizeof(TestReport));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->file = strdup(file);
	retVal->lineNo = lineNo;
	retVal->expr = strdup(expr);
	retVal->outcome = outcome;
}

void destroyTestReport(TestReport* tr) {
	free(tr->file);
	free(tr->expr);
	free(tr);
}
