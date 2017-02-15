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

#include "uthash.h"

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
 * Callback executed when a malloc returns NULL
 */
#ifndef MALLOCERRORCALLBACK
#	define MALLOCERRORCALLBACK() exit(1)
#endif

/**
 * represents a single tag cell inside a ::TagHashTable
 */
typedef struct Tag {
	///the key of the hashtable
	int id;
	///the value of the hashtable
	char* name;
	UT_hash_handle hh;
} Tag;

/**
 * An alias to improve readability inside the code
 */
typedef Tag TagHashTable;

/**
 * Represents the type ::Section::levelId has
 */
typedef int SectionLevelId;
struct SectionCell;

/**
 * Main structure representing a piece of testable code
 *
 * Examples of sections may be \c TESTCASE, \c WHEN, \c THEN and so on. Whilst this structure <b>does not contain</b>
 * any of the code inside the section, it represents the metadata of such code. An example of metadata is the number of
 * subsections a section has. We need them in order to generate useful statistics.
 *
 * For example the following code is structure as exposed in the graph:
 *
 * @code
 * 	TESTCASE("tc1", "") {
 * 		WHEN("w1", "") {
 * 			// code inside when 1
 * 		}
 * 		WHEN("w2", "") {
 * 			//code
 * 			THEN("t1", "") {
 * 				//code inside then 1
 * 			}
 * 			//code
 * 			THEN("t2", "") {
 * 				//code inside then 2
 * 			}
 * 			//code
 * 		}
 * 		WHEN("w3", "") {
 * 			THEN("t3", "") {
 * 				//code inside then 3
 * 			}
 * 			THEN("t4", "") {
 * 				//code inside then 4
 * 			}
 * 			THEN("t5", "") {
 * 				//code inside then 5
 * 			}
 * 		}
 * 	}
 * @endcode
 *
 * Sections can be organized as in a tree, like this:
 *
 * @dot
 * 	digraph {
 * 		rankdir="TB";
 *		TC1 [label="test case 1"];
 *
 *		subgraph {
 *			rank="same";
 *			WHEN1 [label="when 1"];
 *			WHEN2 [label="when 2"];
 *			WHEN3 [label="when 3"];
 *		}
 *
 *		subgraph {
 *			rank="same";
 *			THEN1 [label="then 1"];
 *			THEN2 [label="then 2"];
 *			THEN3 [label="then 3"];
 *			THEN4 [label="then 4"];
 *			THEN5 [label="then 5"];
 *		}
 *
 *		TC1 -> WHEN1;
 *		WHEN1 -> WHEN2;
 *		WHEN2 -> WHEN3;
 *		WHEN2 -> THEN1;
 *		THEN1 -> THEN2;
 *		WHEN3 -> THEN3;
 *		THEN3 -> THEN4;
 *		THEN4 -> THEN5;
 * 	}
 * @enddot
 *
 * Note that the code inside when2 (for example, is all the code between the thens plus the THENs themselves
 */
