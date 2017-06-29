#include "crashC.h"

Section rootSection = {0, "root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;
test_pointer tests_array[MAX_TESTS];
int suites_array_index = 0;

/**
 * This function registers a testsuite by storing its function pointer into
 * the global array. The function automatically updates the variable used to
 * keep track of the array dimension.
 * TODO: Add control on duplicates testsuites
 *
 * TODO
 * 	Understand why id is useful
 *
 * @param[in] id ignored parameter
 * @param[in] func the function to register
 */
void update_test_array(int id, test_pointer func) {
    tests_array[suites_array_index] = func;
    suites_array_index++;
}

bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, BeforeStartingSectionCallBack callback) {
	if (!section->loop2) {
		return false;
	}
	section->accessGranted = cs(section);
	if (section->accessGranted) {
		callback(section);
	}
	return section->accessGranted;
}

bool runOnceAndDoWorkAtEnd(Section* section, Section** pointerToSetAsParent, AfterExecutedSectionCallBack callback, AfterExecutedSectionCallBack accessGrantedCallback, AfterExecutedSectionCallBack accessDeniedCallback) {
	if (section->loop1) {
		return true;
	}
	//callback is always executed and it can (and often will) change pointerToSetAsParent and child pointers (since they point to the same structure).
	//in order to use the previously pointed structure we copy the child pointer. As for pointerToSetAsParent, we can do nothing since it will be changed nonetheless
	Section* _child = section;
	callback(pointerToSetAsParent, section);
	if (section->accessGranted) {
		accessGrantedCallback(pointerToSetAsParent, _child);
	} else {
		accessDeniedCallback(pointerToSetAsParent, _child);
	}
	return false;
}

/** If we are still computing the children of the given parent section the function
 *  creates a new section with the given levelId, description,tags and adds it to its
 *  parent section and returns a pointer to the new section.
 *  Else, the function returns a pointer to the currentChild of the given section.
 */
Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription, const char* tags) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(sectionLevelId, decription, tags), parent);
	}
	return getNSection(parent, parent->currentChild);
}

int hash(const char* str) {
	unsigned int hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

bool getAlwaysTrue(Section* section) {
	return true;
}

void doWorkAtEndCallbackGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section) {
	//we finish a section. we return to the parent
	*pointerToSetAsParent = section->parent;
	//we go to the next sibling of child
	(*pointerToSetAsParent)->currentChild += 1;
}

void doWorkAtEndCallbackChildrenNumberComputed(Section** pointerToSetAsParent, Section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (!section->childrenNumberComputed) {
		section->childrenNumberComputed = true;
	}
	section->currentChild = 0;
}

void doWorkAtEndCallbackUpdateSectionToRun(Section** pointerToSetAsParent, Section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (section->parent->childrenNumberComputed) {
		//since childrenNumberComputed is not false, that means that we are executing the cycle at least the second time. Hence
		//we need to pop the head of sectionToRunList. However we don't need to pop the head when we end a WHEN, but when we end a loop cycle.
		//in order to do it, we pop the end after we executed the last children
		if (section->nextSibling == NULL) {
			popHeadFromForwardList(&(section->parent->sectionToRunList));
		}
	}
}

void doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun(Section** pointerToSetAsParent, Section* section) {
	doWorkAtEndCallbackUpdateSectionToRun(pointerToSetAsParent, section);
	doWorkAtEndCallbackChildrenNumberComputed(pointerToSetAsParent, section);
}

void doWorkAtEndCallbackResetContainer(Section** pointerToSetAsParent, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	child->currentChild = 0;
}

void doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section) {
	//in the first cycle, when we're trying to compute the number of children of the parent, we also initialize the sectionToRunList
	if (!section->parent->childrenNumberComputed) {
		//we can add every children of parent except the first one: such child has already run while we were computing the number of children
		if (section->parent->currentChild > 0) {
			addTailInForwardList(&(section->parent->sectionToRunList), section);
		}
	}

	doWorkAtEndCallbackGoToParentAndThenToNextSibling(pointerToSetAsParent, section);
}

void doWorkAtEndCallbackDoNothing(Section** pointerToSetAsParent, Section* section) {

}

bool getAccessSequentially(Section* section) {
	if (section->parent == NULL) {
		return true;
	}

	if (section->parent->childrenNumberComputed == false) {
		return (section->parent->currentChild == 0);
	}


	if (peekHeadFromForwardList(&(section->parent->sectionToRunList)) == section) {
		return true;
	}

	return false;
}

void callbackDoNothing(Section* section) {

}

int defaultMain(int argc, const char* argv[]) {

}

/**
 * Prints  a debug information about the section
 *
 * @param[in] section the section to analyze
 * @param[in] recursive true if you want to print information about the children (both direct and indirect) of this section, false otherwise;
 */
static void printSectionData(const Section* section, bool recursive) {
	fprintf(stdout, "****************\ndescription=%s\nparent=%s\nchildrenNumber=%d\nchildrenNumberComputed=%s\nexecuted=%s\ncurrentChild=%d\nloop1=%s\n",
			section->description,
			(section->parent != NULL ? section->parent->description : "<none>"),
			section->childrenNumber,
			(section->childrenNumberComputed ? "yes" : "no"),
			(section->executed ? "yes" : "no"),
			section->currentChild,
			(section->loop1 ? "yes" : "no")
	);

	if (recursive && section->firstChild != NULL)
		printSectionData(section->firstChild, recursive);

	if (recursive && section->nextSibling != NULL) {
		printSectionData(section->nextSibling, recursive);
	}
}
