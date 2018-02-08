/*
 * conversions.c
 *
 *  Created on: Sep 15, 2017
 *      Author: koldar
 */

#include "conversions.h"
#include <errno.h>
#include <stdlib.h>
#include "utility.h"
#include <limits.h>
#include <math.h>

bool convertString2Double(char* str, double* output) {
	char* endPtr;
	double val;

	errno = 0;
	val = strtod(str, &endPtr);

	//code inspired from "man 3 strtod"
	if ((errno == ERANGE && (val == HUGE_VALF || val == HUGE_VALL)) || (errno != 0 && val == 0)) {
		return false;
	}

	if (endPtr == str) {
		return false;
	}

	*output = val;
	return true;
}

bool convertString2Int(char* str, int* output) {
	long val;
	if (convertString2Long(str, &val)) {
		*output = val;
		return true;
	}
	return false;
}

bool convertString2Long(char* str, long* output) {
	char* endPtr;
	long val;

	errno = 0;
	val = strtol(str, &endPtr, 10);

	//code copied from "man 3 strtol"
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0)) {
		return false;
	}

	if (endPtr == str) {
		return false;
	}

	*output = val;
	return true;
}

bool convertString2Bool(char* str, bool* output) {
	if (isStrCaseEqual(str, "true") || isStrCaseEqual(str, "yes") || isStrCaseEqual(str, "ok") || isStrEqual(str, "1")) {
		*output = true;
		return true;
	}

	if (isStrCaseEqual(str, "false") || isStrCaseEqual(str, "no") || isStrCaseEqual(str, "ko") || isStrEqual(str, "0")) {
		*output = false;
		return true;
	}

	return false;
}
