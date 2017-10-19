/*
 * section.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "section.h"
#include <stdio.h>
#include <stdarg.h>

#include "macros.h"

/**
 * The id the next section created with ::initSection will have.
 *
 * It goes from 1 and not from0 because 0 is **manually** set to the root section, created **statically**.
 * See ::rootSection
 */
static int nextSectionId = 1;

static void printSectionData(const Section* section, bool recursive);
static void computeDotFileOfSectionTree(FILE* fout, const Section* section);
static void updateDotFileOfSectionTreeWithSectionInfo(FILE* fout, const Section* section);
static void updateDotFileOfSectionTreeWithSectionEdges(FILE* fout, const Section* section);


/** Adds the given section as a children to the specified parent section.
 *  The function also automatically reorganize the internal sections tree
 *  by properly updating the involved nodes pointers.
 *  Returns a pointer to the added section.
 */
Section* addSectionToParent(Section* restrict toAdd, Section* restrict parent) {
	//NEVER USED - Section* r = NULL;
	Section* list = NULL;

	toAdd->parent = parent;

	list = parent->firstChild;
	if (list == NULL) {
		parent->firstChild = toAdd;
		return toAdd;
	}
	while (true) {
		if (list->nextSibling != NULL) {
			list = list->nextSibling;
		} else {
			list->nextSibling = toAdd;
			return toAdd;
		}
	}
}

/** Returns the n-th child of the given section or NULL if it has none.
 */
Section* getNSection(const Section* parent, int nChild) {
	Section* list = parent->firstChild;
	while(true) {
		if ((nChild == 0) || (list == NULL)) {
			return list;
		} else {
			list = list->nextSibling;
			nChild--;
		}
	}
}

/** Initializes a new Section struct given its levelId, description and tags.
 *  Other values are initialized to their default.
 *  Returns a pointes to the newly created Section.
 */
Section* initSection(section_type type, SectionLevelId levelId, const char* description, const char* tags) {
	Section* retVal = malloc(sizeof(Section));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->id = nextSectionId;
	nextSectionId += 1;
	retVal->accessGranted = false;
	retVal->alreadyFoundWhen = false;
	retVal->childrenNumber = 0;
	retVal->childrenNumberComputed = false;
	retVal->status = SECTION_UNEXEC;
	retVal->currentChild = 0;
	retVal->description = strdup(description);
	retVal->firstChild = NULL;
	retVal->type = type;
	retVal->levelId = levelId;
	retVal->loopId = 0;
	retVal->loop1 = false;
	retVal->loop2 = false;
	retVal->nextSibling = NULL;
	retVal->latestSnapshot = NULL;
	retVal->parent = NULL;
	retVal->tags = initHT();

	populateTagsHT(retVal->tags, tags, CC_TAGS_SEPARATOR);

	return retVal;
}

void destroySection(Section* section) {
	if (section->firstChild != NULL) {
		destroySection(section->firstChild);
	}
	if (section->nextSibling != NULL) {
		destroySection(section->nextSibling);
	}
	if (section->levelId <= 0) {
		//root section shouldn't be removed at all
		return;
	}

	destroyHTWithElements(section->tags, destroyTag);
	//destroyListWithElement(section->sectionToRunList, destroySection);
	free(section->description);
	free(section);
}

/** Checks whether we are still computing the number of a section's children and returns
 *  true in this case, false otherwise.
 */
bool areWeComputingChildren(const Section* section) {
	return !section->childrenNumberComputed;
}

bool haveWeRunEverythingInSection(const Section* section) {
	return (section->status == SECTION_DONE);
}

bool haveWeRunWholeTreeSection(const Section* rootSection) {
	printf("checking whether we have run the whole tree...\n");
	return haveWeRunEveryChildrenInSection(rootSection);
}

bool haveWeRunEveryChildrenInSection(const Section* section) {
	// ****************** CHECKING IF WE HAVE RUN EVERYTHING IN THE GIVEN SECTION *****************************

	if (section->status == SECTION_DONE) {
		return true;
	}

	//We still don't know how many children this section has, so we can't tell if we have completely run this section or not
	if (areWeComputingChildren(section)) {
		return false;
	}

	// ***************** CHECKING DIRECT CHILDREN OF THE GIVEN SECTION RECURSIVELY ******************************

	//the section has no children. So of course we have run everything in this section!
	if (section->firstChild == NULL) {
		return true;
	}

	//otherwise, we repeat the question to each children of the section:
	if (!haveWeRunEveryChildrenInSection(section->firstChild)) {
		return false;
	}
	Section* tmp = section->firstChild->nextSibling;
	while (tmp != NULL) {
		if (!haveWeRunEveryChildrenInSection(tmp)) {
			return false;
		}
		tmp = tmp->nextSibling;
	}
	return true;
}

int populateBufferStringOfSection(const Section* s, int spaceLeft, char* buffer) {
	int i = 0;
	i += snprintf(&buffer[i], spaceLeft - i, "[%d: %s; status:", s->levelId, s->description);
	i += populateBufferStringOfSectionStatus(s->status, spaceLeft - i, &buffer[i]);
	i += snprintf(&buffer[i], spaceLeft - i, "; when found: %s", s->alreadyFoundWhen ? "yes" : "no");
	i += snprintf(&buffer[i], spaceLeft - i, "]");
	return i;
}

