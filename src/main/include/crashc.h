/**
 * @file
 *
 * Represents the main file of the framework
 *
 * The file contains all the main macro definition used to provide crashC features to the developer
 *
 * TEST CASE
 * ---------
 *
 * Test cases are particularly important sections: they serve as entry points for all our tests. They need:
 * 	\li a cycle: if there multiple WHEN clauses, we need to rerun their body until all the when clauses are run. This requires a cycle;
 * 	\li a function definition: test cases are not defined inside the main, so the only alternative is that they are functions. Why they can't be inside the main?
 * 		Because otherwise we would need to have "}" at the end of all the test cases. But with macro programming you can't simply do that (we may tell the user
 * 		to write the "}" by himself, but that would add boilerplate code to the test case, code that we do not want to add.
 *
 * Ok, how can we add a function definition that compiles? Remember: you can't add anything after you put the "{ ... }" of the test case.
 * The idea is simple: the test is a function and the "{ ... }" is its body. However in this way we can't have the cycle, cycle that we need to deal
 * with the when clauses. The solution is to have for every test, <b>2 functions</b>: "{...}" is  the body of the second function whilst the first function
 * is defined before the second one, like this:
 *
 * @code
 *
 * TESTCASE ("tc1", "important") {
 * 	//our beatiful tests
 * }
 *
 * //this becomes
 *
 * void test_1a() {
 * 	LOOPER(...) {
 * 		test_1b();
 * 	}
 * }
 *
 * void test_1b() {
 * 	//our beatiful tests
 * }
 *
 * @endcode
 */

#ifndef CRASHC_H_
#define CRASHC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "errors.h"
#include "hashtable.h"
#include "section.h"
#include "sig_handling.h"
#include "command_line.h"
#include "model.h"
#include "test_report.h"
#include "main_model.h"
#include "report_producer.h"
#include "ct_assert.h"

/**
 * Callback type of a function representing a general condition that determine if we can access to a particular @containablesection source code
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you want to access
 * @return
 * 	\li @true whenever the testing framework decided that the code can enter in the @containablesection source code;
 * 	\li @false otherwise.
 */
typedef bool (*ct_access_callback_t)(ct_model_t* model, struct ct_section*);

/**
 * Callback signature representing a callback to execute as soon as you **exit** from the @containablesection source code
 *
 * For example, suppose the following. Suppose you have exited from \c WHEN. The ct_model_t::current_section variable needs to be
 * reset from \c WHEN to its parent, namely \c TESTCASE. The function prototype allows you to code that
 *
 * @code
 * 	TESTCASE("","") {
 * 		WHEN("","") {
 * 		}
 * 	}
 * @endcode
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[inout] parent_position a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you'have just exit from.
 */
typedef void (*ct_exit_callback_t)(ct_model_t* model, struct ct_section** parent_position, struct ct_section* child);

/**
 * Callback prototype for any function executing addtional code before entering in a @containablesection
 *
 * For example, asume you are in \c TESTCASE and assume the framework gave you access to \c WHEN @containablesection.
 * Before actually enter in the section, you might want to perform additional code. The prototype has been specified for this very reason
 * @code
 * 	TESTCASE("","") {
 * 		WHEN("","") {
 * 		}
 * 	}
 * @endcode
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access (hence the child).
 */
typedef void (*ct_enter_callback_t)(ct_model_t* model, struct ct_section* section);

/**
 * Register a new @testsuite in the @crashc ecosystem
 *
 * C language doesn't have anything allowing you to fetch all defined functions within a file or a group of file (something like
 * Java's annotation or Python introspection). So, in order to detect all the suite the tests defined, one need to manually
 * register them in a data structure (like an array).
 *
 * This function goal is precisely this: it allows you to register a @testsuite within @crashc framework.
 *
 * TODO: Add control on duplicates testsuites
 *
 * \post
 * 	\li \c func added in the register
 *
 * @param[inout] model the model where we operate on
 * @param[in] func the function to register
 */
