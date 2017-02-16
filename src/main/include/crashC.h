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
#include "forwardList.h"
#include "section.h"

/**
 * the character used to divide tags inside a single string
 */
#ifndef CC_TAGS_SEPARATOR
#	define CC_TAGS_SEPARATOR ' '
#endif

/**
 * Represents a default test main you can use to quickly run your tests with default functionalities
 */
#ifndef CC_AUTOMAIN
#	define CC_AUTOMAIN int main(int argc, const char* argv[]){											\
		return defaultMain(argc, argv);																	\
}
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
 * compute the has of a string
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
 * Grants always access
 *
 * @param[in] section the section involved
 */
bool getAlwaysTrue(Section* section);
/**
 * Grant access only to one section type per ::LOOPER loop
 *
 * Grant access only if:
 * \li we're still computing the number of children and we are in the first children;
 * \li we have already computed the number of children and the children we're analyzing is the one in the head of ::Section::sectionToRunList
 */
bool getAccessSequentially(Section* section);

///@}

int defaultMain(int argc, const char* argv[]);

///\defgroup AfterExecutedSectionCallbacks callbacks that can be used inside ::runOnceAndDoWorkAtEnd callbacks parameters
///@{

void doWorkAtEndCallbackGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackChildrenNumberComputed(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackUpdateSectionToRun(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun(Section** pointerToSetAsParent, Section* section);
void doWorkAtEndCallbackResetContainer(Section** pointerToSetAsParent, Section* child);
void doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section);
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

/**
 * Register a function defined by ::TEST_FUNCTION
 *
 * In my knowledge, it's impossible to auto register a function in C with macro programming.
 * From this assumption, we need to register in some way a function. This macro allows to mask the register itself.
 */
#define REGISTER_FUNCTION(testFunctionName)

#define LOOPER(parent, sectionLevelId, description, tags)																				\
		CONTAINABLESECTION(																												\
				parent, sectionLevelId, description, tags,																				\
				getAlwaysTrue, callbackDoNothing, 																						\
				doWorkAtEndCallbackResetContainer, doWorkAtEndCallbackDoNothing, doWorkAtEndCallbackDoNothing,							\
				for (																													\
						;																												\
						!haveWeRunEveryChildrenInSection(currentSection)																\
						;																												\
						\
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
		getAccessSequentially, callbackDoNothing, 																						\
		doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling, doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun, doWorkAtEndCallbackUpdateSectionToRun,		\
		NOCODE																															\
)

#define WHEN(description, tags) ENTER_ONE_PER_LOOP(5, description, tags)
#define EZ_WHEN(description) WHEN(description, "")
#define THEN(description, tags) ALWAYS_ENTER(10, description, tags)
#define EZ_THEN(description) THEN(description, "")

#endif
