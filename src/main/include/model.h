/*
 * \file model.h
 *
 * Represents the model of crashc. Every variable needed by the framework should be inserted here
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "typedefs.h"
#include "section.h"
#include <signal.h>
#include <setjmp.h>

/**
 * The maximum number of registrable suites
 *
 * This macro is used to set the initial size of the internal array used to contain the tests functions pointers
 */
#ifndef MAX_TESTS
#   define MAX_TESTS 256
#endif

typedef struct crashc_model {
	/**
	 * Array containing the pointers to the testsuites functions
	 */
	test_pointer tests_array[MAX_TESTS];
	/**
	 * This variable is used to keep track of the tests_array[] array dimension
	 */
	int suites_array_index;
	/**
	 * field representing the root of the section tree
	 */
	Section* rootSection;
	/**
	 * field representing what is the section we're analyzing right now.
	 *
	 * Suppose we have the tree shown in ::Section. Suppose you're analyzing when2: you need something
	 * that allows you to synchronize what the code is reading and the section tree metadata; aka you need something
	 * that points which section you're actually in. This variable is that pointer.
	 *
	 * When you're in when2 this variable is set to the node in the tree representing when2. When you enter inside the code of "then1",
	 * this variable is reset to point then 1 ::Section. Then, when you return to when2 to execute the code between then1 and then2, this variable
	 * is set again in a way to point "when2".
	 */
	Section* currentSection;
	/**
	 * field representing the test case crashC is handling right now.
	 * Crash C can handle at most 1 test case per time
	 */
	Section* testCaseInvolved;
	/**
	 * Represents the tags the user has specified as the only one that should be consider
	 *
	 * A test is run only if it declares at least one tag inside this container.
	 * If the test is in conflict with ::excludeTags, ::ecludeTags has the precedence.
	 *
	 * If this hashtable is empty, then we consider as if the check does't need to happen
	 */
	tag_ht* runOnlyIfTags;
	/**
	 * Represents the tags the user has specified as the ones that excludes tests
	 *
	 * A test is skipped if it declares at least one tag inside this container.
	 * If the test is in conflict with ::excludeTags, ::ecludeTags has the precedence
	 *
	 * If this hashtable is empty, then we consider as if the check does't need to happen
	 */
	tag_ht* excludeTags;
	/**
	 * This variable is used to store the execution state to be restored thanks to
	 * setjmp and longjmp when needed during faulty test execution.
	 * By faulty test we mean tests which generate SIGSEGV or SIGFPE.
	 */
	jmp_buf signal_jump_point;
	/**
	 * A structure representing the flag intercepted by crashc
	 */
	struct sigaction _crashc_sigaction;
} crashc_model;

/**
 * Create a model correctly initialized
 *
 * @return a setupped model
 */
crashc_model setupDefaultMainModel();

/**
 * Destory every memory allocated by the model
 *
 * @param[in] ccm the model to destroy
 */
void tearDownModel(crashc_model ccm);

#endif /* MODEL_H_ */