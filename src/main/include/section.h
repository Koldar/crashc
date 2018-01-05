/**
 * @file
 *
 * Provides Section struct, which is used to represents @containablesection
 *
 * @author koldar
 * @date Feb 16, 2017
 */

#ifndef SECTION_H_
#define SECTION_H_

#include <stdbool.h>

#include "tag.h"
#include "errors.h"
#include "typedefs.h"
#include "list.h"

/**
 * Represents the type of a ::Section
 *
 * section type is used to uniquely determine the behaviour of the section itself.
 */
typedef enum section_type {
	/**
	 * The section is the root of the tree.
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
 * Represents each state a ::Section can have
 *
 * ::Section state is used to check whether or not @crashc runtime should access to the related @containablesection code
 * or not. This answer greatly depends on the section_type of the given ::Section as well.
 */
typedef enum {
	/**
	 * Represents a section @crashc knows exists, but it didn't visited it at all
	 */
	SECTION_UNEXEC,
	/**
	 * Represents a section @crashc has visited, but it didn't fully visited
	 *
	 * @definition Partially visited section
	 * It's a section where at least one **direct** or **indirect** child section has not been visited
	 */
	SECTION_EXEC,
	/**
	 * Represents a section CrashC has fully explored
	 *
	 * @definition Fully visited section
	 * It's a section where all its direct and indirect children sections have been fully visited
	 */
	SECTION_DONE,
	/**
	 * A section has this state when inside its body the program encountered a signal
	 *
	 * The status is set only when the code **directed** owned by the section generates a signal. So for example in
	 * the figure below the signal was raised during the source code of section "B", after executing section "C" but before section "D"
	 *
	 * @dotfile signalDetectedTree.dot
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
	SECTION_SKIPPED_BY_TAG,
} section_status_enum;


/**
 * Defines the possible statuses in which a snapshot can be.
 *
 * This indicates the "exit status" of the section in the particular test
 */
typedef enum {
	/**
	 * A snapshot contains all **passed assertions**
	 */
	SNAPSHOT_OK,
	/**
	 * A snapshot run a code which produced an unhandled signal
	 */
	SNAPSHOT_SIGNALED,
	/**
	 * A snapshot contained a **failed assertions**
	 *
	 */
	SNAPSHOT_FAILED
} snapshot_status;

/**
 * Contains relevant test results information contained in a ::Section at a precise moment of the program execution
 *
 * @definition Section Snapshot
 * Represents the in-test information a given ::Section has. Note that one ::Section may actually have several, different, section shapshot, since
 * a @containablesection represented by a ::Section can be run multiple times in a @testcase. A section snapshot is tree because it is a run
 * of a single cycle of a ::LOOPER, like @testcase: in it, the section are organized as a **tree**, not just as a list. For example a @then may have multiple
 * but different subsection @then.
 *
 * Such information are then used by the test reports to yield to the user a more detailed set of informations about a given test.
 * Note that this structure contains only the fields of a ::Section which are subject to changes during the execution
 * of the tests. Furthermore, note that this structure represents an implicit tree, used by @crashc to track the relationships between the
 * sections involved in a precise test
 */
struct SectionSnapshot {
	/**
	 * The description of the ::Section represented by the struct
	 *
	 * @notnull
	 */
	char * description;
	/**
	 * The tags associated to the ::Section represented by the struct
	 *
	 * @notnull
	 */
	tag_ht *  tags;
	/**
	 * The type of the ::Section represented by the struct
	 */
	section_type type;
	/**
	 * The status of the snapshot.
	 *
	 * This pretty much just mimic the status the section to which this snapshot
	 * refers to, but without some section status which makes no sense in the
	 * context of the snapshot: for example SECTION_UNEXEC, SECTION_EXEC are one of them
	 */
	snapshot_status status;
	/**
	 * The amount of time, in microseconds, the section ran during one specific test.
	 *
	 * This field has no meaning until the associated ::Section has been completely exeuted during the specific test
	 */
	long elapsed_time;
	/**
	 * The list of reports of the assertions executed in the ::Section represented.
	 *
	 * @notnull
	 */
	ct_list_t * assertion_reports;
	/**
	 * The pointer to the parent snapshot in the snapshot tree.
	 *
	 * If @null, the snapshot is actually the root of the tree
	 */
	struct SectionSnapshot * parent;
	/**
	 * The pointer to the next sibling of this snapshot in the snapshot tree
	 *
	 * If @null, the snapshot is actually the last child of the parent in the snaphshot tree
	 */
	struct SectionSnapshot * next_sibling;
	/**
	 * The pointer to the first child of this snapshot in the snapshot tree
	 *
	 * If @null, the snapshot has no children, therefore it's a leaf in the snapshot tree
	 */
	struct SectionSnapshot * first_child;
};

/**
 * Main structure representing a piece of testable code
 *
 * @definition section
 * It's data structure representing, in @crashc framework, a given @containablesection.
 * ::Section is one of the most important data structure in @crashc. section are hierarchuically organized in a tree, called **section tree**
 *
 * Examples of sections may be @testcase, @when, @then and so on. Whilst this structure **does not contain**
 * any of the code inside the section, it represents the metadata of such code. An example of metadata is the number of
 * subsections (aka children) a section has. We need them in order to generate useful statistics.
 *
 * For example the following code is structure as exposed in the graph:
 *
 * @include exampleOfTestCase.dox
 *
 * Sections can be organized as in a tree, like this:
 *
 * @dotfile exampleOfTestCase.dot
 *
 * Note that the code inside "when 2" is all the code between the thens plus the code within each @then (a total of 9 lines).
 */
struct Section {
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
	 * For example you can use this field to get whether the section is a @when, a @then or something else
	 */
	section_type type;
	/**
	 * Description of the section
	 *
	 * @notnull
	 */
	const char* description;
	/**
	 * List of tags associated to the section
	 *
	 * @notnull
	 */
	tag_ht* tags;

	/**
	 * determine if ::Section::childrenNumber has a meaning
	 *
	 * The field is @true if we have already scanned the section content at least once. This means
	 * that we explore the associated @containablesection from the beginning till the end. This has 2 effects:
	 * 	\li we know whether there are subsections inside the @containablesection or not;
	 *	\li we know the actual number of children the section has;
	 *
	 * The second effects allows us to safely use Section::childrenNumber and  Section::firstChild
	 */
	bool childrenNumberComputed;
	/**
	 * the number of subsection this section has
	 *
	 * This field has only a meaning when ::Section::childrenNumberComputed is set to @true.
	 * For example, given the code:
	 *
	 * @include exampleOfTestCase.dox
	 *
	 * section "when 2" has 2 children, namely "then 1" and "then 2"
	 */
	int childrenNumber;
	/**
	 * Holds information about the current state of the section.
	 *
	 */
	section_status_enum status;
	/**
	 * The number of the child we're currently analyzing
	 *
	 * Given the test code:
	 *
	 * @include exampleOfTestCase.dox
	 *
	 * Suppose you are in "when 3" for the first time (this also applies, although for different reasons, even if you're exploring the section
	 * for the n-th time). Since when you just entered in "when 3" you can't possibly know the exact children of the section, since the program still needs to read
	 * the code, you can't immediately create the relevant section tree.
	 *
	 * After you've done analyzing a subsection (i.e "then 3"), you need to increase a counter representing the number of children you've encoutered
	 * so far when analyzing "when 3" and keep analyzing "when 3" by executing the code, eventually arriving to the next section child (i.e. "then 4").
	 * This field does exactly represent such counter. When the program arrives at the end of "when 3", this counter will also represents the actual number of children
	 * "when 3" has.
	 */
	int currentChild;
	/**
	 * The number of times the @crashc encountered this section instance
	 *
	 * \note
	 * "encounters" means that we reach the macro definition. It doesn't matter if we actually enter or not, the counter will increase nonetheless
	 */
	int loopId;

	/**
	 * The variable used to loop only once inside parent switcher in ::CONTAINABLESECTION
	 *
	 * \attention this variable is for internal usage only
	 *
	 * The for loop is an excellent way to have, after a macro, "{ }" and to execute code after the actual "{ }" code content.
	 * However, we need to call the for content only once. Since section are nested, we cannot define a counter to loop over otherwise
	 * 2 nested section will generate a <c>variable redefinition</c> compile error. Hence we store the loop variable inside the section itself.
	 */
	bool loop1;
	/**
	 * like ::Section::loop1, but for the access cycle inside ::CONTAINABLESECTION
	 */
	bool loop2;
	/**
	 * @true if the software has given us the access to execute the code inside the Section, @false otherwise
	 */
	bool accessGranted;
	/**
	 * @true if the software has given us the access to execute the code tag-wise
	 *
	 * The software can deny the access to the underlying section for 2 reasons:
	 * \li the section has incompatible tags with the tag context of the test run;
	 * \li the condition of the section is not satisfied;
	 *
	 *
	 * The first check is computed before the second one. Together with  ::Section::accessGranted, this field
	 * allow you to understand why a particular section has been denied. In particular this field is @true
	 * if the section tags are compatible with the tag context; @false otherwise
	 */
	bool accessTagGranted;
	/**
	 * Used to ensure that sibling @when sections are executed in the proper order.
	 *
	 * Since only a single @when needs to be executed during a given @testcase loop, we use
	 * this field in the parent section to know if during this cycle we already found and
	 * executed a @when section.
	 *
	 * This implies that this field needs to be reset at every @testcase cycle.
	 */
	bool alreadyFoundWhen;
	/**
	 * The signal detected when running this section
	 *
	 * The value is meaningful only when ::Section::status is set to ::SECTION_SIGNAL_DETECTED
	 */
	int signalDetected;
	//TODO remove
	/**
	 * The latest snapshot taken of this Section, used to create test reports
	 *
	struct SectionSnapshot * latestSnapshot;
	*/
	///the parent of this section in the tree. May be @null
	struct Section* parent;
	///the first child this section has in the tree. May be @null
	struct Section* firstChild;
	///the next sibling this section has in the tree. May be @null
	struct Section* nextSibling;
};

/**
 * Adds a ::Section inside the children list of a parent section
 *
 * @param[in] toAdd the new child \c parent has;
 * @param[inout] parent the parent \c toAdd have from this point on;
 * @return \c toAdd
 */
Section* addSectionToParent(Section* restrict toAdd, Section* restrict parent);
/**
 * get the n-th child of a given ::Section
 *
 * @param[in] parent the parent involved
 * @param[in] nChild the number of the child we want to fetch
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
 * @return the new ::Section instance just created
 */
Section* initSection(section_type type, SectionLevelId levelId, const char* description, const char* tags);
/**
 * Destroy a ::Section inside the heap
 *
 * \note
 * The function will destory all the sections which have parent (directly or indirectly) \c section.
 * Formally, the function will free from the memory the subtree generate by \c section (\c section included).
 *
 * There is only a ::Section that won't be freed by this function: all the sections with Section::levelId set to 0 won't be touched.
 * Normally this isn't a problem because there is only one ::Section with such an id: Section::rootSection. The rationale is that such variable is global one
 * allocated in the \b stack, so freeing such a variable is illegal.
 *
 * @param[inout] section the section to free
 */
void destroySection(Section* section);

/**
 * Check if we're still retrieving the number fo children a given section has
 *
 * @param[in] section the section to analyze
 * @return
 * 	\li @true if we're still computing the number of children of \c section,
 * 	\li @false otheriwse
 */
bool areWeComputingChildren(const Section* section);

/**
 * Check if every section known in the tree generated by the given section has been run
 *
 * @param[in] rootSection the section generating a section tree; we need to check if we have run every section inside that tree
 * @return
 * 	\li @true if the tree generated by \c rootSection is totally known (no children unknown);
 * 	\li @true if we have explored every known children;
 * 	\li @false if we find at least one children totally or partially unexplored;
 */
bool haveWeRunWholeTreeSection(const Section* rootSection);

/**
 * Check if have run at least once every **direct** children of the given section
 *
 * @param[in] section the section involved
 * @return
 *  \li @true if we have executed at least once every **direct** children of this section;
 *  \li @false otherwise
 */
bool haveWeRunEveryChildrenInSection(const Section* section);

/**
 * Marks the given section as a section where the related @containablesection raised a signal
 *
 * @param[inout] section the section to alter
 */
void markSectionAsSignalDetected(Section* section);

/**
 * Check if the related @containablesection has raise a signal
 *
 * @param[in] section the section to check
 * @return true if the section was marked as signal detected; false otherwise
 */
bool isSectionSignalDetected(const Section* section);

/**
 * Mark the section as **partially executed**
 *
 * @param[inout] section the section to mark
 */
void markSectionAsExecuted(Section* section);

/**
 * Mark the section as **fully visited**
 *
 * @param[inout] section the section to mark
 */
void markSectionAsDone(Section* section);

/**
 * Mark section as a one which we didn't explore because of the tags
 *
 * @param[inout] section the section whose status we need to update
 */
void markSectionAsSkippedByTag(Section* section);

/**
 * Tells if a given section will be executed again depending on its status.
 *
 * @param[in] section the section to manage
 * @return
 *  \li @true if the status is either ::SECTION_UNEXEC or ::SECTION_EXEC;
 *  \li @false iff the status is either ::SECTION_SKIPPED_BY_TAG, ::SECTION_DONE or ::SECTION_SIGNAL_DETECTED;
 */
bool sectionStillNeedsExecution(Section * section);

/*
 * We use this function to determine whether we can set a section as fully visited.
 *
 * If so, we don't need to execute it anymore.
 * A section is considered fully executed in two cases:
 * -# When it has no child
 * -# When every **direct** children of the section is fully visited itself
 *
 * Note that we first check if the section has been executed at least once, as
 * if a given section has never been executed it surely can not be fully visited.
 *
 * @param[in] section the section to manage
 * @return
 * 	\li @true if we don't need to visit the section anymore;
 * 	\li @false otherwise
 */
bool isSectionFullyVisited(Section * section);

/**
 * Check if the 2 hashtables has an intersection not null
 *
 * @param[in] tagSet1 the first tagset to check;
 * @param[in] tagSet2 the second tagset too check
 * @return
 * 	\li @true if the 2 sets has at least one element in common;
 * 	\li @false otherwise
 */
bool haveTagSetsIntersection(const tag_ht* tagSet1, const tag_ht* tagSet2);

/**
 * Populated a buffer containing a string representation of the section
 *
 * @param[in] s the section whose string representation we want to retrieve
 * @param[in] spaceLeft the size of the buffer
 * @param[inout] buffer the buffer itself
 * @return the number of characters added in the buffer. So after this operation, the space left in the buffer will be \f$ spaceLeft - retVal \f$;
 */
int populateBufferStringOfSection(const Section* s, int spaceLeft, char* buffer);

/**
 * Populated a buffer containing a string representastion of the section status
 *
 * @param[in] ss the section status to print
 * @param[in] spaceLeft the size of the buffer
 * @param[inout] buffer the buffer to populate
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