void ct_update_test_array(ct_model_t* model, test_pointer func);

/**
 * Function to concretely perform the **access cycle**
 *
 * @param[inout] model the model containing all the data representing the automatic testing
 * @param[in] section the section we want to access in
 * @param[in] cs the condition we need to satisfy in order to access the section
 * @param[in] callback the code to execute if the system grant us access to the section. Note that in this way the code is called \b before entering in the section
 * @param[in] run_tags an hash table containing all the tags allowed. If a section does not have a tag inside this set, it won't be run
 * @param[in] exclude_tags an hash table containing all the tags prohibited. If a section has at least one tag inside this set, it won't be run
 * @return
 * 	\li @true if the section has to be visited;
 * 	\li @false otherwise;
 */
bool ct_run_once_check_access(ct_model_t* model, struct ct_section* section, ct_access_callback_t cs, ct_enter_callback_t callback, const ct_tag_hashtable_o* restrict run_tags, const ct_tag_hashtable_o* restrict exclude_tags);
/**
 * Function supposed to run in the **parent switcher** cycle
 *
 * @param[inout] model the model containing all the data representing the automatic testing
 * @param[in] section the section we want to fetch the associated parent from
 * @param[in] pointer_to_set_as_parent a pointer that we has to set to <tt>section->parent</tt> in this call. <b>This has to be done by exactly one of the callbacks</b>
 * @param[in] callback function **always called after we have check the access** to the section: if we have access, this function is called **after** we entered inside the section source code
 * @param[in] access_granted_callback function called \b afrer \c callback if the software has entered in the section source code
 * @param[in] access_denied_callback function called \b afrer \c callback if the software hasn't entered in the section source code
 * @return
 *	\li @true if this is the first iteration of the **parent switcher** cycle.
 *	\li @false if this si the second iteration of the  **parent switcher cycle.
 */
bool ct_run_once_final_work(ct_model_t* model, struct ct_section* section, struct ct_section** pointer_to_set_as_parent, ct_exit_callback_t callback, ct_exit_callback_t access_granted_callback, ct_exit_callback_t access_denied_callback);

/**
 * Fetches the section representing a particular @containablesection. If not present, it creates it.
 *
 * @code
 * 	TESTCASE("","") {
 * 		WHEN("a","") {
 * 		}
 * 		WHEN("b","") {
 * 		}
 * 	}
 * @endcode
 * For example, this function creates the section representing the @containablesection @when "a" if there is no section inside the section tree
 * of @testcase representing it; otherwise, it just fetches such struct ct_section.
 *
 * \post
 * 	\li a ::ct_section representing the given @containablesection now exists within the section tree.
 *
 * @param[in] parent the section containing the one we're creating. For example if we're in the test code of @testcase and we see a @when clause
 * 				this attribute is set to the metadata representing @testcase.
 * @param[in] type the kind of section to fetch
 * @param[in] description a brief string explaining what this section is and does
 * @param[in] tags a list of tags. See \ref tags for further information
 * @return
 * 	\li a newly created section if we're still computing the children of \c parent
 * 	\li the struct ct_section::current_child -th child of \c parent otherwise
 */
struct ct_section* ct_fetch_section(struct ct_section* parent, enum ct_section_type type, const char* description, const char* tags);

/**
 * Reset the ct_model_t::current_section global variable to the given one after we have detected a signal
 *
 * Sometimes it happens that we need to abruptly break the flow of ct_model_t::current_section.
 * For example when we detect an unhandled signal in one of the sections, we don't need to return to the parent of the section involved,
 * but we need to immediately go to the @testcase.
 *
 * This function allows you to set all the metadata to ensure that such "unorthodox" flow is valid.
 *
 * \post
 * 	\li ct_model_t::current_section is valid and refers to \c testcase_section
 *
 * @param[inout] model the model containing all the data representing the automatic testing
 * @param[in] jump_source_section the section where a signal has been raised
 * @param[in] testcase_section the section representing a test case we need to reset the ct_model_t:current_section to
 */
