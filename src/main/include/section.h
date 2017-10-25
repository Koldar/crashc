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
#include "errors.h"

/**
 * Represents the type ::Section::levelId has
 */
typedef int SectionLevelId;
struct SectionCell;
struct SectionSnapshot;
typedef struct Section Section;


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

typedef enum {
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
	SECTION_SIGNAL_DETECTED,
	/**
	 * A section has this state when we encountered it but we didn't explore its body because the tags were uncompliant with the context
	 *
	 * Suppose the user chose to exclude sections whose tags contains "SKIP". If we encounter any section containing the tag "SKIP"
	 * we need to mark it somehow. This status represents this very concept.
	 */
	SECTION_SKIPPED_BY_TAG
} section_status_enum;


/**
 * This structure defines the possible statuses in which a snapshot can be.
 * This indicates the "exit status" of the section in the particular test
 */
typedef enum {
	SNAPSHOT_EXEC,
	SNAPSHOT_SKIPPED,
	SNAPSHOT_SIGNALED,
	SNAPSHOT_FAILED
} snapshot_status;

/**
 * This structure contains the informations contained in a Section at a precise moment of the program execution,
 * which are then used by the test reports to yield the user a more detailed set of informations about a given test.
 * Note that this structure contains only the fields of a Section which are subject to changes during the execution
 * of the tests. Note that this structure contains an implicit tree, used by CrashC to track the relationships between the
 * sections involved in a precise test
 */
typedef struct SectionSnapshot {
	/**
	 * The description of this section
	 */
	char * description;

	/**
	 * The tags associated to this section
	 */
	tag_ht *  tags;

	/**
	 * The type of this section
	 */
	section_type type;

	/**
	 * The status of the snapshot.
	 * This actually pretty much just mimic the status the section to which this snapshot
	 * refers to, but it is a bit different as some section statuses makes no sense in the
	 * context of the snapshot, e.g SECTION_UNEXEC, SECTION_EXEC, etc..
	 */
	snapshot_status status;

	/**
	 * The amount of time the section ran during one specific test.
	 * This field has no meaning until the section is complitely executed.
	 */
	long elapsed_time;

	/**
	 * The pointer to the parent of this snapshot in the snapshot tree
	 */
	struct SectionSnapshot * parent;

	/**
	 * The pointer to the first child of this snapshot in the snapshot tree
	 */
	struct SectionSnapshot * firstChild;

	/**
	 * The pointer to the next sibling of this snapshot in the snapshot tree
	 */
	struct SectionSnapshot * nextSibling;

} SectionSnapshot;

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
	 * An id that uniquely identifies a particular section.
	 *
	 * Like the pointer of the section, but more easy to read. Used internally
	 */
	int id;
	//TODO levelId is actually useless because you can fetch it from parent pointers
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
	tag_ht* tags;

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
	section_status_enum status;
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
	 * true if the software has given us the access to execute the code tag-wise
	 *
	 * The software can deny the access to the underlying section for 2 reasons:
	 * \li the section has incompatible tags with the tag context of the test run;
	 * \li the condition of the section is not satisfied;
	 *
	 *
	 * The first check is computed before the second one. Together with  ::Section::accessGranted, this field
	 * allow you to understand why a particular section has been denied. In particular this field is true
	 * if the section tags are compatible with the tag context; false otherwise
	 */
	bool accessTagGranted;

	/**
	 * This field is used to ensure that WHEN sections are executed in the proper order.
	 * Since only a single WHEN needs to be executed during a given testcase loop, we use
	 * this field in the parent section to know if during this cycle we already found and
	 * executed a WHEN section. This implies that this field needs to be reset at every cycle.
	 *
	 */
	bool alreadyFoundWhen;

	/**
	 * The signal datected when running this section
	 *
	 * The value is meaningful only when ::Section::status is set to ::SECTION_SIGNAL_DETECTED
	 */
	int signalDetected;

	/**
	 * The latest snapshot taken of this Section, used to create test reports
	 */
	struct SectionSnapshot * latestSnapshot;

	///the parent of this section in the tree. May be NULL
	struct Section* parent;
	///the first child this section has in the tree. May be NULL
	struct Section* firstChild;
	///the next sibling this section has in the tree. May be NULL
	struct Section* nextSibling;
};


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
 * @param[in] section the section to check
 * @return true if the section was marked as signal detected; false otherwise
 */
bool isSectionSignalDetected(const Section* section);

/**
 * Mark the section as \b executed
 *
 * @param[in] section the section to mark
 */
void markSectionAsExecuted(Section* section);

/**
 * Mark the section as \b fully visited
 *
 * @param[in] section the section to mark
 */
void markSectionAsDone(Section* section);

/**
 * Mark section as a one which we didn't explore because of the tags
 *
 * @param[in] section the section whose status we need to update
 */
void markSectionAsSkippedByTag(Section* section);

/**
 * This function tells if a given section will be executed again depending
 * on its status. More precisely, a section still needs to be executed when its status
 * is UNEXEC or EXEC. If it is SKIPPED_BY_TAG, DONE or SIGNALED then it means it will
 * no longer be executed.
 */
bool sectionStillNeedsExecution(Section * section);

/*
 * We use this function to determine wheter we can set a section as
 * fully visited, thus we don't need to execute it anymore.
 * A section is considered fully executed in two cases:
 * 1- When it has no child
 * 2- When every child of the section is fully visited itself
 *
 * Note that we first check if the section has been executed at least once, as
 * if a given section has never been executed it surely can not be fully visited.
 */
bool isSectionFullyVisited(Section * section);

/**
 * Check if the 2 hashtables has an intersection not null
 *
 * @param[in] tagSet1 the first tagset to check;
 * @param[in] tagSet2 the second tagset too check
 * @return
 * 	\li true if the 2 sets has at least one element in common;
 * 	\li false otherwise
 */
bool haveTagSetsIntersection(const tag_ht* tagSet1, const tag_ht* tagSet2);

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