typedef struct Section {
	/**
	 * The id of the level of the section
	 *
	 * Sections on the same level in the tree will share the same level id.
	 * Section near the root of the tree will have ids near 0 whilst sections near the bottom
	 * will have ids near +infinity
	 */
	SectionLevelId levelId;
	/**
	 * Decription of the section
	 */
	const char* description;
	/**
	 * List of tags associated to the section
	 */
	TagHashTable* tags;

	/**
	 * determine if ::Section::childrenNumber has a meaning
	 *
	 * The field is true if we have already scanned the section content at least once. This means
	 * that we explore it and we whether there are subsections inside it. Since we explored it
	 * we also know the number of children the section has, hence we can safely use
	 * ::Section::childNumber and  ::Section::firstChild
	 */
	bool childrenNumberComputed;
	/**
	 * the number of subsection this section has
	 *
	 * This field has only a meaning when ::Section::childrenNumberComputed is set to true.
	 * For example, in the graph inside ::Section, section "when 2" has 2 children
	 */
	int childrenNumber;
	bool executed;

	/**
	 * The number of the child we're currently analyzing
	 *
	 * Suppose you are in when3 for the first time (this also applies, although for different reasons, even if you're exploring the section
	 * for the n-th time). You need to know the number of subsection in order to fully create the tree. After you've done analyzing a subsection (i.e then3),
	 * you need to increase a counter and pass to the next one (i.e. then4). This field does exactly this.
	 *
	 * The counter is also useful to synchronize the walk inside the source code and the walk inside the section metadata tree.
	 */
	int currentChild;
	/**
	 * The number of times the testing framework encounter this section instance
	 *
	 * \note
	 * "encounters" means that we reach the macro definition. It doesn't matter if we actually enter or not, the counter will increase nonetheless
	 */
	int loopId;

	/**
	 * represents the variable used to loop only once inside the first loop in ::CONTAINABLESECTION
	 *
	 * \attention this variable is for internal usage only
	 *
	 * The for loop is an excellent way to have, after a macro, "{ }" and to execute code after the actual "{ }" code content.
	 * However, we need to call the for content only once. Since section are nested, we cannot define a counter to loop over otherwise
	 * 2 nested section will generate a <c>varaible redefinition</b> compiler error. Hence we store the loop varaible inside the section itself.
	 */
	bool loop1;
	/**
	 * like ::Section::loop1, but for the second loop inside ::CONTAINABLESECTION
	 */
	bool loop2;
	/**
	 * true if the software has given us the access to execute the code inside the Section, false otheriwse
	 */
	bool accessGranted;

	/**
	 * queue used to check which child section has to be executed next.
	 *
	 * \attention
	 * This field is used only for a particular ::CONTAINABLESECTION implementation, not for every section. In particular
	 * this field is used to implement \c WHEN behaviour.
	 *
	 * Suppose you have a test case with lots of whens
	 *
	 * @code
	 * 	TESTCASE("tc1","") {
	 * 		code1;
	 * 		WHEN("w1","") {
	 * 		}
	 * 		code2;
	 * 		WHEN("w2","") {
	 * 		}
	 * 		code3;
	 * 		WHEN("w3","") {
	 * 		}
	 * 		code4;
	 * 	}
	 * @endcode
	 *
	 * Since you need to compute only a when per time, we need something to tell us which is the next when we need to compute.
	 * The head of this list tell us exactly this.
	 *
	 * \todo however, the structure will fail if inside the test case there are 2 different section level (ie. 2 whens and 1 then)
	 */
	struct SectionCell* sectionToRunList;


	///the parent of this section in the tree. May be NULL
	struct Section* parent;
	///the first child this section has in the tree. May be NULL
	struct Section* firstChild;
	///the next sibling this section has in the tree. May be NULL
	struct Section* nextSibling;
} Section;

/**
 * A cell inside a forward list of Sections
 */
typedef struct SectionCell {
	///the section in this cell
	Section* section;
	///pointer to the next cell. Can be NULL
	struct SectionCell* next;
} SectionCell;

typedef SectionCell SectionList;

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
 * Initialize a list of sections
 *
 * @return an instance of section lit
 */
SectionList* initSectionList();
/**
 * Adds a section inside a section list in the \b head
 *
 * @param[in] list the list to update
 * @param[in] section the section to add
 */
void addSectionInHeadSectionList(SectionCell** list, const Section* section);
/**
 * like ::popHeadSectionInSectionList but it doesn't remove the head of the list
 *
 * @param[in] list the list whose head we need to get
 * @return
 * 	\li the section inside the head;
 * 	\li NULL if the list is empty;
 */
Section* peekSectionInHeadSectionList(const SectionCell** list);
/**
 * \note
 * The comparison is done by checking the pointers
 *
 * @param[in] list the list to analyze
 * @param[in] section the section to chekc
 * @return true if \c section is inside \c list
 */