void ct_reset_section_after_jump(ct_model_t* model, struct ct_section* const jump_source_section, struct ct_section* const testcase_section);

///@defgroup accessConditions Access Condition Functions
///@brief Functions that can be used as ::ct_access_callback_t concrete values.
///Use these functions to develop new @containablesection. These group of functions defines the pool of ::ct_access_callback_t you can use
///to create new @containablesection s.
///@{


/**
 * We give access to a @when only if it's the first @when we're giving access to in the current level of the section tree
 *
 * We use this function to check whether or not we need to enter in the given WHEN section.
 * You gain the access to a @when if:
 *
 * \li the @when has not been fully visited;
 * \li given the parent @containablesection, it's the first @when you can access to;
 *
 * To clarify, consider the following situation:
 *
 * @dotfile accessWhen
 *
 * -# At first you enter in @when 1, then you enter in @when 1,1. After that you avoid both @when 1,2 and @when 1,3 because in the current
 * @testcase cycle, you've already accessed to a @when within the @containablesection called @when 1. The same can be said for @when 2:
 * in the current @testcase cycle, you've already accessed to a @when within the @containablesection called @testcase.
 * -# In the second @testcase cycle, you still enter in @when 1 but, since @when 1,1 has already been visited, you gain the access to @when 1,2.
 * Again, you avoid entering in @when 1,3 since you've already gained access to a @when in the same level (namely @when 1,2). @when 2 follows the
 * same principle, since you've entered in @when 1.
 * -# In the third loop, you gain access, as per usual, to @when 1 (since it's not fully visited yet). Within it, you access to @when 1,3. After it, @when 1
 * is finally fully visited. Again, you don't access to @when 2 since you've visited @when 1 in this @testcase cycle loop.
 * -# Finally you avoid entering in @when 1 since it's fully visited but you access to @when 2.
 *
 * @param[in] model the model involved
 * @param[in] section the section we're trying to access
 * @return
 * 	\li \true if we can access to section \c section;
 * 	\li \false otherwise
 */
bool ct_get_access_when(ct_model_t* model, struct ct_section* section);

/**
 * Grants always access
 *
 * This condition allows you to **always** gain the access to the @containablesection
 *
 * @param[in] model the model involved
 * @param[in] section the section we're trying to access
 * @return
 * 	\li true if we can access to section \c section;
 * 	\li false otherwise
 */
bool ct_always_enter(ct_model_t* model, struct ct_section* section);


///@}

///@defgroup beforeEntering Before Entering Containable struct ct_section Functions
///@brief Functions that can be used as ::ct_enter_callback_t concrete values.
///Use these functions to develop new @containablesection. These group of functions defines the pool of ::ct_enter_callback_t you can use
///to create new @containablesection s.
///@{

/**
 * Perform setup operations before entering in a @when
 *
 * This callback should be executed when we get access granted to a WHEN section, before executing its code.
 * It sets ::ct_section::already_found_when of ct_model_t::current_section and takes a snapshot of the WHEN section for the test report
 * and adds it to the test report snapshots tree.
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access.
 */
void ct_callback_entering_when(ct_model_t* model, struct ct_section* section);

/**
 * Perform setup operations before entering in a @then
 *
 * This callback should be executed when we get access granted to a THEN section, before executing its code.
 * It takes a snapshot of the section and adds it to the current_section's test report snapshots tree.
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access.
 */
void ct_callback_entering_then(ct_model_t* model, struct ct_section* section);

///@}

/**
 * Updates the current_snapshot, creating a new one if we started a new test
 * or adding a snapshot to the current test snapshots tree
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access.
 */
void ct_update_current_snapshot(ct_model_t* model, struct ct_section* section);

