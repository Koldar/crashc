#include "crashC.h"

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
