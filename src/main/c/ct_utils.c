/*
 * ct_utils.c
 *
 *  Created on: Oct 31, 2017
 *      Author: noodles
 */

#include "ct_utils.h"

char* const ct_file_basename(char* const filename) {
	int i = 0;
	int last_slash_index = 0;
	char c = filename[0];

	while (c != '\0') {
		if (c == '/') {
			last_slash_index = i;
		}

		i++;
		c = filename[i];
	}

	return filename + (last_slash_index + 1);
}

