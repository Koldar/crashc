/*
 * section.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

#include "section.h"
#include "macros.h"

/**
 * The id the next section created with ::ct_section_init will have.
 *
 * It starts from 1 and not from 0 because 0 is **manually** set to the root section, created **statically**.
 * See ::rootSection
 */
static int next_section_id = 1;

static int populate_section_status_buffer_string(const int ss, int space_left, char* buffer);
static void compute_section_tree_dot_file(FILE* fout, const struct ct_section* section);
static void update_section_tree_dot_file_with_section_info(FILE* fout, const struct ct_section* section);
static void update_section_tree_dot_file_with_section_edges(FILE* fout, const struct ct_section* section);

struct ct_section* ct_section_add_child(struct ct_section* restrict to_add, struct ct_section* restrict parent) {
	struct ct_section* list = NULL;

	to_add->parent = parent;

	list = parent->first_child;
	if (list == NULL) {
		parent->first_child = to_add;
		return to_add;
	}
	while (true) {
		if (list->next_sibling != NULL) {
			list = list->next_sibling;
		} else {
			list->next_sibling = to_add;
			return to_add;
		}
	}
}

struct ct_section* ct_section_get_child(const struct ct_section* parent, int n) {
	struct ct_section* list = parent->first_child;
	while(true) {
		if ((n == 0) || (list == NULL)) {
			return list;
		} else {
			list = list->next_sibling;
			n--;
		}
	}
}

struct ct_section* ct_section_init(enum ct_section_type type, const char* description, const char* tags) {
	struct ct_section* ret_val = malloc(sizeof(struct ct_section));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->id = next_section_id;
	next_section_id += 1;
	ret_val->access_granted = false;
	ret_val->already_found_when = false;
	ret_val->children_number = 0;
	ret_val->children_number_known = false;
	ret_val->status = CT_SECTION_UNVISITED;
	ret_val->current_child = 0;
	ret_val->description = strdup(description);
	ret_val->first_child = NULL;
	ret_val->type = type;
	ret_val->times_encountered = 0;
	ret_val->loop1 = false;
	ret_val->loop2 = false;
	ret_val->next_sibling = NULL;
	ret_val->parent = NULL;
	ret_val->tags = ct_ht_init();

	ct_tag_ht_populate(ret_val->tags, tags, CT_TAGS_SEPARATOR);

	return ret_val;
}

void ct_section_destroy(struct ct_section* section) {
	if (section->first_child != NULL) {
		ct_section_destroy(section->first_child);
	}
	if (section->next_sibling != NULL) {
		ct_section_destroy(section->next_sibling);
	}
	if (section->parent == NULL) {
		//root section shouldn't be removed at all
		return;
	}

	ct_ht_destroy_with_elements(section->tags, (ct_destroyer_c)(ct_tag_destroy));
	free((void*) section->description);
	free((void*) section);
}

bool ct_section_still_discovering_children(const struct ct_section* section) {
	return !section->children_number_known;
}

void ct_section_set_signaled(struct ct_section* section) {
	section->status = CT_SECTION_SIGNAL_DETECTED;
}

bool ct_section_is_signaled(const struct ct_section* section) {
	return section->status == CT_SECTION_SIGNAL_DETECTED;
}

void ct_section_set_executed(struct ct_section* section) {
	section->status = CT_SECTION_PARTIALLY_VISITED;
}

void ct_section_set_done(struct ct_section* section) {
	section->status = CT_SECTION_FULLY_VISITED;
}

void ct_section_set_skipped(struct ct_section* section) {
	section->status = CT_SECTION_SKIPPED_BY_TAG;
}

bool ct_section_still_needs_execution(struct ct_section* section) {
	if (section->status == CT_SECTION_UNVISITED || section->status == CT_SECTION_PARTIALLY_VISITED) {
		return true;
	}
	else {
		return false;
	}
}

int ct_section_get_level(const struct ct_section* section) {
	int level = 0;
	const struct ct_section* tmp = section;

	while (tmp->parent != NULL) {
		level++;
		tmp = tmp->parent;
	}

	return level;
}

bool ct_section_is_fully_visited(struct ct_section* section) {
	if (section->children_number == 0) {
		return true;
	}
	else {
		struct ct_section* next_child = section->first_child;

		while (next_child != NULL) {
			if (ct_section_still_needs_execution(next_child)) {
				return false;
			}
			next_child = next_child->next_sibling;
		}
		return true;
	}
}

