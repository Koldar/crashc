/**
 * \file crashC.h
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
#include "sigHandling.h"
#include "command_line.h"
#include "model.h"
#include "testReport.h"
#include "main_model.h"
#include "report_producer.h"

/**
 * Callback representing a general condition that determine if we can access to a particular section
 *
 * the parameter is the section to check whilst the return value is
 * true if the software grant us access to the section, false otherwise
 */
typedef bool (*condition_section)(crashc_model * model, Section*);
typedef void (*AfterExecutedSectionCallBack)(crashc_model * model, Section** parentPosition, Section* child);
typedef void (*BeforeStartingSectionCallBack)(crashc_model * model, Section* sectionGranted);

/**
 * This function registers a testsuite by storing its function pointer into
 * the global array. The function automatically updates the variable used to
 * keep track of the array dimension.
 * TODO: Add control on duplicates testsuites
 *
 * \post
 * 	\li \c func added in the index
 *
 * @param[in] func the function to register
 * @param[inout] model the model where we operate on
 */
void update_test_array(crashc_model * model, test_pointer func);

/**
 * Function to run in the access cycle
 *
 * ::CONTAINABLESECTION is composed by 2 loops: the most inner one is the <b>access cycle</b> that checks if we can actually enter in the section
 * or not. The for is implementing an "if" condition with the added feature to execute call code at the end. This function has to be called inside
 * the condition check of the for loop and ensures the loop behaves like an if: this is necessary because for loops condition has to be checked twice: one
 * to check if we can access to the section and one (if we have entered inside the loop) to exit from the loop itself
 *
 * @param[in] section the section we want to access in
 * @param[in] cs the condition we need to satisfy in order to access the section
 * @param[in] callback the code to execute if the system grant us access to the section. Note that in this way the code is called \b before entering in the section
 * @param[in] runOnlyIfTags an hash table containing all the tags allowed. If a section does not have a tag inside this set, it won't be run
 * @param[in] excludeIfTags an hash table containing all the tags prohibited. If a section has at least one tag inside this set, it won't be run
 * @return
 * 	\li true if the section has to be explored;
 * 	\li false otherwise;
 */
bool runOnceAndCheckAccessToSection(crashc_model * model, Section* section, condition_section cs, BeforeStartingSectionCallBack callback, const tag_ht* restrict runOnlyIfTags, const tag_ht* restrict excludeIfTags);
/**
 * Function supposed to run in the parentSwitcher cycle
 *
 * ::CONTAINABLESECTION is a 2 nested cycle. The outermost is the <b>parent switcher</b>, allowing you to return to the section parent
 * after you have analyzed the section (analyzed doesn't mean access to the section, but merely check the access).
 * Just like ::runOnceAndCheckAccessToSection, we model the if with a for loop. Since the for loop condition is run twice, we need to ensure the
 * first call is always true whilst the second one is always false (this ensure the "if" behaviour"). Moreover we want to perform additional
 * code.
 *
 * @param[in] section the section we want to fetch the associated parent from
 * @param[in] pointerToSetAsParent a pointer that we has to set to <tt>section->parent</tt> in this call. <b>This has to be done by exactly one of the callbacks</b>
 * @param[in] callback function always called after we have check the access to the section: if we have access, this function is called \b after we entered inside the section source code
 * @param[in] accessGrantedCallback function called \b afrer \c callback if the software has entered in the section source code
 * @param[in] accessDeniedCallback function called \b afrer \c callback if the software hasn't entered in the section source code
 */
bool runOnceAndDoWorkAtEnd(crashc_model * model, Section* section, Section** pointerToSetAsParent, AfterExecutedSectionCallBack callback, AfterExecutedSectionCallBack accessGrantedCallback, AfterExecutedSectionCallBack accessDeniedCallback);