bool containsSectionInSectionList(const SectionCell** list, const Section* section);
/**
 * Removes a section inside a list
 *
 * \note
 * The ::Section won't be removed from the memory at all
 *
 * the list won't be changed if the section is not inside the \c list to begin with
 *
 * @param[in] list the section to update
 * @param[in] section the section to remove form the list
 */
bool removeSectionInSectionList(SectionCell** list, const Section* section);
/**
 * Removes the head of the section list
 *
 * @param[in] list the list to update
 * @return
 * 	\li the section inside the head of the list;
 * 	\li NULL if the list is empty;
 */
Section* popHeadSectionInSectionList(SectionCell** list);
/**
 * Removes from the memory the whole list of sections
 *
 * \note
 * The sections pointed by \c list won't be affected at all
 *
 * @param[in] the list to free
 */
void destroySectionList(SectionCell** list);
/**
 * Adds a ::Section inside the children list of a parent section
 *
 * @param[in] toAdd the new child \c parent has;
 * @param[in] parent the parent \c toAdd have from this point on;
 * @return toAdd
 */
Section* addSectionToParent(Section* restrict toAdd, Section* restrict parent);
/**
 * get the n-th child of a given ::Section
 *
 * @param[in] parent the parent involved
 * @param[in] the number of the child we want to fetch
 * @return
 * 	\li the n-th child of parent;
 * 	\li NULL if such child does not exist
 */
Section* getNSection(const Section* parent, int nChild);
/**
 * Creates a new section
 *
 * \attention
 * The function allocates data in the heap. To remove it, call ::destroySection
 *
 * @param[in] levelId the level of this section
 * @param[in] description a text describing briefly the section
 * @param[in] tags a single string containing all the tags associated to the section. See \ref tags
 * @return the new ::Section instance jsut created
 */
Section* initSection(SectionLevelId levelId, const char* description, const char* tags);
/**
 * Destroy a ::Section inside the heap
 *
 * \note
 * The function will destory all the sections which have parent (directly or indirectly) \c section.
 * Formally, the function will free from the memory the subtree generate by \c section (\c section included).
 *
 * There is only a ::Section that won't be freed by this functions: all the sections with ::Section::levelId set to 0 won't be touched.
 * Normally this isn't a problem because there is only one ::Section with such an id: ::rootSection. The rationale is that such variable is global one
 * allocated in the \b stack, so freeing such a variable is illegal.
 *
 * @param[in] the section to free
 */
void destroySection(Section* section);
/**
 * @param[in] section the section to analyze
 * @return true if we're still computing the number of children of \c section, false otheriwse
 */
bool areWeComputingChildren(const Section* section);
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
 * @param[in] section involved
 * @return True if the source code section has been executed at least once
 */
bool haveWeRunEverythingInSection(const Section* section);
/**
 * @param[in] section the section involved
 * @return true if we have executed at least once every <b>direct</b> children of this section
 */
bool haveWeRunEveryChildrenInSection(Section* section);
/**
 * Mark the section as \b executed
 *
 * @param[in] section the section to mark
 */
void markSectionAsExecuted(Section* section);
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
 * Removed from memory a previously created tag
 *
 * @param[in] tag the tag to destroy
 */
void destroyTag(Tag* tag);
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
/**
 * Given a string \c tags, the function will populate the hash table in \c section with all
 * the tags inside \c tags
 *
 * For example if \c separator is ' ', a compliant \c tags string is <tt>tag1 tag2 tag3</tt>
 *
 * \note
 * the strings generated are \b cloned inside the hash table
 *
 * @param[in] section the section whose tag hash table will be populated
 * @param[in] tags a string containing "separator"-separated substring, each of them representing a tag
 * @param[in] separator a character dividing 2 tags in string \c tags
 */
void populateTagsHT(Section* section, const char* tags, char separator);

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
void addSectionInTailSectionList(SectionCell** list, Section* section);

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
