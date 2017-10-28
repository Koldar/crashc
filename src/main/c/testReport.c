/*
 * testReport.c
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#ifndef ALL_IN_ONE
	#include <string.h>
#endif

#include "testReport.h"
#include "errors.h"

typedef struct TestReport {
	const char* file;
	int lineNo;
	const char* expr;
	/**
	 * True if the particular test has passed; false otherwise
	 */
	bool outcome;
};

TestReport* initTestReport(const char* file, const int lineNo, const char* expr, const bool outcome) {
	TestReport* retVal = malloc(sizeof(TestReport));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->file = strdup(file);
	retVal->lineNo = lineNo;
	retVal->expr = strdup(expr);
	retVal->outcome = outcome;

	return retVal;
}

void destroyTestReport(TestReport* tr) {
	free(tr->file);
	free(tr->expr);
	free(tr);
}

bool getOutcomeFromTestReport(const TestReport* tr) {
	return tr->outcome;
}

const char* getFileTestedOfTestReport(const TestReport* tr) {
	return tr->file;
}

int getLineNoOfTestReport(const TestReport* tr) {
	return tr->lineNo;
}

const char* getExprTestedOfTestReport(const TestReport* tr) {
	return tr->expr;
}
