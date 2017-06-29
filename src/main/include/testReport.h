/*
 * testReport.h
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#ifndef TESTREPORT_H_
#define TESTREPORT_H_

#include <stdbool.h>
#include <stdlib.h>

#include "errors.h"

typedef struct TestReport {
	const char* file;
	int lineNo;
	const char* expr;
	bool outcome;
} TestReport;

/**
 * Creates a new test report
 *
 * @param[in] file the file where the assertion was positioned
 * @param[in] lineNo the line number in \c file where the assertion was positioned
 * @param[in] expr the expression inside the assertion
 * @param[in] outcome true if the assertion was satisfied, false otherwise
 */
TestReport* initTestReport(const char* file, const int lineNo, const char* expr, const bool outcome);
/**
 * Removes from the memory a ::TestReport
 *
 * @param[in] tr the report to remove
 */
void destroyTestReport(TestReport* tr);

#endif /* TESTREPORT_H_ */
