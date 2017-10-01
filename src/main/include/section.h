/*
 * section.h
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#ifndef SECTION_H_
#define SECTION_H_

#include <stdbool.h>
#include "tag.h"
#include "testReport.h"

/**
 * Represents the type ::Section::levelId has
 */
typedef int SectionLevelId;
struct SectionCell;

/**
 * Represents the type of this section
 *
 * secton type can be used to uniquely determine the behaviour of the section itself
 */
typedef enum section_type {
	/**
	 * The section is the root of the tree
	 */
	ST_ROOT,
	/**
	 * The section is a suite case
	 */
	ST_SUITECASE,
	/**
	 * The section is a testcase
	 */
	ST_TESTCASE,
	/**
	 * The section is a when
	 */
	ST_WHEN,
	/**
	 * The section is a then
	 */
	ST_THEN
} section_type;

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
	 * An id that uniquely identifies a prticular section.
	 *
	 * Like the pointer of the section, but more easy to read. Used internally
	 */
	int id;
	/**
	 * The id of the level of the section
	 *
	 * Sections on the same level in the tree will share the same level id.
	 * Section near the root of the tree will have ids near 0 whilst sections near the bottom
	 * will have ids near +infinity
	 */
	SectionLevelId levelId;
	/**
	 * Represents the type of this section.
	 *
	 * For example you can use this field to get whether the section is a WHEN, a THEN or something else
	 */
	section_type type;
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

	/**
	 * This field is very important, as it holds information about the current state of the section.
	 * It can assume different values:
	 * -SECTION_UNEXEC:		means that the section has never been executed before
	 * -SECTION_EXEC  :		means that the section has been executed at least once but needs to be executed again for some reason
	 * -SECTION_DONE  :		means that the section has been fully visited and needs not to be executed anymore
	 * By definition, we consider a section to be done when is has no child and has been executed at least once or when all of
	 * its children are done themselves.
	 */
	enum {
		SECTION_UNEXEC,
		SECTION_EXEC,
		SECTION_DONE,
		/**
		 * A section has this state when inside its body the program encountered a signal
		 *
		 * The status is set only when the code **directed** owned by the section generates a signal. So for example in
		 * the figure below the signal was raised during the source code of section "B", after executing section "C" but before section "D"
		 * @dot
		 * \digraph {
		 *  A[label="A\\nEXEC"]; B[label="B\\SIGNAL_DETECTED"]; C[label="C\\nDONE"] D[label="UNEXEC"]; E[label="E\\nUNEXEC"]; F[label="F\\nUNEXEC"];
		 *	A -> B -> C; B -> D; A -> E; A -> F;
		 * }
		 * @enddot
		 *
		 * Sections with this status should not be visited anymore (so even if there are undirect children with status ::SECTION_UNEXEC,
		 * those section will never be run at all.
		 */
		SECTION_SIGNAL_DETECTED
	} status;

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
	 * 2 nested section will generate a <c>variable redefinition</c> compiler error. Hence we store the loop variable inside the section itself.
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
	 * This field is used to ensure that WHEN sections are executed in the proper order.
	 * Since only a single WHEN needs to be executed during a given testcase loop, we use
	 * this field in the parent section to know if during this cycle we already found and
	 * executed a WHEN section. This implies that this field needs to be reset at every cycle.
	 *
	 */
	bool alreadyFoundWhen;

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
	 *
	struct SectionCell* sectionToRunList;*/ //I dont think this is really needed
	/**
	 * A list containing all the assertions inside this section
	 */
	TestReportList* assertionReportList;
	/**
	 * A list containing all the failures inside ::Section::assertionReportList
	 */
	TestReportList* failureReportList;
	/**
	 * The signal datected when running this section
	 *
	 * The value is meaningful only when ::Section::status is set to ::SECTION_SIGNAL_DETECTED
	 */
	int signalDetected;


	///the parent of this section in the tree. May be NULL
	struct Section* parent;
	///the first child this section has in the tree. May be NULL
	struct Section* firstChild;
	///the next sibling this section has in the tree. May be NULL
	struct Section* nextSibling;
} Section;


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
 * @param[in] type the type of this section
 * @param[in] levelId the level of this section
 * @param[in] description a text describing briefly the section
 * @param[in] tags a single string containing all the tags associated to the section. See \ref tags
 * @return the new ::Section instance jsut created
 */
Section* initSection(section_type type, SectionLevelId levelId, const char* description, const char* tags);
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
 * Check if every section known in the tree generated by the given section has been ru
 *
 * @param[in] rootSection the section generating a section tree; we need to chekc if we have run every section inside that tree
 * @return
 * 	\li true if the tree generated by \c rootSection is totally known (no children unknown);
 * 	\li true if we have explored every known children;
 * 	\li false if we find at least one children totally or partially unexplored;
 */
bool haveWeRunWholeTreeSection(const Section* rootSection);

/**
 * @param[in] section involved
 * @return True if the source code section has been executed at least once
 */
bool haveWeRunEverythingInSection(const Section* section);
/**
 * @param[in] section the section involved
 * @return true if we have executed at least once every <b>direct</b> children of this section
 */
bool haveWeRunEveryChildrenInSection(const Section* section);

/**
 * Marks the given section as a section where a signal was indeed raised
 *
 * @param[inout] section the section to alter
 */
void markSectionAsSignalDetected(Section* section);

/**
 * Mark the section as \b executed
 *
 * @param[in] section the section to mark
 */
void markSectionAsExecuted(Section* section);

//Documentation in .c file, TODO: move it here
bool isSectionFullyVisited(Section * section);

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

/**
 * Populated a buffer containing a string representation of the section
 *
 * @param[in] s the section whose string representation we want to retrieve
 * @param[in] spaceLeft the size of the buffer
 * @param[inout] the buffer itself
 * @return the number of characters added in the buffer. So after this operation, the space left in the buffer will be \f$ spaceLeft - retVal \f$;
 */
int populateBufferStringOfSection(const Section* s, int spaceLeft, char* buffer);

/**
 * Populated a buffer containing a string representastion of the section status
 *
 * @param[in] ss the section status to print
 * @param[in] spaceLeft the size of the buffer
 * @param[inout] the buffer to populate
 * @return number of characters filled in the buffer
 */
int populateBufferStringOfSectionStatus(const int ss, int spaceLeft, char* buffer);

/**
 * Generates an image of the section tree useful for debugging purposes
 *
 * \pre
 * 	\li graphviz installed on your system and available in your PATH;
 * \post
 * 	\li a png file named like \c format will be present. The path is relative to the CWD
 *
 * @param[in] section the section tree where you want to start drawing
 * @param[in] format a `printf` like format string with all the formatter strings
 * @param[in] ... variadic components of `printf`
 */
void drawSectionTree(const Section* section, const char* format, ...);

#endif /* SECTION_H_ */