int populateBufferStringOfSectionStatus(const int ss, int spaceLeft, char* buffer) {
	int i = 0;
	switch (ss) {
	case SECTION_DONE: { i += snprintf(&buffer[i], spaceLeft - i, "DONE"); break; }
	case SECTION_EXEC: { i += snprintf(&buffer[i], spaceLeft - i, "EXECUTED"); break; }
	case SECTION_UNEXEC: { i += snprintf(&buffer[i], spaceLeft - i, "UNEXECUTED"); break; }
	case SECTION_SIGNAL_DETECTED: { i += snprintf(&buffer[i], spaceLeft - i, "SIGNAL DETECTED"); break; }
	default: {
		fprintf(stderr, "invalid section status %d\n", ss);
		exit(1);
	}
	}

	return i;
}

void markSectionAsSignalDetected(Section* section) {
	section->status = SECTION_SIGNAL_DETECTED;
}

bool isSectionSignalDetected(const Section* section) {
	return section->status == SECTION_SIGNAL_DETECTED;
}

/** Marks the given section as already executed once by setting
 *  the appropriate section_status_enum value
 */
void markSectionAsExecuted(Section* section) {
	section->status = SECTION_EXEC;
}

/**
 * Marks the given section as fully visited by setting the
 * appropriate section_status_enum value
 */
void markSectionAsDone(Section * section) {
	section->status = SECTION_DONE;
}

void markSectionAsSkippedByTag(Section* section) {
	section->status = SECTION_SKIPPED_BY_TAG;
}

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
bool isSectionFullyVisited(Section * section) {
	if (section->status == SECTION_UNEXEC) {
		return false;
	}

	if (section->childrenNumber == 0) {
		return true;
	}
	else {
		Section * next_child = section->firstChild;

		while (next_child != NULL) {
			if (next_child->status != SECTION_DONE) {
				return false;
			}
			next_child = next_child->nextSibling;
		}
		return true;
	}
}

void drawSectionTree(const Section* section, const char* format, ...) {
	//create image name
	va_list ap;
	char imageTemplate[300];
	va_start(ap, format);
	vsnprintf(imageTemplate, 300, format, ap);
	va_end(ap);

	char dotFilename[300];
	strcpy(dotFilename, imageTemplate);
	strcat(dotFilename, ".dot");

	FILE* dotFile = fopen(dotFilename, "w");
	if (dotFile == NULL) {
		fprintf(stderr, "Can't create dotFile %s! Exiting...", dotFilename);
		exit(1);
	}
	computeDotFileOfSectionTree(dotFile, section);
	fclose(dotFile);

	char pngFilename[300];
	strcpy(pngFilename, imageTemplate);
	strcat(pngFilename, ".png");

	char command[300];
	snprintf(command, 300, "dot -Tpng -o%s %s", pngFilename, dotFilename);
	system(command);
	unlink(dotFilename);
}

static void computeDotFileOfSectionTree(FILE* fout, const Section* section) {
	fprintf(fout, "digraph {\n");

	// ******************* define nodes **********************
	updateDotFileOfSectionTreeWithSectionInfo(fout, section);
	// ******************* create edges **********************
	updateDotFileOfSectionTreeWithSectionEdges(fout, section);

	fprintf(fout, "}\n");
}

/**
 * Adds inside a *.dot file the lists of all the nodes inside the section tree starting from a given one
 *
 * \pre
 * 	\li \c fout open in "w" mode;
 * \post
 * 	\li \c fout contains node whose names patterns are "SECTION%5d" ("%05d" refers to a given section id)
 *
 * @param[inout] fout the file to write on
 * @param[in] section the section where we start writing information from
 */
static void updateDotFileOfSectionTreeWithSectionInfo(FILE* fout, const Section* section) {
	char buffer[300];
	populateBufferStringOfSectionStatus(section->status, 300, buffer);
	fprintf(fout, "\tSECTION%05d [label=\"%s\\nlevel=%d;\\nstatus=%s\", shape=\"box\"];\n", section->id, section->description, section->levelId, buffer);

	if (section->firstChild == NULL) {
		return;
	}

	updateDotFileOfSectionTreeWithSectionInfo(fout, section->firstChild);

	Section* tmp = section->firstChild->nextSibling;
	while (tmp != NULL) {
		updateDotFileOfSectionTreeWithSectionInfo(fout, tmp);
		tmp = tmp->nextSibling;
	}

}

/**
 * Adds inside a "*.dot" file all the edges between the nodes
 *
 * \pre
 * 	\li \c fout open in "w" mode;
 * 	\li ::updateDotFileOfSectionTreeWithSectionInfo called;
 * \post
 * 	\li \c fout contains all the edges;
 *
 * @param[inout] fout the file to write on
 * @param[in] section the section where we need to start adding edges from
 */
static void updateDotFileOfSectionTreeWithSectionEdges(FILE* fout, const Section* section) {
	if (section->firstChild == NULL) {
		return;
	}

	updateDotFileOfSectionTreeWithSectionEdges(fout, section->firstChild);

	fprintf(fout, "\tSECTION%05d -> SECTION%05d;\n", section->id, section->firstChild->id);
	Section* tmp = section->firstChild->nextSibling;
	while (tmp != NULL) {
		fprintf(fout, "SECTION%05d -> SECTION%05d;\n", section->id, tmp->id);
		tmp = tmp->nextSibling;
	}

}
