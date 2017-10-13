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
 * This structure defines the possible statuses in which a snapshot can be.
 * This indicates the "exit status" of the section in the particular test
 */
typedef enum {
	SNAPSHOT_EXEC,
	SNAPSHOT_SKIPPED,
	SNAPSHOT_SIGNALED,
	SNAPSHOT_FAILED
} snapshot_status;

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

/**
 * This structure contains the informations contained in a Section at a precise moment of the program execution,
 * which are then used by the test reports to yield the user a more detailed set of informations about a given test.
 * Note that this structure contains only the fields of a Section which are subject to changes during the execution
 * of the tests. Note that this structure contains an implicit tree, used by CrashC to track the relationships between the
 * sections involved in a precise test
 */
typedef struct {
	/**
	 * The description of this section
	 */
	char * description;

	/**
	 * The tags associated to this section
	 */
	tag_ht *  tags;

	/**
	 * The type of this section
	 */
	section_type type;

	/**
	 * The status of the snapshot.
	 * This actually pretty much just mimic the status the section to which this snapshot
	 * refers to, but it is a bit different as some section statuses makes no sense in the
	 * context of the snapshot, e.g SECTION_UNEXEC, SECTION_EXEC, etc..
	 */
	snapshot_status status;

	/**
	 * The amount of time the section ran during one specific test.
	 * This field has no meaning until the section is complitely executed.
	 */
	struct timespec elapsed_time;

	/**
	 * The pointer to the parent of this snapshot in the snapshot tree
	 */
	SectionSnapshot * parent;

	/**
	 * The pointer to the first child of this snapshot in the snapshot tree
	 */
	SectionSnapshot * firstChild;

	/**
	 * The pointer to the next sibling of this snapshot in the snapshot tree
	 */
	SectionSnapshot * nextSibling;

} SectionSnapshot;

TestReport * initTestReport(char * filename, Section * testcase);

#endif /* TESTREPORT_H_ */