/**
 * @param[in] parent the section containing the one we're creating. For example if we're in the test code of <tt>TESTCASE</tt> and we see a  <tt>WHEN</tt> clause
 * 				this attribute is set to the metadata representing <tt>TESTCASE</tt>.
 * @param[in] type the kind of section to fetch
 * @param[in] decription a brief string explaining what this section is and does
 * @param[in] tags a list of tags. See \ref tags for further information
 * @return
 * 	\li a newly created section if we're still computing the children of \c parent
 * 	\li the ::Section::currentChild -th child of \c parent otherwise
 */
Section* getSectionOrCreateIfNotExist(Section* parent, section_type type, const char* decription, const char* tags);

/**
 * Reset the ::currentSection global variable to the given one after we have detected a signal
 *
 * Sometimes it happens that we need to abrutely break the flow of ::currentSection.
 * For example when we detect an unhandled signal in one of the sections, we don't need to return to the parent of the section involved,
 * but immediately go to the ::TESTCASE.
 *
 * This function allows you to set all the metadata to ensure that such "unorthodox" flow is valid.
 *
 * \post
 * 	\li ::currentSection is valid and refers to \c s
 *
 * @param[inout] model the model to update
 * @param[in] signal the signal raised
 * @param[in] signalSection the section that caused a signal
 * @param[in] s the section ::currentSection will be moved to
 */
void resetFromSignalCurrentSectionTo(crashc_model* model, int signal, const Section* signaledSection, const Section* s);

/**
 * Compute the hash of a string
 *
 * \note
 * we use djb2 algorithm, described <a href="http://www.cse.yorku.ca/~oz/hash.html">here</a>
 *
 * @param[in] str the string whose has we need to compute
 * @return the has of the string
 */
int hash(const char* str);

///\defgroup accessConditions function that can be used as ::condition_section
///@{


/**
 * We use this function to check whether or not we need to enter in the
 * given WHEN section
 */
bool getAccess_When(crashc_model * model, Section * section);

/**
 * Grants always access
 *
 * @param[in] section the section involved
 */
bool getAlwaysTrue(crashc_model * model, Section* section);

/**
 * Grant access only to one section type per ::LOOPER loop
 *
 * Grant access only if:
 * \li we're still computing the number of children and we are in the first children;
 * \li we have already computed the number of children and the children we're analyzing is the one in the head of ::Section::sectionToRunList
 */
bool getAccessSequentially(Section* section);

///@}

/**
 * This callback is executed when we get access granted to a WHEN section, before executing its code.
 * It sets the currentSection's alreadyFoundWhen field and takes a snapshot of the WHEN section for the test report
 * and adds it to the test report snapshots tree.
 */
void callbackEnteringWhen(crashc_model * model, Section * section);

/**
 * This callback is executed when we get access granted to a THEN section, before executing its code.
 * It takes a snapshot of the section and adds it to the currentSection's test report snapshots tree.
 */
void callbackEnteringThen(crashc_model * model, Section * section);

/**
 * Updates the currentSnapshot, creating a new one if we started a new test
 * or adding a snapshot to the current test snapshots tree
 */
void updateCurrentSnapshot(crashc_model * model, Section * section);

///\defgroup AfterExecutedSectionCallbacks callbacks that can be used inside ::runOnceAndDoWorkAtEnd callbacks parameters
///@{

