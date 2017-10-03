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
#include "uthash.h"
#include "section.h"
#include "sigHandling.h"

/**
 * the character used to divide tags inside a single string
 */
#ifndef CC_TAGS_SEPARATOR
#	define CC_TAGS_SEPARATOR ' '
#endif

/**
 * Callback representing a general condition that determine if we can access to a particular section
 *
 * the parameter is the section to check whilst the return value is
 * true if the software grant us access to the section, false otherwise
 */
typedef bool (*condition_section)(Section*);
typedef void (*AfterExecutedSectionCallBack)(Section** parentPosition, Section* child);
typedef void (*BeforeStartingSectionCallBack)(Section* sectionGranted);

/**
 * Function pointer used to point at the functions which contain the testsuites.
 *
 */
typedef void (*test_pointer)();

/**
 * Array containing the pointers to the testsuites functions
 */
extern test_pointer tests_array[];

/**
 * This variable is used to keep track of the tests_array[] array dimension
 */
extern int suites_array_index;

/**
 * global variable representing the root of the section tree
 */
extern Section rootSection;
/**
 * global variable representing what is the section we're analyzing right now.
 *
 * Suppose we have the tree shown in ::Section. Suppose you're analyzing when2: you need something
 * that allows you to synchronize what the code is reading and the section tree metadata; aka you need something
 * that points which section you're actually in. This variable is that pointer.
 *
 * When you're in when2 this variable is set to the node in the tree representing when2. When you enter inside the code of "then1",
 * this variable is reset to point then 1 ::Section. Then, when you return to when2 to execute the code between then1 and then2, this variable
 * is set again in a way to point "when2".
 */
extern Section* currentSection;


/**
 * Function used to update the tests_array[] array when registering a new testsuite
 */
void update_test_array(test_pointer);

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
 */
bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, BeforeStartingSectionCallBack callback);
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
bool runOnceAndDoWorkAtEnd(Section* section, Section** pointerToSetAsParent, AfterExecutedSectionCallBack callback, AfterExecutedSectionCallBack accessGrantedCallback, AfterExecutedSectionCallBack accessDeniedCallback);

/**
 * @param[in] parent the section containing the one we're creating. For example if we're in the test code of <tt>TESTCASE</tt> and we see a  <tt>WHEN</tt> clause
 * 				this attribute is set to the metadata representing <tt>TESTCASE</tt>.
 * @param[in] sectionLevelId the level the children is located
 * @param[in] decription a brief string explaining what this section is and does
 * @param[in] tags a list of tags. See \ref tags for further information
 * @return
 * 	\li a newly created section if we're still computing the children of \c parent
 * 	\li the ::Section::currentChild -th child of \c parent otherwise
 */
Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription, const char* tags);

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
bool getAccess_When(Section * section);

/**
 * Grants always access
 *
 * @param[in] section the section involved
 */
bool getAlwaysTrue(Section* section);

///@}

void callbackSetAlreadyFoundWhen(Section * section);

///\defgroup AfterExecutedSectionCallbacks callbacks that can be used inside ::runOnceAndDoWorkAtEnd callbacks parameters
///@{

void doWorkAtEndCallbackGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackChildrenNumberComputed(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackUpdateSectionToRun(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackResetContainer(Section** pointerToSetAsParent, Section* child);
void doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section);
/**
 * Do absolutely nothing
 */
void doWorkAtEndCallbackDoNothing(Section** pointerToSetAsParent, Section* section);

///@}

///\defgroup accessGrantedCallBack callbacks that can be used as accessGrantedCallBack in ::BeforeStartingSectionCallBack
///@{

/**
 * Does nothing
 */
void callbackDoNothing(Section* section);

///@}

//END

/**
 * Main macro of the test suite
 */
#define CONTAINABLESECTION(parent, sectionLevelId, description, tags, condition, accessGrantedCallBack, getBackToParentCallBack, exitFromContainerAccessGrantedCallback, exitFromContainerAccessDeniedCallback, setupCode)	\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																																\
		currentSection = getSectionOrCreateIfNotExist(parent, sectionLevelId, description, tags);												\
		currentSection->loopId += 1;																									\
		setupCode																														\
		for (																															\
				currentSection->loop1 = true																							\
				;																														\
				runOnceAndDoWorkAtEnd(currentSection, &currentSection, 																	\
						getBackToParentCallBack, exitFromContainerAccessGrantedCallback, exitFromContainerAccessDeniedCallback			\
				)																														\
				;																														\
				/**
				 *  This code is execute when we have already executed the code
				 *  inside the container. We assume every post condition of
				 *  CONTAINABLESECTION is satisfied for its children
				 *  CONTAINABLESECTION.
				 */																														\
				 currentSection->loop1 = false																							\
		)																																\
		for (																															\
				currentSection->loop2 = true																							\
				;																														\
				runOnceAndCheckAccessToSection(currentSection, condition, accessGrantedCallBack)										\
				;																														\
				currentSection->loop2 = false,																							\
				markSectionAsExecuted(currentSection)																					\
		)

#define NOCODE

/**
 * Represents a macro that defines a function
 */
#define TEST_FUNCTION(testFunctionName)																							\
	void testFunctionName()

#define LOOPER(parent, sectionLevelId, description, tags)																				\
		CONTAINABLESECTION(																												\
				parent, sectionLevelId, description, tags,																				\
				getAlwaysTrue, callbackDoNothing, 																						\
				doWorkAtEndCallbackResetContainer, doWorkAtEndCallbackDoNothing,  doWorkAtEndCallbackDoNothing, 																						    \
				if (setjmp(signal_jump_point)) {                                                                                        \
					markSectionAsExecuted(currentSection);                                                                              \
				}                                                                                                                       \
				for (    																												\
						;																												\
						!haveWeRunEveryChildrenAndSignalHandlingSetup(currentSection)                                                   \
						;																												\
				)																														\
		)


#define TESTCASE(description, tags) LOOPER(&rootSection, 1, description, tags)
#define EZ_TESTCASE(description) TESTCASE(description, "")

//#define TESTCASE(description, tags)	TEST_FUNCTION(testcase ## __LINE__, LOOPER(&rootSection, 1, description, tags))
//#define EZ_TESTCASE(description) TESTCASE(description, "")

#define ALWAYS_ENTER(sectionLevelId, description, tags) CONTAINABLESECTION(																\
		currentSection, sectionLevelId, description, tags,																				\
		getAlwaysTrue, callbackDoNothing,																								\
		doWorkAtEndCallbackGoToParentAndThenToNextSibling,	doWorkAtEndCallbackChildrenNumberComputed, doWorkAtEndCallbackDoNothing,					\
		NOCODE																															\
)

#define ENTER_ONE_PER_LOOP(sectionLevelId, description, tags) CONTAINABLESECTION(														\
		currentSection, sectionLevelId, description, tags,																				\
		getAccess_When, callbackSetAlreadyFoundWhen, 																						\
		doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling, doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun, doWorkAtEndCallbackUpdateSectionToRun,																							\
		NOCODE 																															\
)

#define WHEN(description, tags) ENTER_ONE_PER_LOOP(5, description, tags)
#define EZ_WHEN(description) WHEN(description, "")
#define THEN(description, tags) ALWAYS_ENTER(10, description, tags)
#define EZ_THEN(description) THEN(description, "")

//Multiple source files support
/* The maximum number of registrable suites. This macro is used to set the
 * initial size of the internal array used to contain the tests functions pointers
 */
#ifndef MAX_TESTS
#   define MAX_TESTS 256
#endif

/**
 * Macro used to contain all test declarations and to generate the main function for the
 * execution of the various tests
 */
#define TESTS_START int main(void) {

/**
 * This macro is used to complete the mainfile main function and to start the execution
 * of the registered testsuites
 */
#define TESTS_END \
    for (int i = 0; i < suites_array_index; i++) { \
        tests_array[i](); \
    } \
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
#define GET_FE_NAME( \
          _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, \
         _60, _61, _62, _63, name, ...) name


/**
 * Macro that registers a single test suite given its ID.
 */
 #define REG_TEST(id) \
     void suite_ ## id(); \
     update_test_array(suite_ ## id)

/**
 * Registers all the TESTSUITES whose ID is specified in the macro arguments.
 * Note that for a single use of the macro the maximum number of specified IDs is 64
 * due to CPP limitations. This problem is easily solved by the use of two or more
 * consequent macro uses.
 */
#define REGTESTS(...) GET_FE_NAME(__VA_ARGS__, FE_64, FE_63, FE_62, \
    FE_61, FE_60, FE_59, FE_58, FE_57, FE_56, FE_55, FE_54, FE_53, FE_52, FE_51,FE_50, \
    FE_49, FE_48, FE_47,FE_46,FE_45,FE_44,FE_43,FE_42,FE_41,FE_40, \
    FE_39,FE_38,FE_37,FE_36,FE_35,FE_34,FE_33,FE_32,FE_31,FE_30, \
    FE_29,FE_28,FE_27,FE_26,FE_25,FE_24,FE_23,FE_22,FE_21,FE_20, \
    FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10, \
    FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1, FE_0)(REG_TEST, __VA_ARGS__)


#endif
