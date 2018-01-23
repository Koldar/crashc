/**
 * @file
 *
 * Contains all the general purpose typedefs used in @crashc
 *
 * @author koldar
 * @date Oct 11, 2017
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

struct ct_model;
struct ct_section;
struct ct_test_report;
struct ct_snapshot;
struct ct_test_statistics_t;
struct ct_report_producer_t;

typedef struct ct_model ct_model_t;
typedef struct ct_test_report ct_test_report_t;
typedef struct ct_test_statistics_t ct_test_statistics_t;
typedef struct ct_report_producer_t ct_report_producer_t;

/**
 * Represents the signature of a function which release a structure from the memory
 *
 * @param[inout] obj the structure to release from the memory. See <a ref="http://yarchive.net/comp/const.html">Linus thread</a>
 * for the rationale behind *const*.
 */
typedef void (*ct_destructor_t)(const void* obj);

/**
 * Function signature used to point at the functions which represent @testsuite
 */
typedef void (*test_pointer)(void);

/**
 * Function signature for a teardown function parameter
 *
 * The type of the function pointer of the global teardown function
 *
 * @see ct_model::ct_teardown
 */
typedef void (*ct_teardown_t)(void);

/**
 * @addtogroup reportFunctionType Report Function Types
 * @brief function types used in ::ct_report_producer_t in order to build a test report human readable
 * @{
 */

/**
 * This type defines the function pointer to the function used to produce the report of a single test.
 *
 * This function usually calls a ct_snapshot_tree_reporter.
 *
 * @param[inout] model the model under analysis
 * @param[in] test_report the test report we need to create a human readable representation of
 */
typedef void (*ct_test_reporter_t)(ct_model_t * model, ct_test_report_t * test_report);

/**
 * This type defines the function pointer to the function used to produce the report of a snapshot tree
 *
 * @param[inout] model the model under analysis
 * @param[in] snapshot the ::ct_snapshot we need to create a human readable representation of
 * @param[in] level the depth (in the snapshot tree) of the \c snapshot
 */
typedef void (*ct_snapshot_tree_reporter_t)(ct_model_t * model, struct ct_snapshot * snapshot, int level);

/**
 * This type defines the function pointer to the function used to produce the tests summary.
 *
 * The statistics of the tests, if used by the model, can be reached through it, so they are not passed
 * as an argument explicitly
 *
 * @param[inout] model the model to use to create the human readable report of the statistics
 */
typedef void (*ct_summary_reporter_t)(ct_model_t * model);

/**
 * This type defines the function pointer to the function used to produce the reports for the assertions executed during a specific snapshot.
 *
 * @param[inout] model the model under analysis
 * @param[in] snapshot the ::ct_snapshot containing the relevant asseritons. This argument **can be used** to get the list of ct_assert_report_t.
 * @param[in] level the depth (in the snapshot tree) of the \c snapshot
 */
typedef void (*ct_assert_reporter_t)(ct_model_t* model, struct ct_snapshot* snapshot, int level);

/**
 * function pointer type used to create the whole report by calling the other \ref reportFunctionType.
 *
 * The list of test reports is passed through the model. In the same way it is possible to reach
 * the report_producer implementation.
 *
 * @param[inout] model the model to consider
 */
typedef void (*ct_reporter_t)(ct_model_t * model);

///@}

#endif /* TYPEDEFS_H_ */