void doWorkAtEndCallbackGoToParentAndThenToNextSibling(crashc_model * model, Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackChildrenNumberComputed(crashc_model * model, Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackUpdateSectionToRun(crashc_model * model, Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun(crashc_model * model, Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackResetContainer(crashc_model * model, Section** pointerToSetAsParent, Section* child);
void doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling(crashc_model * model, Section** pointerToSetAsParent, Section* section);
/**
 * Do absolutely nothing
 */
void doWorkAtEndCallbackDoNothing(crashc_model * model, Section** pointerToSetAsParent, Section* section);

///@}

///\defgroup accessGrantedCallBack callbacks that can be used as accessGrantedCallBack in ::BeforeStartingSectionCallBack
///@{

/**
 * Does nothing
 */
void callbackDoNothing(crashc_model * model, Section* section);

/**
 * This callback it is used as accessGrantedCallback by TESTCASE sections.
 * It initializes the test report associated to the particular looper iteration
 * adds it to the test reports list and takes the snapshot of the testcase
 */
void callbackEnteringTestcase(crashc_model * model, Section * section);

/**
 * This callback is used when exiting from a testcase when access is granted: it simply updates the test report
 * outcome depending on the status of the last snapshot of the test tree and resets the currentSnapshot pointer
 * back to NULL to indicate that the current test is over
 */
void callbackExitAccessGrantedTestcase(crashc_model * model, Section ** pointerToSetAsParent, Section * section);

///@}

//END

/**
 * Main macro of the test suite
 *
 * @param[inout] model variable of type pointer of ::crashc_model containing all the data to manage
 */
#define CONTAINABLESECTION(model, parent, sectionType, description, tags, condition, accessGrantedCallBack, getBackToParentCallBack, exitFromContainerAccessGrantedCallback, exitFromContainerAccessDeniedCallback, setupCode)	\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																																							\
		(model)->currentSection = getSectionOrCreateIfNotExist(parent, sectionType, description, tags);																\
		(model)->currentSection->loopId += 1;																														\
		setupCode																																					\
		for (																																						\
				(model)->currentSection->loop1 = true																												\
				;																																					\
				runOnceAndDoWorkAtEnd((model), (model)->currentSection, &((model)->currentSection), 																\
						getBackToParentCallBack, exitFromContainerAccessGrantedCallback, exitFromContainerAccessDeniedCallback										\
				)																																					\
				;																																					\
				/**
				 *  This code is execute when we have already executed the code
				 *  inside the container. We assume every post condition of
				 *  CONTAINABLESECTION is satisfied for its children
				 *  CONTAINABLESECTION.
				 */																																					\
				 (model)->currentSection->loop1 = false																												\
		)																																							\
		for (																																						\
				(model)->currentSection->loop2 = true																												\
				;																																					\
				runOnceAndCheckAccessToSection((model), (model)->currentSection, condition, accessGrantedCallBack, (model)->runOnlyIfTags, (model)->excludeTags)	\
				;																																					\
				(model)->currentSection->loop2 = false,																												\
				markSectionAsExecuted((model)->currentSection)																										\
		)

#define NOCODE

/**
 * @param[inout] model a variable of type ::crashc_model containing all the data needed by crashc
 * @param[in] parent a variable of type ::Section representing the parent section of this section
 */
#define LOOPER(model, parent, sectionType, description, tags)																										\
		CONTAINABLESECTION(																																			\
				(model),																																			\
				parent, sectionType, description, tags,																												\
				getAlwaysTrue, callbackEnteringTestcase, 																											\
				doWorkAtEndCallbackResetContainer, callbackExitAccessGrantedTestcase,  doWorkAtEndCallbackDoNothing, 												\
																																									\
				(model)->testCaseInvolved = (model)->currentSection;																								\
				bool UV(signalDetected) = false;																													\
				if (sigsetjmp((model)->signal_jump_point, 1)) {                                                                                  					\
					/*we have caught a signal: here currentSection is the section where the signal was raised*/																							\
					markSectionAsSignalDetected((model)->currentSection);                                                                        					\
					UV(signalDetected) = true; 																														\
					/*we reset the currentSection to the test case*/																								\
					resetFromSignalCurrentSectionTo((model), (model)->currentSection->signalDetected, (model)->currentSection, (model)->testCaseInvolved);			\
				}                                                                                                                       							\
				for (    																																			\
						;																																			\
						!UV(signalDetected) && sectionStillNeedsExecution((model)->currentSection)                                                   				\
						;																																			\
				)																																					\
		)


//TODO the parent of the test case is not the root section, but the "suite section". We need to include them in the section tree as well!
#define TESTCASE(description, tags) LOOPER(&cc_model, ((&cc_model)->rootSection), ST_TESTCASE, description, tags)
#define EZ_TESTCASE(description) TESTCASE(description, "")

//#define TESTCASE(description, tags)	TEST_FUNCTION(testcase ## __LINE__, LOOPER(&rootSection, 1, description, tags))
//#define EZ_TESTCASE(description) TESTCASE(description, "")

#define ALWAYS_ENTER(model, sectionType, description, tags) CONTAINABLESECTION(															\
		(model), 																														\
		(model)->currentSection, sectionType, description, tags,																		\
		getAlwaysTrue, callbackEnteringThen,																							\
		doWorkAtEndCallbackGoToParentAndThenToNextSibling,	doWorkAtEndCallbackChildrenNumberComputed, doWorkAtEndCallbackDoNothing,	\
		NOCODE																															\
)

#define ENTER_ONE_PER_LOOP(model, sectionType, description, tags) CONTAINABLESECTION(													\
		(model), 																														\
		(model)->currentSection, sectionType, description, tags,																		\
		getAccess_When, callbackEnteringWhen, 																							\
		doWorkAtEndCallbackGoToParentAndThenToNextSibling, doWorkAtEndCallbackChildrenNumberComputed, doWorkAtEndCallbackDoNothing,		\
		NOCODE 																															\
)

#define WHEN(description, tags) ENTER_ONE_PER_LOOP((&cc_model), ST_WHEN, description, tags)
#define EZ_WHEN(description) WHEN(description, "")
#define THEN(description, tags) ALWAYS_ENTER((&cc_model), ST_THEN, description, tags)
#define EZ_THEN(description) THEN(description, "")

//TODO all those functions should be included in the only one global models
/**
 * Macro used to contain all test declarations and to generate the main function for the
 * execution of the various tests
 */
#define TESTS_START int main(const int argc, const char** args) { 																\
		cc_model = setupDefaultMainModel();																						\
		parseCommandLineArguments(argc, args, CC_TAGS_SEPARATOR, (&cc_model)->runOnlyIfTags, (&cc_model)->excludeTags); 		\
		registerSignalHandlerForSignals();

/**
 * This macro is used to register the teardown function. Note that the function itself must be written
 * by the user
 */
#define AFTER_TESTS(x) (&cc_model)->ct_teardown = x
/**
 * This macro is used to complete the mainfile main function and to start the execution
 * of the registered testsuites
 */
#define TESTS_END 																	\
    for (int i = 0; i < (&cc_model)->suites_array_index; i++) { 					\
    	(&cc_model)->tests_array[i](); 												\
    } 																				\
	(&cc_model)->report_producer_implementation->report_producer(&cc_model);		\
	if ((&cc_model)->ct_teardown != NULL) {											\
		(&cc_model)->ct_teardown();													\
	}																				\
	tearDownDefaultModel(cc_model);													\
} //main function closing bracket

/**
 * This macro is used to contain a test suite, which can contain an arbitrary number
 * of testcases. Every TESTSUITE must be given a unique ID. IDs must be valid C identifiers.
 * Progressive numerical IDs are strongly reccomended as the framework provides a few
 * really useful macros to easily register their testsuites.
 * Note that ID coherency is left as a user responsability.
 */
#define TESTSUITE(id) void suite_ ## id()

//FOREACH macros, used to iterate on the arguments passed to the REGTESTS macro
#define FE_0
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) WHAT(X); FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X); FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X); FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X); FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X); FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X); FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X); FE_7(WHAT, __VA_ARGS__)
#define FE_9(WHAT, X, ...) WHAT(X); FE_8(WHAT, __VA_ARGS__)
#define FE_10(WHAT, X, ...) WHAT(X); FE_9(WHAT, __VA_ARGS__)
#define FE_11(WHAT, X, ...) WHAT(X); FE_10(WHAT, __VA_ARGS__)
#define FE_12(WHAT, X, ...) WHAT(X); FE_11(WHAT, __VA_ARGS__)
#define FE_13(WHAT, X, ...) WHAT(X); FE_12(WHAT, __VA_ARGS__)
#define FE_14(WHAT, X, ...) WHAT(X); FE_13(WHAT, __VA_ARGS__)
#define FE_15(WHAT, X, ...) WHAT(X); FE_14(WHAT, __VA_ARGS__)
#define FE_16(WHAT, X, ...) WHAT(X); FE_15(WHAT, __VA_ARGS__)
#define FE_17(WHAT, X, ...) WHAT(X); FE_16(WHAT, __VA_ARGS__)
#define FE_18(WHAT, X, ...) WHAT(X); FE_17(WHAT, __VA_ARGS__)
#define FE_19(WHAT, X, ...) WHAT(X); FE_18(WHAT, __VA_ARGS__)
#define FE_20(WHAT, X, ...) WHAT(X); FE_19(WHAT, __VA_ARGS__)
#define FE_21(WHAT, X, ...) WHAT(X); FE_20(WHAT, __VA_ARGS__)
#define FE_22(WHAT, X, ...) WHAT(X); FE_21(WHAT, __VA_ARGS__)
#define FE_23(WHAT, X, ...) WHAT(X); FE_22(WHAT, __VA_ARGS__)
#define FE_24(WHAT, X, ...) WHAT(X); FE_23(WHAT, __VA_ARGS__)
#define FE_25(WHAT, X, ...) WHAT(X); FE_24(WHAT, __VA_ARGS__)
#define FE_26(WHAT, X, ...) WHAT(X); FE_25(WHAT, __VA_ARGS__)
#define FE_27(WHAT, X, ...) WHAT(X); FE_26(WHAT, __VA_ARGS__)
#define FE_28(WHAT, X, ...) WHAT(X); FE_27(WHAT, __VA_ARGS__)
#define FE_29(WHAT, X, ...) WHAT(X); FE_28(WHAT, __VA_ARGS__)
#define FE_30(WHAT, X, ...) WHAT(X); FE_29(WHAT, __VA_ARGS__)
#define FE_31(WHAT, X, ...) WHAT(X); FE_30(WHAT, __VA_ARGS__)
#define FE_32(WHAT, X, ...) WHAT(X); FE_31(WHAT, __VA_ARGS__)
#define FE_33(WHAT, X, ...) WHAT(X); FE_32(WHAT, __VA_ARGS__)
#define FE_34(WHAT, X, ...) WHAT(X); FE_33(WHAT, __VA_ARGS__)
#define FE_35(WHAT, X, ...) WHAT(X); FE_34(WHAT, __VA_ARGS__)
#define FE_36(WHAT, X, ...) WHAT(X); FE_35(WHAT, __VA_ARGS__)
#define FE_37(WHAT, X, ...) WHAT(X); FE_36(WHAT, __VA_ARGS__)
#define FE_38(WHAT, X, ...) WHAT(X); FE_37(WHAT, __VA_ARGS__)
#define FE_39(WHAT, X, ...) WHAT(X); FE_38(WHAT, __VA_ARGS__)
#define FE_40(WHAT, X, ...) WHAT(X); FE_39(WHAT, __VA_ARGS__)
#define FE_41(WHAT, X, ...) WHAT(X); FE_40(WHAT, __VA_ARGS__)
#define FE_42(WHAT, X, ...) WHAT(X); FE_41(WHAT, __VA_ARGS__)
#define FE_43(WHAT, X, ...) WHAT(X); FE_42(WHAT, __VA_ARGS__)
#define FE_44(WHAT, X, ...) WHAT(X); FE_43(WHAT, __VA_ARGS__)
#define FE_45(WHAT, X, ...) WHAT(X); FE_44(WHAT, __VA_ARGS__)
#define FE_46(WHAT, X, ...) WHAT(X); FE_45(WHAT, __VA_ARGS__)
#define FE_47(WHAT, X, ...) WHAT(X); FE_46(WHAT, __VA_ARGS__)
#define FE_48(WHAT, X, ...) WHAT(X); FE_47(WHAT, __VA_ARGS__)
#define FE_49(WHAT, X, ...) WHAT(X); FE_48(WHAT, __VA_ARGS__)
#define FE_50(WHAT, X, ...) WHAT(X); FE_49(WHAT, __VA_ARGS__)
#define FE_51(WHAT, X, ...) WHAT(X); FE_50(WHAT, __VA_ARGS__)
#define FE_52(WHAT, X, ...) WHAT(X); FE_51(WHAT, __VA_ARGS__)
#define FE_53(WHAT, X, ...) WHAT(X); FE_52(WHAT, __VA_ARGS__)
#define FE_54(WHAT, X, ...) WHAT(X); FE_53(WHAT, __VA_ARGS__)
#define FE_55(WHAT, X, ...) WHAT(X); FE_54(WHAT, __VA_ARGS__)
#define FE_56(WHAT, X, ...) WHAT(X); FE_55(WHAT, __VA_ARGS__)
#define FE_57(WHAT, X, ...) WHAT(X); FE_56(WHAT, __VA_ARGS__)
#define FE_58(WHAT, X, ...) WHAT(X); FE_57(WHAT, __VA_ARGS__)
#define FE_59(WHAT, X, ...) WHAT(X); FE_58(WHAT, __VA_ARGS__)
#define FE_60(WHAT, X, ...) WHAT(X); FE_59(WHAT, __VA_ARGS__)
#define FE_61(WHAT, X, ...) WHAT(X); FE_60(WHAT, __VA_ARGS__)
#define FE_62(WHAT, X, ...) WHAT(X); FE_61(WHAT, __VA_ARGS__)
#define FE_63(WHAT, X, ...) WHAT(X); FE_62(WHAT, __VA_ARGS__)
#define FE_64(WHAT, X, ...) WHAT(X); FE_63(WHAT, __VA_ARGS__)

