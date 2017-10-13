/**
 * This file contains the definitions used by CrashC to handle the test reports
 *
 * Author: Lorenzo Nodari
 *
 */

#ifndef TESTREPORT_H_
#define TESTREPORT_H_

#include <time.h>
#include "list.h"
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
	 * The description of the testcase that contained this test
	 */
	char * testcase_desc;

	/**
	 * The list of the SectionSnapshots which composed this test
	 */
	list * test_sections;

	/**
	 * The time that it took to complete the test
	 * Note that execution times might be higher than expected due to the necessary
	 * overhead introduced by the internal code created by CrashC to properly
	 * guide the tests' execution flow
	 */
	clock_t execution_time;

} TestReport;

/**
 * This structure contains the informations contained in a Section at a precise moment of the program execution,
 * which are then used by the test reports to yield the user a more detailed set of informations about a given test.
 * Note that this structure contains only the fields of a Section which are subject to changes during the execution
 * of the tests.
 */
typedef struct {
	char * description;
	tag_ht *  tags;
	section_type type;
} SectionSnapshot;

TestReport * initTestReport(Section * testcase);
void destroyTestReport(TestReport * report);

#endif /* TESTREPORT_H_ */