///@defgroup afterEntering After Entering Containable struct ct_section Functions
///@brief Callbacks that can be used in ::ct_run_once_final_work as callbacks. Set of candidate callbacks to be called after a struct ct_section **access cycle**, **regardless** of its outcome.
///These callbacks have the main task to repair ct_model_t::current_section in order to ensure that the @containablesection
///we're actually in when the code is executing is the same pointed by the section tree.
///Furthermore, these callbacks fetches new data useful for other @crashc task (i.e. test reports).
///@{

/**
 * updates ct_model_t::current_section and increases the children of the parent by 1
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_next_sibling(ct_model_t* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);
/**
 * Update section tree metadata whenever you have successfully entered in a child @containablesection
 *
 * If you have just exited from a @containablesection, this means you know the number of **direct** children it contains.
 * This function then performs the following operations:
 *  \li Mark the \c section as "now we know the exact number of children you have";
 *  \li Updates ct_model_t::current_snapshot;
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_children_number_computed(ct_model_t* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);

//TODO find a new name. It's quite similar to ct_exit_callback_children_number_computed, so maybe we should have 2 similar names?
/**
 * like ::ct_exit_callback_children_number_computed but it doesn't update the ct_model_t::current_snapshot
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_reset_container(ct_model_t* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);

/**
 * Do nothing
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_do_nothing(ct_model_t* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);

/**
 * Finalize operations when a @testcase is finished.
 *
 * it simply updates the test report outcome depending on the status of the last snapshot of the test tree
 * and resets the ct_model_t::current_snapshot pointer back to NULL to indicate that the current test is over
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] section the section representing the @containablesection you'have just exit from.
 */
void ct_exit_callback_access_granted_testcase(ct_model_t* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section);

///@}

///\defgroup accessGrantedCallBack Access Granted Callbacks
///@brief Callbacks that can be used in ::ct_run_once_check_access as \c access_granted_callBack. These callbacks are useful to
///create new @containablesection implementations
///@{

/**
 * Does nothing
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access (hence the child).
 */
void ct_callback_do_nothing(ct_model_t* model, struct ct_section* section);

/**
 * Setup function for @testcase sections
 *
 * It initializes the test report associated to the particular looper iteration,
 * adds it to the test reports list and takes the snapshot of the testcase.
 *
 * @param[inout] model the global ct_model_t crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access (hence the child).
 */
void ct_callback_entering_testcase(ct_model_t* model, struct ct_section* section);

///@}

