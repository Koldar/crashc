/**
 * This file contains the definitions used by CrashC to handle the test reports
 *
 * Author: Lorenzo Nodari
 *
 */

#ifndef TESTREPORT_H_
#define TESTREPORT_H_

#include <string.h>
#include <stdlib.h>

#include "section.h"
#include "errors.h"
#include "tag.h"
#include "typedefs.h"

/**
 * This enumerations represents the possible outcomes of a single test:
 *  - success;
 *  - failure
 */
typedef enum {
	TEST_SUCCESS,
	TEST_FAILURE
} test_outcome;

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
struct TestReport {

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
	  * The outcome of the test. Can be SUCCESS or FAILURE
	  */
	 test_outcome outcome;

	/**
	 * The time that it took to complete the test
	 * Note that execution times might be higher than expected due to the necessary
	 * overhead introduced by the internal code created by CrashC to properly
	 * guide the tests' execution flow
	 */
	long execution_time;

};


TestReport * initTestReport(struct Section * testcase);
void destroyTestReport(TestReport * report);
SectionSnapshot * initSectionSnapshot(Section * section);
void destroySnapshotTree(SectionSnapshot * snapshot);

/**
 * This function adds the given section to the section tree and automatically
 * reorganize the tree's children
 */
SectionSnapshot * addSnapshotToTree(SectionSnapshot * to_add, SectionSnapshot * tree);

/**
 * This functions checks the status of the section associated to a given snapshot
 * in order to change coherently the status of the snapshot. This is needed due to the fact
 * that every snapshot is created when entering at the beginning of the section and by default
 * is set as SNAPSHOT_OK, so it discovers if any error occourred only at the end, by checking the section.
 */
void updateSnapshotStatus(Section * section, SectionSnapshot * snapshot);

/**
 * This functions checks the status of the last executed snapshot in the test tree and sets the
 * status of the test as needed
 */
void updateTestOutcome(TestReport * report, SectionSnapshot * last_snapshot);

#endif /* TESTREPORT_H_ */
