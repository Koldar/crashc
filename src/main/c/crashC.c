#include "crashC.h"

Section rootSection = {0, "root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;

static void printSectionData(const Section* section, bool recursive);

SectionCell* initSectionList() {
	return NULL;
}

void addSectionInHeadSectionList(SectionCell** list, const Section* section) {
	SectionCell* retVal = malloc(sizeof(SectionCell));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->section = section;
	retVal->next = NULL;

	if ((*list) == NULL) {
		*list = retVal;
	} else {
		retVal->next = *list;
		*list = retVal;
	}
}

Section* peekSectionInHeadSectionList(const SectionCell** list) {
	return (*list) != NULL ? (*list)->section : NULL;
}

bool containsSectionInSectionList(const SectionCell** list, const Section* section) {
	SectionCell* tmp = *list;
	while (tmp != NULL) {
		if (tmp->section == section) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

bool removeSectionInSectionList(SectionCell** list, const Section* section) {
	SectionCell* tmp = *list;
	SectionCell* previous = NULL;
	while (tmp != NULL) {
		if (tmp->section == section) {
			if (previous == NULL) {
				//first iteration
				*list = tmp->next;
			} else {
				previous->next = tmp->next;
			}
			free(tmp);
			return true;
		}
		previous = tmp;
		tmp = tmp->next;
	}
	return false;
}

Section* popHeadSectionInSectionList(SectionCell** list) {
	if (*list == NULL) {
		return NULL;
	}
	Section* retVal = (*list)->section;
	SectionCell* tmp = *list;
	*list = (*list)->next;
	free(tmp);
	return retVal;
}

void destroySectionList(SectionCell** list) {
	SectionCell* tmp = *list;
	SectionCell* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	*list = NULL;
}

Section* addSectionToParent(Section* restrict toAdd, Section* restrict parent) {
	Section* r = NULL;
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


Section* initSection(SectionLevelId levelId, const char* description, const char* tags) {
	Section* retVal = malloc(sizeof(Section));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->accessGranted = false;
	retVal->childrenNumber = 0;
	retVal->childrenNumberComputed = false;
	retVal->executed = false;
	retVal->currentChild = 0;
	retVal->description = strdup(description);
	retVal->firstChild = NULL;
	retVal->levelId = levelId;
	retVal->loopId = 0;
	retVal->sectionToRunList = initSectionList();
	retVal->loop1 = false;
	retVal->nextSibling = NULL;
	retVal->parent = NULL;
	retVal->tags = NULL;

	populateTagsHT(retVal, tags, ' ');

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

	Tag* current;
	Tag* tmp;
	HASH_ITER(hh, section->tags, current, tmp) {
		HASH_DEL(section->tags, current);
		destroyTag(current);
	}
	destroySectionList(&section->sectionToRunList);
	free(section->description);
	free(section);
}

bool areWeComputingChildren(const Section* section) {
	return !section->childrenNumberComputed;
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

bool haveWeRunEverythingInSection(const Section* section) {
	return section->executed;
}

bool haveWeRunEveryChildrenInSection(Section* section) {
	if (areWeComputingChildren(section)) {
		return false;
	}
	if (section->firstChild == NULL) {
		return true;
	}

	if (!haveWeRunEverythingInSection(section->firstChild)) {
		return false;
	}

	Section* tmp = section->firstChild->nextSibling;
	while (tmp != NULL) {
		if (!haveWeRunEverythingInSection(tmp)) {
			return false;
		}
		tmp = tmp->nextSibling;
	}
	return true;
}

void markSectionAsExecuted(Section* section) {
	section->executed = true;
}

Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription, const char* tags) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(sectionLevelId, decription, tags), parent);
	}
	return getNSection(parent, parent->currentChild);
}

void destroyTag(Tag* tag) {
	free(tag->name);
	free(tag);
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

void populateTagsHT(Section* section, const char* tags, char separator) {
	char token[100];
	char* positionToWriteInBuffer = NULL;
	int tokenId;
	Tag* tag;

	//i don't want to use strtok because it may be used inside the functions to test: i don't want to mess with their strtok starte
	while(*tags != '\0') {

		//fetch a tag inside tags
		positionToWriteInBuffer = token;
		while((*tags != separator) && (*tags != '\0')) {
			*positionToWriteInBuffer = *tags;
			positionToWriteInBuffer++;
			tags++;
		}
		*positionToWriteInBuffer = '\0';
		if (*tags != '\0') {
			//if we've not reached the end we need to go to the next tag
			tags++;
		}

		tokenId = hash(token);
		HASH_FIND_INT(section->tags, &tokenId, tag);
		if (tag == NULL) {
			tag = malloc(sizeof(Tag));
			if (tag == NULL) {
				MALLOCERRORCALLBACK();
			}
			tag->id = tokenId;
			tag->name = strdup(token);
			HASH_ADD_INT(section->tags, id, tag);
		}
	}
}

void addSectionInTailSectionList(SectionCell** list, Section* section) {
	SectionCell* toAdd = malloc(sizeof(SectionCell));
	if (toAdd == NULL) {
		MALLOCERRORCALLBACK();
	}

	toAdd->section = section;
	toAdd->next = NULL;
	printf("leak %s\n", section->description);

	SectionCell* tmp = *list;
	SectionCell** position = list;
	while (tmp != NULL) {
		position = &tmp->next;
		tmp = tmp->next;
	}
	*position = toAdd;
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
			popHeadSectionInSectionList(&(section->parent->sectionToRunList));
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
			addSectionInTailSectionList(&(section->parent->sectionToRunList), section);
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

	if (peekSectionInHeadSectionList(&(section->parent->sectionToRunList)) == section) {
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
