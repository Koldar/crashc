/**
 * This file contains the definitions used by CrashC to handle the test reports
 *
 * Author: Lorenzo Nodari
 *
 */

#ifndef TESTREPORT_H_
#define TESTREPORT_H_

#include "section.h"
#include "errors.h"
#include "tag.h"


/**
 * This struct represents the actual test report.
 * It contains all the informations regarding the status of an executed test.
 * Note that CrashC considers a test to be a single flow of execution of a LOOPER (i.e. A single iteration of a testcase)
 * E.g:
 * Let's suppose we have this testcase:
 *
 * TESTCASE("...", "...") {
 *     WHEN("outer when", "") {
 *         WHEN("inner when 1", "") {
 *             ...some code...
 *         }
 *         WHEN("inner when 2", "") {
 *             ...other code...
 *         }
 *     THEN("then", "") {
 *         ...then code...
 *     }
 * }
 *
 * The flow of execution of this testcase would be:
 * 1) inner when 1 -> then
 * 2) inner when 2 -> then
 *
 * These are what CrashC considers to be tests.
 * This implies that the informations stored in the TestReport struct are related to the flow of execution, not to a given section.
 * This is the reason why the TestReport struct needs an auxiliary struct, SectionSnapshot, to hold the information on the statuses of
 * the sections involved in the test at the moment they were executed.
 */
typedef struct {

	/**
	 * The name of the file that contained this test
	 */
	char * filename;

	/**
	 * The snapshot of the testcase that contained this test.
	 * This is actually the root of a tree which contains the section path followed by the test
	 */
	 SectionSnapshot * testcase_snapshot;

	/**
	 * The time that it took to complete the test
	 * Note that execution times might be higher than expected due to the necessary
	 * overhead introduced by the internal code created by CrashC to properly
	 * guide the tests' execution flow
	 */
	long execution_time;

} TestReport;


TestReport * initTestReport(char * filename, struct Section * testcase);
SectionSnapshot * initSectionSnapshot(Section * section);

#endif /* TESTREPORT_H_ */
