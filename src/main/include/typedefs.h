/*
 * \file typedefs.h
 *
 * Contains all the general purpose typedefs used in the project
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

struct crashc_model;
struct Section;
struct TestReport;
struct SectionSnapshot;
struct ct_test_statistics_t;
struct ct_report_producer_t;


typedef struct Section Section;
typedef struct SectionSnapshot SectionSnapshot;
typedef struct crashc_model crashc_model;
typedef struct TestReport TestReport;
typedef struct ct_test_statistics_t ct_test_statistics_t;
typedef struct ct_report_producer_t ct_report_producer_t;


/**
 * Represents the type ::Section::levelId has
 */
typedef int SectionLevelId;

/**
 * Function pointer used to point at the functions which contain the testsuites.
 */
typedef void (*test_pointer)(void);

/**
 * This type defines the function pointer to the function used to  produce the report of a single test.
 * This function usually calls a ct_snapshot_tree_reporter.
 */
typedef void (*ct_test_reporter_t)(crashc_model *, TestReport *);

/**
 * This type defines the function pointer to the function used to produce the report of a snapshot tree
 */
typedef void (*ct_snapshot_tree_reporter_t)(crashc_model *, SectionSnapshot *, int);

/**
 * This type defines the function pointer to the function used to produce the tests summary.
 * The statistics of the tests, if used by the model, can be reached through it, so they are not passed
 * as an argument explicitly
 */
typedef void (*ct_summary_reporter_t)(crashc_model *);

/**
 * This type defines the function pointer to the function used to create the whole report by calling
 * the above functions. The list of test reports is passed through the model. In the same way it is possible to reach
 * the report_producer implementation.
 */
typedef void (*ct_reporter_t)(crashc_model *);

#endif /* TYPEDEFS_H_ */
