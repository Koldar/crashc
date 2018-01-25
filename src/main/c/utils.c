/*
 * ct_utils.c
 *
 *  Created on: Oct 31, 2017
 *      Author: noodles
 */

#include "utils.h"

enum time_format {
	TF_NANO    = 0,
	TF_MICRO   = 1,
	TF_MILLI   = 2,
	TF_SECONDS = 3
};

static enum time_format parse_time_format(const char* str);

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

struct timespec ct_get_time() {
	struct timespec ret_val;
	clock_gettime(CLOCK_MONOTONIC, &ret_val);
	return ret_val;
}

static enum time_format parse_time_format(const char* str) {
	switch (*str) {
	case 's': return TF_SECONDS;
	case 'm': return TF_MILLI;
	case 'u': return TF_MICRO;
	case 'n': return TF_NANO;
	}
}

long ct_compute_time_gap(struct timespec start, struct timespec end, const char* format_str)  {
	long sec, nano_sec, ret_val;
	enum time_format format = parse_time_format(format_str);

	sec = end.tv_sec - start.tv_sec;
	nano_sec = end.tv_nsec - start.tv_nsec;

	ret_val = ((sec * 1E9 + nano_sec) + nano_sec);
	for (int i = 0; i < format; i++) {
		ret_val /= 1000;
	}

	return ret_val;
}

