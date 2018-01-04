#include <ct_utils.h>

#include "crashC.h"
#include "main_model.h"
#include "list.h"

void ct_update_test_array(ct_model_t* model, test_pointer func) {
	model->tests_array[model->suites_array_index] = func;
	model->suites_array_index++;
}

bool ct_run_once_check_access(ct_model_t* model, Section* section, ct_access_callback_t cs, ct_enter_callback_t callback, const tag_ht* restrict run_tags, const tag_ht* restrict exclude_tags) {
	if (!section->loop2) {
		return false;
	}

	//TODO here we need  to replace the parameter runOnlyWithTags and excludeIfTags with a pointer of the global model
	//check if the section we're dealing with is compliant with the context tags
	if (!isHTEmpty(exclude_tags)) {
		if (haveTagSetsIntersection(section->tags, exclude_tags)) {
			section->accessTagGranted = false;
			section->accessGranted = false;
			markSectionAsSkippedByTag(section);
			return false;
		}
	}

	if (!isHTEmpty(run_tags)) {
		if (!haveTagSetsIntersection(section->tags, run_tags)) {
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

bool ct_run_once_final_work(ct_model_t* model, Section* section, Section** pointer_to_set_as_parent, ct_exit_callback_t callback, ct_exit_callback_t access_granted_callback, ct_exit_callback_t access_denied_callback) {
	if (section->loop1) {
		return true;
	}
	//callback is always executed and it can (and often will) change pointerToSetAsParent and child pointers (since they point to the same structure).
	//in order to use the previously pointed structure we copy the child pointer. As for pointerToSetAsParent, we can do nothing since it will be changed nonetheless
	Section* _child = section;
	callback(model, pointer_to_set_as_parent, section);
	if (section->accessGranted) {
		//markSectionAsExecuted(section);
		//If we executed the section we check if this execution made the section
		//fully visited and update its status consequently
		if (isSectionFullyVisited(section)) {
			markSectionAsDone(section);
		}

		//We reset the WHEN found tag
		section->alreadyFoundWhen = false;

		access_granted_callback(model, pointer_to_set_as_parent, _child);
	} else {
		access_denied_callback(model, pointer_to_set_as_parent, _child);
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
Section* ct_fetch_section(Section* parent, section_type type, const char* description, const char* tags) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(type, parent->levelId + 1, description, tags), parent);
	}
	return getNSection(parent, parent->currentChild);
}

void ct_reset_section_after_jump(ct_model_t* model, Section* const jump_source_section, Section* const testcase_section) {
	model->current_section = testcase_section;
}

bool ct_always_enter(ct_model_t* model, Section* section) {
	return true;
}

void ct_exit_callback_next_sibling(ct_model_t* model, Section** pointer_to_set_as_parent, Section* section) {
	//we finish a section. we return to the parent
	*pointer_to_set_as_parent = section->parent;
	//we go to the next sibling of child
	(*pointer_to_set_as_parent)->currentChild += 1;
}

void ct_exit_callback_children_number_computed(ct_model_t* model, Section** pointerToSetAsParent, Section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (!section->childrenNumberComputed) {
		section->childrenNumberComputed = true;
	}
	section->currentChild = 0;

	ct_update_snapshot_status(model->current_section, model->current_snapshot);

	model->current_snapshot = model->current_snapshot->parent;
}

void ct_exit_callback_reset_container(ct_model_t* model, Section** pointer_to_set_as_parent, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	child->currentChild = 0;
}

void ct_exit_callback_do_nothing(ct_model_t* model, Section** pointer_to_set_as_parent, Section* section) {

}

bool ct_get_access_when(ct_model_t* model, Section* section) {
	//section is the WHEN we're considering right now

	//we don't enter in this WHEN if we have already entered in another WHEN of the parent
	if (section->parent->alreadyFoundWhen) {
		return false;
	}

	//we don't access this WHEN if we have already completed it or if it generated a SECTION_SIGNAL
	if (section->status == SECTION_DONE || section->status == SECTION_SIGNAL_DETECTED) {
		return false;
	}

	return true;
}

void ct_callback_do_nothing(ct_model_t* model, Section* section) {

}

void ct_callback_entering_testcase(ct_model_t* model, Section* section) {
	ct_update_current_snapshot(model, model->current_section);
	ct_test_report_t* report = ct_init_test_report(model->current_snapshot);
	ct_add_tail_in_list(model->test_reports_list, report);
}

void ct_callback_entering_then(ct_model_t* model, Section* section) {
	ct_update_current_snapshot(model, section);
}

void ct_callback_entering_when(ct_model_t* model, Section* section) {
	section->parent->alreadyFoundWhen = true;

	ct_update_current_snapshot(model, section);
}

void ct_exit_callback_access_granted_testcase(ct_model_t* model, Section** pointer_to_set_as_parent, Section* section) {
	ct_test_report_t* report = ct_list_last_element(model->test_reports_list);
	SectionSnapshot* last_snapshot = model->current_snapshot;

	ct_update_snapshot_status(section, model->current_snapshot);
	ct_update_test_outcome(report, last_snapshot);

	//Resets the current_snapshot pointer to NULL to indicate the end of the test
	model->current_snapshot = NULL;
}

/**
 * This functions updates the current_snapshot. More precisely:
 *  - if current_snapshot is NULL it means we just started a new test, so it simply takes a section snapshot and sets it
 *  	as the current_snapshot
 *  - if current_snapshot is not NULL it means we are not in a new test, so we take a snapshot and adds it to the test report tree
 */

void ct_update_current_snapshot(ct_model_t* model, Section* section) {
	SectionSnapshot* snapshot = ct_init_section_snapshot(section);

	if (model->current_snapshot == NULL) {
		model->current_snapshot = snapshot;
	}
	else {
		model->current_snapshot = ct_add_snapshot_to_tree(snapshot, model->current_snapshot);
	}

}

void ct_signal_callback_do_nothing(int signal, Section* signaled_section, Section* section, Section* target_section) {

}