void ct_section_draw_tree(const struct ct_section* section, const char* format, ...) {
	//create image name
	va_list ap;
	char image_template[CT_BUFFER_SIZE];
	va_start(ap, format);
	vsnprintf(image_template, CT_BUFFER_SIZE, format, ap);
	va_end(ap);

	char dot_filename[CT_BUFFER_SIZE];
	strncpy(dot_filename, image_template, CT_BUFFER_SIZE);
	//TODO here we need to make sure ".dot" can be put within the buffer
	strcat(dot_filename, ".dot");

	FILE* dot_file = fopen(dot_filename, "w");
	if (dot_file == NULL) {
		fprintf(stderr, "Can't create dot_file %s! Exiting...", dot_filename);
		//TODO create a new error like FILE_ERROR_CALLBACK
		exit(1);
	}
	compute_section_tree_dot_file(dot_file, section);
	fclose(dot_file);

	char png_filename[CT_BUFFER_SIZE];
	strncpy(png_filename, image_template, CT_BUFFER_SIZE);
	//TODO here we need to make sure ".dot" can be put within the buffer
	strcat(png_filename, ".png");

	char command[CT_BUFFER_SIZE];
	snprintf(command, CT_BUFFER_SIZE, "dot -Tpng -o%s %s", png_filename, dot_filename);
	system(command);
	unlink(dot_filename);
}

static int populate_section_status_buffer_string(const int ss, int space_left, char* buffer) {
	int i = 0;
	//TODO renamed DONE, EXECUTED and UNEXECUTED to fit section status renaming
	switch (ss) {
	case CT_SECTION_FULLY_VISITED: { i += snprintf(&buffer[i], space_left - i, "DONE"); break; }
	case CT_SECTION_PARTIALLY_VISITED: { i += snprintf(&buffer[i], space_left - i, "EXECUTED"); break; }
	case CT_SECTION_UNVISITED: { i += snprintf(&buffer[i], space_left - i, "UNEXECUTED"); break; }
	case CT_SECTION_SIGNAL_DETECTED: { i += snprintf(&buffer[i], space_left - i, "SIGNAL DETECTED"); break; }
	default: {
		//TODO create a new error type, like SWITCH_DEFAULT_ERROR_CALLBACK
		fprintf(stderr, "invalid section status %d\n", ss);
		exit(1);
	}
	}

	return i;
}

static void compute_section_tree_dot_file(FILE* fout, const struct ct_section* section) {
	fprintf(fout, "digraph {\n");

	// ******************* define nodes **********************
	update_section_tree_dot_file_with_section_info(fout, section);
	// ******************* create edges **********************
	update_section_tree_dot_file_with_section_edges(fout, section);

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
static void update_section_tree_dot_file_with_section_info(FILE* fout, const struct ct_section* section) {
	char buffer[CT_BUFFER_SIZE];
	populate_section_status_buffer_string(section->status, CT_BUFFER_SIZE, buffer);
	fprintf(fout, "\tSECTION%05d [label=\"%s\\nlevel=%d;\\nstatus=%s\", shape=\"box\"];\n", section->id, section->description, ct_section_get_level(section), buffer);

	if (section->first_child == NULL) {
		return;
	}

	update_section_tree_dot_file_with_section_info(fout, section->first_child);

	struct ct_section* tmp = section->first_child->next_sibling;
	while (tmp != NULL) {
		update_section_tree_dot_file_with_section_info(fout, tmp);
		tmp = tmp->next_sibling;
	}

}

/**
 * Adds inside a "*.dot" file all the edges between the nodes
 *
 * \pre
 * 	\li \c fout open in "w" mode;
 * 	\li ::update_section_tree_dot_file_with_section_info called;
 * \post
 * 	\li \c fout contains all the edges;
 *
 * @param[inout] fout the file to write on
 * @param[in] section the section where we need to start adding edges from
 */
static void update_section_tree_dot_file_with_section_edges(FILE* fout, const struct ct_section* section) {
	if (section->first_child == NULL) {
		return;
	}

	update_section_tree_dot_file_with_section_edges(fout, section->first_child);

	fprintf(fout, "\tSECTION%05d -> SECTION%05d;\n", section->id, section->first_child->id);
	struct ct_section* tmp = section->first_child->next_sibling;
	while (tmp != NULL) {
		fprintf(fout, "SECTION%05d -> SECTION%05d;\n", section->id, tmp->id);
		tmp = tmp->next_sibling;
	}

}