/**
 * Main macro of CrashC
 *
 * @definition
 * A containable section is defined as a piece of developer code you might want to test. To delimit it, containable section
 * are often wrap in curly brackets. Containable sections may be hierarchical organized: that is, one containable section X may contain
 * multiple "children" containable sections, which have one and only parent containable section, namely X.
 * Access to containable section may be granted depending on **specific conditions**.
 * The main goal of containable sections is to forcefully change the flow of the developer code,
 * regardless on how the code may appear from the outside. For example, code in \c WHEN sections seems to be executable just after the code
 * shown before it. Actually code in \c WHEN section is executed depending on a much more complex condition.
 * Concretely a @containablesection is 2 nested for...loop cycle that, at the end, are just 2 masked \c if conditions.
 *
 *
 * @definition
 * ::CONTAINABLESECTION is 2 nested for...loop cycles. The outermost is the **parent switcher**, allowing you to keep synchronized the
 * @containablesection the code is actually in with the metadata representing @containablesection, namely the tree formed by ::ct_section.
 * This syncronization is a two-pass system: first it change the ct_model_t::current_section from the parent @containablesection to the child one,
 * **regardless** if you have the access to the child. After fully execute the **access cycle**, it repairs ct_model_t::current_section by updating
 * it from the child to the parent. Code-wise, just like ::ct_run_once_check_access, we model the if with a for loop. Since the for loop condition is run twice, we need to ensure the
 * cycle condition is in the first call @true whilst in the second one @false (this ensure the "if" behaviour"). Moreover we want to perform additional
 * code. Such operation is done by ::ct_run_once_final_work.
 *
 *
 * @definition
 * ::CONTAINABLESECTION is composed by 2 loops: the most inner one is the **access cycle** that checks if we can **actually enter** in the @containablesection
 * represented by a  struct ct_section data structure. If the condition is not surpassed, the access cycle immediately release the control to the **parent switcher**. Otherwise
 * the @containablesection source code is executed. The for is implementing an \c if condition with the added feature to execute call code at the end. This function has to be called inside
 * the condition check of the for loop and ensures the loop behaves like an \c if: this is necessary because for loops condition has to be checked twice: one
 * to check if we can access to the section and one (if we have entered inside the loop) to exit from the loop itself
 *
 * @param[inout] model variable of type pointer of ct_model_t containing all the data to manage
 * @param[in] parent the ::ct_section this @containablesection is contained
 * @param[in] section_type (whose type is ::ct_section_type) the type of the ::ct_section representing this @containablesection
 * @param[in] description (whose type is <tt>char*</tt>) a brief description of this @containablesection
 * @param[in] tags a value (whose type is <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 * @param[in] condition the condition (whose type is ::ct_access_callback_t)you need to clear in order to gain access to the internal test code of the @containablesection
 * @param[in] access_granted_callback a callback (whose type is ::ct_enter_callback_t) representing a set of instructions to execute if you gain access to the internal test code. This function will be execute before executing the actual test code
 * @param[in] back_to_parent_callback a callback (whose type is ::ct_exit_callback_t) executed when you're surpassing this @containablesection. Note that this function will be called regardless if you actually enter inside the code or not.
 * @param[in] exit_access_granted_callback a callback (whose type is ::ct_exit_callback_t)to execute if you entered inside the test code of the @containablesection. Called after \c back_to_parent_callback;
 * @param[in] exit_access_denied_callback a callback (whose type is ::ct_exit_callback_t)to execute if you didnt' enter inside the test code of the @containablesection. Called after \c back_to_parent_callback;
 * @param[in] setup_code a piece of C code to paste in the source code before starting all the reasoning for the @containablesection. ct_model_t::current_section for this @containablesection is already populated though.
 */
#define CONTAINABLESECTION(model, parent, section_type, description, tags, condition, access_granted_callback, back_to_parent_callback, exit_access_granted_callback, exit_access_denied_callback, setup_code)	\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																																								\
		(model)->current_section = ct_fetch_section(parent, section_type, description, tags);																			\
		(model)->current_section->times_encountered += 1;																															\
		setup_code																																						\
		for (																																							\
				(model)->current_section->loop1 = true																													\
				;																																						\
				ct_run_once_final_work((model), (model)->current_section, &((model)->current_section), 																	\
						back_to_parent_callback, exit_access_granted_callback, exit_access_denied_callback																\
				)																																						\
				;																																						\
				/**
				 *  This code is execute when we have already executed the code
				 *  inside the container. We assume every post condition of
				 *  CONTAINABLESECTION is satisfied for its children
				 *  CONTAINABLESECTION. Here current_section has not been repaired yet!
				 */																																						\
				 (model)->current_section->loop1 = false																												\
		)																																								\
		for (																																							\
				(model)->current_section->loop2 = true																													\
				;																																						\
				ct_run_once_check_access((model), (model)->current_section, condition, access_granted_callback, (model)->run_only_if_tags, (model)->exclude_tags)		\
				;																																						\
				(model)->current_section->loop2 = false,																												\
				ct_section_set_executed((model)->current_section)																											\
		)

/**
 * Convenience macro for a NOOP
 */
#define NOCODE

