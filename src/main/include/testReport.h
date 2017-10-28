/*
 * testReport.h
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#ifndef TESTREPORT_H_
#define TESTREPORT_H_

#ifndef ALL_IN_ONE
	#include <stdbool.h>
	#include <stdlib.h>
#endif
#include "list.h"
#include "errors.h"

typedef struct TestReport TestReport;
typedef list TestReportList;

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

/**
 * @param[in] tr the report to analyze
 * @return
 * 	\li true if the test cited by \c tr has passed;
 * 	\li false otherwise
 */
bool getOutcomeFromTestReport(const TestReport* tr);

/**
 * @param[in] tr the report to analyze
 * @return the name of the file where the test this report model was located;
 */
const char* getFileTestedOfTestReport(const TestReport* tr);

/**
 * @param[in] the tst report to consider
 * @return the line number of the test \tr models
 */
int getLineNoOfTestReport(const TestReport* tr);

/**
 * @param[in] tr the test report to consider
 * @return the assertion code of the test \c tr models
 */
const char* getExprTestedOfTestReport(const TestReport* tr);

#endif /* TESTREPORT_H_ */
