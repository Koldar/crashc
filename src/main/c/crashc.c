#include "utils.h"
#include "crashc.h"
#include "main_model.h"
#include "list.h"

void ct_update_test_array(struct ct_model* model, ct_test_c func) {
	model->tests_array[model->suites_array_index] = func;
	model->suites_array_index++;
}

bool ct_run_once_check_access(struct ct_model* model, struct ct_section* section, ct_access_c cs, ct_enter_c callback, const ct_tag_hashtable_o* restrict run_tags, const ct_tag_hashtable_o* restrict exclude_tags) {
	if (!section->loop2) {
		return false;
	}

	//TODO here we need  to replace the parameter runOnlyWithTags and excludeIfTags with a pointer of the global model
	//check if the section we're dealing with is compliant with the context tags
	if (!ct_ht_is_empty(exclude_tags)) {
		if (ct_have_tag_set_intersection(section->tags, exclude_tags)) {
			section->tag_access_granted = false;
			section->access_granted = false;
			ct_section_set_skipped(section);
			return false;
		}
	}

	if (!ct_ht_is_empty(run_tags)) {
		if (!ct_have_tag_set_intersection(section->tags, run_tags)) {
			section->tag_access_granted = false;
			section->access_granted = false;
			ct_section_set_skipped(section);
			return false;
		}
	}
	section->tag_access_granted = true;

	section->access_granted = cs(model, section);
	if (section->access_granted) {
		callback(model, section);
	}
	return section->access_granted;
}

bool ct_run_once_final_work(struct ct_model* model, struct ct_section* section, struct ct_section** pointer_to_set_as_parent, ct_exit_c callback, ct_exit_c access_granted_callback, ct_exit_c access_denied_callback) {
	if (section->loop1) {
		return true;
	}
	//callback is always executed and it can (and often will) change pointerToSetAsParent and child pointers (since they point to the same structure).
	//in order to use the previously pointed structure we copy the child pointer. As for pointerToSetAsParent, we can do nothing since it will be changed nonetheless
	struct ct_section* _child = section;
	callback(model, pointer_to_set_as_parent, section);
	if (section->access_granted) {
		//ct_section_set_executed(section);
		//If we executed the section we check if this execution made the section
		//fully visited and update its status consequently
		if (ct_section_is_fully_visited(section)) {
			ct_section_set_done(section);
		}

		//We reset the WHEN found tag
		section->already_found_when = false;

		access_granted_callback(model, pointer_to_set_as_parent, _child);
	} else {
		access_denied_callback(model, pointer_to_set_as_parent, _child);
	}
	return false;
}

/** If we are still computing the children of the given parent section the function
 *  creates a new section with the given description, tags and adds it to its
 *  parent section and returns a pointer to the new section.
 *  Else, the function returns a pointer to the current_child of the given section.
 *
 *  @param[in] type the kind of section we're getting.
 *  @see ::ct_section_type
 */
struct ct_section* ct_fetch_section(struct ct_section* parent, enum ct_section_type type, const char* description, const char* tags) {
	if (ct_section_still_discovering_children(parent)) {
		parent->children_number += 1;
		return ct_section_add_child(ct_section_init(type, description, tags), parent);
	}
	return ct_section_get_child(parent, parent->current_child);
}

void ct_reset_section_after_jump(struct ct_model* model, struct ct_section* const jump_source_section, struct ct_section* const testcase_section) {
	model->current_section = testcase_section;
}

bool ct_always_enter(struct ct_model* model, struct ct_section* section) {
	return true;
}

void ct_exit_callback_next_sibling(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section) {
	//we finish a section. we return to the parent
	*pointer_to_set_as_parent = section->parent;
	//we go to the next sibling of child
	(*pointer_to_set_as_parent)->current_child += 1;
}

void ct_exit_callback_children_number_computed(struct ct_model* model, struct ct_section** pointerToSetAsParent, struct ct_section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (!section->children_number_known) {
		section->children_number_known = true;
	}
	section->current_child = 0;

	ct_update_snapshot_status(model->current_section, model->current_snapshot);

	model->current_snapshot = model->current_snapshot->parent;
}

void ct_exit_callback_reset_container(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->children_number_known = true;
	child->current_child = 0;
}

void ct_exit_callback_do_nothing(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section) {

}

bool ct_get_access_when(struct ct_model* model, struct ct_section* section) {
	//section is the WHEN we're considering right now

	//we don't enter in this WHEN if we have already entered in another WHEN of the parent
	if (section->parent->already_found_when) {
		return false;
	}

	//we don't access this WHEN if we have already completed it or if it generated a SECTION_SIGNAL
	if (section->status == CT_SECTION_FULLY_VISITED || section->status == CT_SECTION_SIGNAL_DETECTED) {
		return false;
	}

	return true;
}

void ct_callback_do_nothing(struct ct_model* model, struct ct_section* section) {

}

void ct_callback_entering_testcase(struct ct_model* model, struct ct_section* section) {
	ct_update_current_snapshot(model, model->current_section);
	struct ct_test_report* report = ct_init_test_report(model->current_snapshot);
	ct_list_add_tail(model->test_reports_list, report);
}

void ct_callback_entering_then(struct ct_model* model, struct ct_section* section) {
	ct_update_current_snapshot(model, section);
}

void ct_callback_entering_when(struct ct_model* model, struct ct_section* section) {
	section->parent->already_found_when = true;

	ct_update_current_snapshot(model, section);
}

void ct_exit_callback_access_granted_testcase(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section) {
	struct ct_test_report* report = ct_list_tail(model->test_reports_list);
	struct ct_snapshot* last_snapshot = model->current_snapshot;

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

void ct_update_current_snapshot(struct ct_model* model, struct ct_section* section) {
	struct ct_snapshot* snapshot = ct_init_section_snapshot(section);

	if (model->current_snapshot == NULL) {
		model->current_snapshot = snapshot;
	}
	else {
		model->current_snapshot = ct_add_snapshot_to_tree(snapshot, model->current_snapshot);
	}

}

void ct_signal_callback_do_nothing(int signal, struct ct_section* signaled_section, struct ct_section* section, struct ct_section* target_section) {

}

void ct_set_crashc_teardown(ct_teardown_c f) {
	ct_model->ct_teardown = f;
}