/**
 * A @containablesection which embeds a loop.
 *
 * The loop keeps going until the @containablesection is fully visited. This means a ::LOOPER does not
 * stop until every single @containablesection within it has been correctly scanned.
 *
 * @param[inout] model a variable of type ::ct_model containing all the data needed by crashc
 * @param[in] parent a variable of type ::ct_section representing the parent section of this @containablesection
 * @param[in] section_type a value of type ::ct_section_type representing the type of this @containablesection
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#define LOOPER(model, parent, section_type, description, tags)																										\
		CONTAINABLESECTION(																																			\
				(model),																																			\
				parent, section_type, description, tags,																												\
				ct_always_enter, ct_callback_entering_testcase, 																											\
				ct_exit_callback_reset_container, ct_exit_callback_access_granted_testcase,  ct_exit_callback_do_nothing, 												\
																																									\
				(model)->jump_source_testcase = (model)->current_section;																							\
				bool UV(jump_occurred) = false;																														\
				if (sigsetjmp((model)->jump_point, 1)) {                                                                          					        		\
					/* We have caught a SIGNAL or an ASSERTION HAS FAILED: here current_section is the section where the signal was raised							\
					 * If the code enters in this if remember that current_section still refer to the containablesection where the signal has happended				\
					 * or the assertion have failed. If this is the case, you need to reset current_section to the containable section of the test case.			\
					 * */																																			\
					UV(jump_occurred) = true; 																														\
					/*we reset the current_section to the test case*/																								\
					ct_reset_section_after_jump((model), (model)->current_section, (model)->jump_source_testcase);													\
				}																																					\
				for (    																																			\
						;																																			\
						!UV(jump_occurred) && ct_section_still_needs_execution((model)->current_section)                                                   				\
						;																																			\
				)																																					\
		)


//TODO the parent of the test case is not the root section, but the "suite section". We need to include them in the section tree as well!
/**
 * Represents a @containablesection which keeps being executed until every direct and undirect @containablesection within it has been executed
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#define TESTCASE(description, tags) LOOPER(ct_model, ((ct_model)->root_section), CT_TESTCASE_SECTION, description, tags)
/**
 * like ::TESTCASE but with the default \c tags value of ""
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 */
#define EZ_TESTCASE(description) TESTCASE(description, "")

/**
 * A @containablesection where you always gain access to
 *
 * @param[inout] model variable of type pointer of ct_model_t containing all the data to manage
 * @param[in] section_type (whose type is ::ct_section_type) the type of the ::ct_section representing this @containablesection
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#define ALWAYS_ENTER(model, section_type, description, tags) CONTAINABLESECTION(															\
		(model), 																														\
		(model)->current_section, section_type, description, tags,																		\
		ct_always_enter, ct_callback_entering_then,																						\
		ct_exit_callback_next_sibling,	ct_exit_callback_children_number_computed, ct_exit_callback_do_nothing,							\
		NOCODE																															\
)

/**
 * A @containablesection where you gain access to only if has
 * not been fully visited and you didn't access to a similar section yet in the same parent.
 *
 * @param[inout] model variable of type pointer of ct_model_t containing all the data to manage
 * @param[in] section_type (whose type is ::ct_section_type) the type of the ::ct_section representing this @containablesection
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#define ENTER_ONCE_PER_LOOP(model, section_type, description, tags) CONTAINABLESECTION(													\
		(model), 																														\
		(model)->current_section, section_type, description, tags,																		\
		ct_get_access_when, ct_callback_entering_when, 																					\
		ct_exit_callback_next_sibling, ct_exit_callback_children_number_computed, ct_exit_callback_do_nothing,								\
		NOCODE 																															\
)

/**
 * Represents a @containablesection with 2 access conditions:
 *
 * \li it has not been fully visited yet;
 * \li you still have not accessed to a @when in the same container
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#define WHEN(description, tags) ENTER_ONCE_PER_LOOP((ct_model), CT_WHEN_SECTION, description, tags)
/**
 * like ::WHEN but with the default \c tags value of ""
 */
