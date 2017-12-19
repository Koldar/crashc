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

struct ct_model;
struct Section;
struct ct_test_report;
struct SectionSnapshot;
struct ct_test_statistics_t;
struct ct_report_producer_t;


typedef struct Section Section;
typedef struct SectionSnapshot SectionSnapshot;
typedef struct ct_model ct_model_t;
typedef struct ct_test_report ct_test_report_t;
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
typedef void (*ct_test_reporter_t)(ct_model_t *, ct_test_report_t *);

/**
 * This type defines the function pointer to the function used to produce the report of a snapshot tree
 */
typedef void (*ct_snapshot_tree_reporter_t)(ct_model_t *, SectionSnapshot *, int);

/**
 * This type defines the function pointer to the function used to produce the tests summary.
 * The statistics of the tests, if used by the model, can be reached through it, so they are not passed
 * as an argument explicitly
 */
typedef void (*ct_summary_reporter_t)(ct_model_t *);

/**
 * This type defines the function pointer to the function used to produce the reports for the assertions
 * executed during a specific snapshot. The SectionSnapshot argument is used to get the list of ct_assert_report_t*.
 * The int parameter is used to specify the indentation level to use.
 */
typedef void (*ct_assert_reporter_t)(ct_model_t*, SectionSnapshot*, int);

/**
 * This type defines the function pointer to the function used to create the whole report by calling
 * the above functions. The list of test reports is passed through the model. In the same way it is possible to reach
 * the report_producer implementation.
 */
typedef void (*ct_reporter_t)(ct_model_t *);

#endif /* TYPEDEFS_H_ */
