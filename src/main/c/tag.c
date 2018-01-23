/*
 * tag.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "tag.h"
#include "hashtable.h"
#include "errors.h"

struct ct_tag* ct_tag_init(const char* name) {
	struct ct_tag* ret_val = malloc(sizeof(struct ct_tag));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->name = strdup(name);

	return ret_val;
}

void ct_tag_destroy(struct ct_tag* tag) {
	free(tag->name);
	free(tag);
}

int ct_tag_compare(const struct ct_tag* tag1, const struct ct_tag* tag2) {
	if (tag1 == tag2) {
		return 0;
	}
	//TODO perform comparation(if the user wants it) with case insensitive as well!
	return strcmp(tag1->name, tag2->name);
}

bool ct_have_tag_set_intersection(const ct_tag_hashtable_o* tag_set1, const ct_tag_hashtable_o* tag_set2) {
	//TODO optimization: pick up the set with the least number of elements first. In this way the outer loop will be executed less

	ITERATE_VALUES_ON_HT(tag_set1, tag1, struct ct_tag*) {
		ITERATE_VALUES_ON_HT(tag_set2, tag2, struct ct_tag*) {
			if (ct_tag_compare(tag1, tag2) == 0) {
				return true;
			}
		}
	}

	return false;
}

void ct_tag_ht_put(ct_tag_hashtable_o* tag_hashtable, const char* name) {
	//TODO to improve performances you don't need to create a tag every time you see a name, There should be a tag pool containing
	//all the tags in the project. In this way, tag with the same name are created only once
	ct_ht_put(tag_hashtable, ct_string_hash(name), ct_tag_init(name));
}

int ct_string_hash(const char* str) {
	unsigned int hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

const char* ct_next_tag_in_string(const char* const str, char separator, char* characters_to_ignore, char* output) {
	char* output_index = NULL;
	char ch = '\0';
	const char* input = NULL;
	bool to_skip = false;

	//I don't use strtok because I don't want to use something with side effects
	input = str;
	output_index = &output[0];
	while (true) {
		ch = *input;

		//check if the character has to be skipped
		to_skip = false;
		for (int i=0; (characters_to_ignore[i])!='\0'; i++) {
			if (ch == characters_to_ignore[i]) {
				to_skip = true;
			}
		}

		if (!to_skip) {
			//add the character to the buffer
			if (ch == separator || ch == '\0') {
				*output_index = '\0';
				goto exit;
			} else {
				*output_index = ch;
			}
		}

		//go to the next character
		output_index += 1;
		input += 1;
	}

	exit:;
	if (*input != '\0') {
		input++;
	}
	return input;
}

void ct_tag_ht_populate(ct_tag_hashtable_o* output, const char* const tags, char separator) {
	char token[CT_BUFFER_SIZE];
	char* position_in_buffer = NULL;
	int token_id;

	const char* token_string = tags;

	while (*token_string != '\0') {
		token_string = ct_next_tag_in_string(token_string, separator, "", token);

		if (strlen(token) == 0) {
			return;
		}

		//add the fetched tag inside the section
		token_id = ct_string_hash(token);
		struct ct_tag* tag_with_token_id = ct_ht_get(output, token_id);
		if (tag_with_token_id == NULL) {
			tag_with_token_id = ct_tag_init(token);
			ct_ht_put(output, token_id, tag_with_token_id);
		}
	}
}
