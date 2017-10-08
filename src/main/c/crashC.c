#include "crashC.h"

Section rootSection = {0, 0, ST_ROOT, "root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;
Section* testCaseInvolved = NULL;
test_pointer tests_array[MAX_TESTS];
int suites_array_index = 0;
tag_ht* runOnlyIfTags = NULL;
tag_ht* excludeTags = NULL;

/**
 * This function registers a testsuite by storing its function pointer into
 * the global array. The function automatically updates the variable used to
 * keep track of the array dimension.
 * TODO: Add control on duplicates testsuites
 *
 *
 * @param[in] func the function to register
 */
void update_test_array(test_pointer func) {
    tests_array[suites_array_index] = func;
    suites_array_index++;
}

void setupContextTags() {
	if (runOnlyIfTags == NULL) {
		runOnlyIfTags = initHT();
	}
	if (excludeTags == NULL) {
		excludeTags = initHT();
	}

}


void tearDownContextTags() {
	if (runOnlyIfTags != NULL) {
		destroyHTCellWithElement(runOnlyIfTags, destroyTag);
		runOnlyIfTags = NULL;
	}
	if (excludeTags != NULL) {
		destroyHTCellWithElement(excludeTags, destroyTag);
		excludeTags = NULL;
	}
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

		//If we executed the section we check if this execution made the section
		//fully visited and update its status consequently
		if (isSectionFullyVisited(section)) {
			markSectionAsDone(section);
		}

		//We reset the WHEN found tag
		section->alreadyFoundWhen = false;

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
 *
 *  @param[in] type the kind of section we're getting.
 *  @see ::section_type
 */
Section* getSectionOrCreateIfNotExist(Section* parent, section_type type, const char* decription, const char* tags) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(type, parent->levelId + 1, decription, tags), parent);
	}
	return getNSection(parent, parent->currentChild);
}

void resetFromSignalCurrentSectionTo(int signal, const Section* signaledSection, const Section* s) {
	currentSection = s;
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
			//popFromList(section->parent->sectionToRunList);
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
			//addTailInList(section->parent->sectionToRunList, section);
		}
	}

	doWorkAtEndCallbackGoToParentAndThenToNextSibling(pointerToSetAsParent, section);
}

void doWorkAtEndCallbackDoNothing(Section** pointerToSetAsParent, Section* section) {

}

bool getAccess_When(Section * section) {
	//section is the WHEN we're considering right now

	//we don't enter in this WHEN if we have already entered in another WHEN of the parent
	if (section->parent->alreadyFoundWhen) {
		return false;
	}

	//we don't access to this WHEN if we have already completed it or if it generated a SECTION_SIGNAL
	if (section->status == SECTION_DONE || section->status == SECTION_SIGNAL_DETECTED) {
		return false;
	}

	return true;
}
/*
bool getAccessSequentially(Section* section) {
	if (section->parent == NULL) {
		return true;
	}

	if (section->parent->childrenNumberComputed == false) {
		return (section->parent->currentChild == 0);
	}


	if (getHeadOfList(section->parent->sectionToRunList) == section) {
		return true;
	}

	return false;
}*/

void callbackDoNothing(Section* section) {

}

void callbackSetAlreadyFoundWhen(Section * section) {
	section->parent->alreadyFoundWhen = true;
}

void signalCallback_doNothing(int signal, Section* signalledSection, Section* section, Section* targetSection) {

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
			(section->status == SECTION_DONE ? "yes" : "no"),
			section->currentChild,
			(section->loop1 ? "yes" : "no")
	);

	if (recursive && section->firstChild != NULL)
		printSectionData(section->firstChild, recursive);

	if (recursive && section->nextSibling != NULL) {
		printSectionData(section->nextSibling, recursive);
	}
}