#define EZ_WHEN(description) WHEN(description, "")
/**
 * Represents a @containablesection you need always to access to
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#define THEN(description, tags) ALWAYS_ENTER((ct_model), CT_THEN_SECTION, description, tags)
/**
 * like ::THEN but with the default \c tags value of ""
 */
#define EZ_THEN(description) THEN(description, "")

//TODO all those functions should be included in the only one global models
/**
 * Represents the default entry point for @crashc main executable
 *
 * The macro is used to contain all test declarations and to generate the main function for the
 * execution of the various tests.
 *
 * The macro is actually masking a \c main function
 *
 */
#define TESTS_START int main(const int argc, char* const args[]) { 																\
		ct_model = ct_setup_default_model();																					\
		ct_parse_args(argc, args, CT_TAGS_SEPARATOR, (ct_model)->run_only_if_tags, (ct_model)->exclude_tags); 		\
		ct_register_signal_handlers();

///@defgroup hooks CrashC developer hooks
///@brief Represents a list of utility APIs you can use to fully customize @crashc testing execution
///@{

/**
 * Used to register a teardown function
 *
 * Use this function to perform additional operations **just before** ct_model_t model is fully disposed.
 * Usually you can use this function to release from memory any additional resources you've created.
 *
 * @param[in] f a function of type <tt>void (*f)(void)</tt> to be executed before tearing down the whole model
 * @see ct_model_t::ct_teardown
 */
void ct_set_crashc_teardown(ct_teardown_t f);

///@}

/**
 * Complete the \c main function created with ::TESTS_START
 *
 * Furthermore, it also start the execution of the registered testsuites
 */
#define TESTS_END 																	\
    for (int i = 0; i < (ct_model)->suites_array_index; i++) { 						\
    	(ct_model)->tests_array[i](); 												\
    } 																				\
	(ct_model)->report_producer_implementation->report_producer(ct_model);			\
	if ((ct_model)->ct_teardown != NULL) {											\
		(ct_model)->ct_teardown();													\
	}																				\
	ct_teardown_default_model(ct_model);											\
} //main function closing bracket

/**
 * Specifies a function prototype representing a test suite.
 *
 * @testsuite can contain an arbitrary number
 * of testcases. Every ::TESTSUITE must be given a unique ID, namely either a valid C identifier
 * or a non negative integer number. Such ID uniquely represents a @testsuite.
 * Progressive numerical IDs are strongly recomended as the framework provides a few
 * really useful macros to easily register their testsuites.
 * Note that ID coherency is left as a user responsability.
 *
 * @param[in] id a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
#define TESTSUITE(id) void suite_ ## id()

/**
 * Macro registering a single test suite given its ID
 *
 * \pre
 * 	\li test suite generated with ::TESTSUITE macro with the same \c id needs to have been declared
 * \post
 * 	\li the @testsuite with id set to \c id has been register in @crashc framework
 *
 * @param[in] id a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
 #define REGISTER_SUITE(id) 								\
     void suite_ ## id(); 								\
     ct_update_test_array((ct_model), suite_ ## id)

/**
 * Alias of ::REGISTER_SUITE
 *
 * @param[in] id a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
#define REG_SUITE(id) REGISTER_SUITE(id)

/**
 * Register a batch of test suites all in one
 *
 * This macro accepts a variadic arguments. Each argument represents the \c id of a @testsuite to register.
 *
 * \pre
 * 	\li every \c id within the variadic argument needs to have a corresponding ::TESTSUITE call with the same \c id
 *
 * Note that for a single use of the macro the maximum number of specified IDs is 64
 * due to CPP limitations. This problem is easily solved by the use of two or more
 * consequent macro uses.
 *
 * @param[in] ... a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
#define REGTESTS(...) VARIADIC_MACRO(REGISTER_SUITE, ## __VA_ARGS__)


#endif
