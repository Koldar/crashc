#include "crashC.h"
#include "main_model.h"

void update_test_array(test_pointer func, crashc_model* model) {
	model->tests_array[model->suites_array_index] = func;
	model->suites_array_index++;
}

bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, BeforeStartingSectionCallBack callback, const tag_ht* restrict runOnlyIfTags, const tag_ht* restrict excludeIfTags) {
	if (!section->loop2) {
		return false;
	}

	//TODO here we need  to replace the parameter runOnlyWithTags and excludeIfTags with a pointer of the global model
	//check if the section we're dealing with is compliant with the context tags
	if (!isHTEmpty(excludeIfTags)) {
		if (haveTagSetsIntersection(section->tags, excludeIfTags)) {
			section->accessTagGranted = false;
			section->accessGranted = false;
			markSectionAsSkippedByTag(section);
			return false;
		}
	}

	if (!isHTEmpty(runOnlyIfTags)) {
		if (!haveTagSetsIntersection(section->tags, runOnlyIfTags)) {
			section->accessTagGranted = false;
			section->accessGranted = false;
			markSectionAsSkippedByTag(section);
			return false;
		}
	}
	section->accessTagGranted = true;

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
		//markSectionAsExecuted(section);
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

void resetFromSignalCurrentSectionTo(crashc_model* model, int signal, const Section* signaledSection, const Section* s) {
	model->currentSection = s;
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

void doWorkAtEndCallbackResetContainer(Section** pointerToSetAsParent, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	child->currentChild = 0;
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

void callbackDoNothing(Section* section) {

}

void callbackSetAlreadyFoundWhen(Section * section) {
	section->parent->alreadyFoundWhen = true;
}

void signalCallback_doNothing(int signal, Section* signalledSection, Section* section, Section* targetSection) {

}
