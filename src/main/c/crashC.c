#include <ct_utils.h>
#include "crashC.h"
#include "main_model.h"
#include "list.h"

void update_test_array(crashc_model * model, test_pointer func) {
	model->tests_array[model->suites_array_index] = func;
	model->suites_array_index++;
}

bool runOnceAndCheckAccessToSection(crashc_model * model, Section* section, condition_section cs, BeforeStartingSectionCallBack callback, const tag_ht* restrict runOnlyIfTags, const tag_ht* restrict excludeIfTags) {
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

	section->accessGranted = cs(model, section);
	if (section->accessGranted) {
		callback(model, section);
	}
	return section->accessGranted;
}

bool runOnceAndDoWorkAtEnd(crashc_model * model, Section* section, Section** pointerToSetAsParent, AfterExecutedSectionCallBack callback, AfterExecutedSectionCallBack accessGrantedCallback, AfterExecutedSectionCallBack accessDeniedCallback) {
	if (section->loop1) {
		return true;
	}
	//callback is always executed and it can (and often will) change pointerToSetAsParent and child pointers (since they point to the same structure).
	//in order to use the previously pointed structure we copy the child pointer. As for pointerToSetAsParent, we can do nothing since it will be changed nonetheless
	Section* _child = section;
	callback(model, pointerToSetAsParent, section);
	if (section->accessGranted) {
		//markSectionAsExecuted(section);
		//If we executed the section we check if this execution made the section
		//fully visited and update its status consequently
		if (isSectionFullyVisited(section)) {
			markSectionAsDone(section);
		}

		//We reset the WHEN found tag
		section->alreadyFoundWhen = false;

		accessGrantedCallback(model, pointerToSetAsParent, _child);
	} else {
		accessDeniedCallback(model, pointerToSetAsParent, _child);
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

bool getAlwaysTrue(crashc_model * model, Section* section) {
	return true;
}

void doWorkAtEndCallbackGoToParentAndThenToNextSibling(crashc_model * model, Section** pointerToSetAsParent, Section* section) {
	//we finish a section. we return to the parent
	*pointerToSetAsParent = section->parent;
	//we go to the next sibling of child
	(*pointerToSetAsParent)->currentChild += 1;
}

void doWorkAtEndCallbackChildrenNumberComputed(crashc_model * model, Section** pointerToSetAsParent, Section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (!section->childrenNumberComputed) {
		section->childrenNumberComputed = true;
	}
	section->currentChild = 0;

	updateSnapshotStatus(model->currentSection, model->currentSnapshot);

	model->currentSnapshot = model->currentSnapshot->parent;
}

void doWorkAtEndCallbackResetContainer(crashc_model * model, Section** pointerToSetAsParent, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	child->currentChild = 0;
}

void doWorkAtEndCallbackDoNothing(crashc_model * model, Section** pointerToSetAsParent, Section* section) {

}

bool getAccess_When(crashc_model * model, Section * section) {
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

void callbackDoNothing(crashc_model * model, Section* section) {

}

void callbackEnteringThen(crashc_model * model, Section * section) {
	updateCurrentSnapshot(model, section);
}

void callbackEnteringWhen(crashc_model * model, Section * section) {
	section->parent->alreadyFoundWhen = true;

	updateCurrentSnapshot(model, section);
}

void callbackExitAccessGrantedTestcase(crashc_model * model, Section ** pointerToSetAsParent, Section * section) {
	TestReport * report = getLastElementOfList(model->test_reports_list);
	SectionSnapshot * last_snapshot = model->currentSnapshot;

	updateSnapshotStatus(section, model->currentSnapshot);
	updateTestOutcome(report, last_snapshot);

	//Resets the currentSnapshot pointer to NULL to indicate the end of the test
	model->currentSnapshot = NULL;
}

/**
 * This functions updates the currentSnapshot. More precisely:
 *  - if currentSnapshot is NULL it means we just started a new test, so it simply takes a section snapshot and sets it
 *  	as the currentSnapshot
 *  - if currentSnapshot is not NULL it means we are not in a new test, so we take a snapshot and adds it to the test report tree
 */

void updateCurrentSnapshot(crashc_model * model, Section * section) {
	SectionSnapshot * snapshot = initSectionSnapshot(section);

	if (model->currentSnapshot == NULL) {
		model->currentSnapshot = snapshot;
	}
	else {
		model->currentSnapshot = addSnapshotToTree(snapshot, model->currentSnapshot);
	}

}

void callbackEnteringTestcase(crashc_model * model, Section * section) {
	TestReport * report = initTestReport(model->currentSnapshot);
	addTailInList(model->test_reports_list, report);

	updateCurrentSnapshot(model, model->currentSection);

	report->testcase_snapshot = model->currentSnapshot;
}

void signalCallback_doNothing(int signal, Section* signalledSection, Section* section, Section* targetSection) {

}