/**
 * Used by REGTESTS() to get the name of the FE_n macro that is
 * to be used to start the iteration on the macro arguments
 */
#define GET_FE_NAME( 										\
          _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, 		\
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, 			\
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, 			\
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, 			\
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, 			\
         _51, _52, _53, _54, _55, _56, _57, _58, _59, 		\
         _60, _61, _62, _63, name, ...) name


/**
 * Macro that registers a single test suite given its ID.
 */
 #define REGISTER_SUITE(id) 							\
     void suite_ ## id(); 								\
     update_test_array((&cc_model), suite_ ## id)

/**
 * Alias of ::REGISTER_SUITE
 */
#define REG_SUITE(id) REGISTER_SUITE(id)

/**
 * Registers all the TESTSUITES whose ID is specified in the macro arguments.
 * Note that for a single use of the macro the maximum number of specified IDs is 64
 * due to CPP limitations. This problem is easily solved by the use of two or more
 * consequent macro uses.
 */
#define REGTESTS(...) GET_FE_NAME(__VA_ARGS__, FE_64, FE_63, FE_62, 							\
    FE_61, FE_60, FE_59, FE_58, FE_57, FE_56, FE_55, FE_54, FE_53, FE_52, FE_51,FE_50, 			\
    FE_49, FE_48, FE_47,FE_46,FE_45,FE_44,FE_43,FE_42,FE_41,FE_40, 								\
    FE_39,FE_38,FE_37,FE_36,FE_35,FE_34,FE_33,FE_32,FE_31,FE_30, 								\
    FE_29,FE_28,FE_27,FE_26,FE_25,FE_24,FE_23,FE_22,FE_21,FE_20, 								\
    FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10, 								\
    FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1, FE_0)(REGISTER_SUITE, __VA_ARGS__)


#endif
