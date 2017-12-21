/*
 * command_line.c
 *
 *  Created on: Oct 9, 2017
 *      Author: koldar
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include "command_line.h"
#include "tag.h"
#include "macros.h"

static struct option long_options[] = {
	{"include_tag",		required_argument,	0,	'i'},
	{"include_tags",	required_argument,	0,	'I'},
	{"exclude_tag",		required_argument,	0,	'e'},
	{"exclude_tags",	required_argument,	0,	'E'},
	{"help",			no_argument,		0,	'h'},
	{0,					0,					0,	0}
};

void printHelp(FILE* fout) {
	int i=0;
	struct option o;

	while (true) {
		o = long_options[i];
		if (o.name == NULL) {
			return;
		}

		fprintf(fout, "-%c, --%s: ", o.val, o.name);

		switch (o.val) {
		case 'h': {
			fprintf(fout, "Show this help");
			break;
		}
		case 'i': {
			fprintf(fout,
					"Adds a tagname in the \"Run Only\" set. "
					"A section is run only if it contains at least a tagname within such set. "
					"Can be defined multiple times.  If a tag is inside both 'e' and 'i', 'e' has the precedence."
			);
			break;
		}
		case 'e': {
			fprintf(fout,
					"Adds a tagname in the \"Exclude Test\" set. "
					"A section isn't run if it contains even one tagname within such set. "
					"Can be defined multiple times. If a tag is inside both 'e' and 'i', this set has the precedence."
			);
			break;
		}
		case 'I': {
			fprintf(fout,
					"Like \"i\", but allows to specify multiple tags. Tags are separated via \"%c\". "
					"Can be defined multiple times.",
					CC_TAGS_SEPARATOR
			);
			break;
		}
		case 'E': {
			fprintf(fout,
					"Like \"e\", but allows to specify multiple tags. Tags are separated via \"%c\". "
					"Can be defined multiple times.",
					CC_TAGS_SEPARATOR
			);
			break;
		}
		}

		fprintf(fout, "\n");

		i++;
	}
}

void parseCommandLineArguments(const int argc, char* const* args, char tagSeparator, tag_ht* runIfTags, tag_ht* excludeTags) {

	while (true) {
		/* getopt_long stores the option index here. */
		int option_index = 0;

		int optionId = getopt_long (argc, args, "i:I:e:E:", long_options, &option_index);

		/* Detect the end of the options. */
		if (optionId == -1)
			break;

		switch (optionId) {
		case 0: {
			//Only flags have the id set to 0.
			break;
		}
		case 'h': {
			printHelp(stdout);
			exit(0);
			break;
		}
		case 'i': {
			addTagNameInTagHashTable(runIfTags, optarg);
			break;
		}
		case 'e': {
			addTagNameInTagHashTable(excludeTags, optarg);
			break;
		}
		case 'I': {
			populateTagsHT(runIfTags, optarg, tagSeparator);
			break;
		}
		case 'E': {
			populateTagsHT(excludeTags, optarg, tagSeparator);
			break;
		}
		case '?': {
			/* getopt_long already printed an error message. */
			break;
		}
		default: {
			abort();
		}
		}
	}

//  ACTIVATE IF YOU WANT TO SEE WHAT TAGS HAVE BEEN STORED
//	ITERATE_VALUES_ON_HT(runIfTags, t, tag*) {
//		printf("run if tag: %s\n", t->name);
//	}
//	ITERATE_VALUES_ON_HT(excludeTags, t2, tag*) {
//		printf("exclude tag: %s\n", t2->name);
//	}

}
