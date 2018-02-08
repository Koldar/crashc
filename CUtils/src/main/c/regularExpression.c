/*
 * regex.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#include "regularExpression.h"
#include <regex.h>
#include <stdbool.h>
#include "macros.h"
#include "log.h"
#include <string.h>

static int compileRegex(regex_t* regex, const char* regexString);
static int executeRegex(regex_t* regex, const char* regexString, const char* testString, regmatch_t* capturingGroups, int capturingGroupsSize);
static bool hasMatched(int reti);
static bool hasNotMatched(int reti);
static void destroyRegex(regex_t* regex);
static const char* getGroup(const char* testString, regmatch_t* groups, int groupNumber);

bool isSatisfyingRegex(const char* restrict string, const char* restrict regex) {
	regex_t regexSupport;
	regmatch_t groups[2];
	int reti;
	char* size;
	bool retVal;

	compileRegex(&regexSupport, regex);
	reti = executeRegex(&regexSupport, regex, string, NULL, 0);
	if (hasMatched(reti)) {
		retVal = true;
	} else {
		retVal = false;
	}
	destroyRegex(&regexSupport);
	return retVal;
}

bool applyRegularExpression(const char* restrict string, const char* restrict regex, int groupSize, char*** groups) {
	int reti;
	regex_t regexSupport;
	regmatch_t* groupsSupport;
	char size;
	bool retVal;
	reti = compileRegex(&regexSupport, regex);
	if (reti != 0) {
		retVal = false;
		return false;
	}

	groupsSupport = calloc(sizeof(regmatch_t), (groupSize + 1));
	if (groupsSupport == NULL) {
		ERROR_MALLOC();
	}
	reti = executeRegex(&regexSupport, regex, string, groupsSupport, groupSize + 1);
	if (hasNotMatched(reti)) {
		retVal = false;
		goto exit;
	}
	retVal = true;

	if (groupSize == 0) {
		goto exit;
	}

	char** arrayOfString = NULL;
	arrayOfString = calloc(sizeof(char*), groupSize + 1);
	if (arrayOfString == NULL) {
		ERROR_MALLOC();
	}

	for (int i=0; i<(groupSize+1); i++) {
		arrayOfString[i] = getGroup(string, groupsSupport, i);
	}
	*groups = arrayOfString;

	exit:
	free(groupsSupport);
	destroyRegex(&regexSupport);
	return retVal;
}

void destroyGroupInfo(int groupSize, char*** groups) {
	char** arrayOfString = *groups;
	groupSize += 1; //there is the whole mathced string as well
	for (int i=0; i<groupSize; i++) {
		free(arrayOfString[i]);
	}
	free(arrayOfString);
}

/**
 * Compile a regex
 *
 * if the regex fails, the function will automatically close the application
 *
 * @param[in] regex an empty structure. Create one on the stack
 * @param[in] regex a string representing the regex <b>according to POSIX</b>
 * @return an int useful to detect errors
 */
static int compileRegex(regex_t* regex, const char* regexString) {
	int reti;
	reti = regcomp(regex,regexString , 0);
	if (reti) {
		ERROR_CONSTRUCTION_FAILED("regex", regexString);
	}
	return reti;
}

/**
 * Executes a regex
 *
 * If a general error occurs, the function will automatically close the application
 *
 * @param[in] regex the regex structure passed to ::compileRegex
 * @param[in] regexString the regex string passed to ::compileRegex. Useful only for error messages
 * @param[in] testString the string you want to test against the regex
 * @param[out] capturingGroups an array in the stack where the capturing groups can be stored. Set it NULL if our regex does not have any capturing group
 * @param[in] capturingGroupsSize the number of capturing groups in the regex
 * @return an int useful to understand the result of the regex. See ::hasMatched or ::hasNotMatched
 */
static int executeRegex(regex_t* regex, const char* regexString, const char* testString, regmatch_t* capturingGroups, int capturingGroupsSize) {
	int reti = regexec(regex, testString, capturingGroupsSize, capturingGroups, 0);
	if (reti && (reti != REG_NOMATCH)) {
		ERROR_APPLICATION_FAILED("regex", regexString, "string", testString);
	}
	return reti;
}

/**
 * Check if the regex has matched
 *
 * @param[in] reti the output of the function ::executeRegex
 * @return true if the regex has matched, false otherwise
 */
static bool hasMatched(int reti) {
	return !reti;
}

/**
 * Check if the regex has not matched
 *
 * @param[in] reti the output of the function ::executeRegex
 * @return false if the regex has matched, true otherwise
 */
static bool hasNotMatched(int reti) {
	return reti || (reti == REG_NOMATCH);
}

/**
 * Clean up regex structure
 *
 * @param[in] the regex structure passed to ::compileRegex
 */
static void destroyRegex(regex_t* regex) {
	regfree(regex);
}

/**
 * fetch a group value from the \c testString
 *
 * Please note that capturing group nnumber 0 is the whole match string
 *
 * \attention
 * please call ::releaseGroupValue on the return value to avoid memory leaks
 *
 * @param[in] testString the string we're testing
 * @param[in] groups the variable populated by ::executeRegex
 * @param[in] groupNumber the size of the \c groups array
 * @return a string representing the the group value
 */
static const char* getGroup(const char* testString, regmatch_t* groups, int groupNumber) {
	int start = groups[groupNumber].rm_so;
	int end = groups[groupNumber].rm_eo;
	char* retVal = (char*) malloc(end - start + 1);
	if (retVal == NULL) {
		ERROR_MALLOC();
	}


	debug("group  start=\"%c\"(%d) end=\"%c\"(%d) groupLength=%d sourceString=\"%s\" ", testString[start], start, testString[end], end, end-start, testString);
	memcpy( retVal, &(testString[start]), end-start);
	retVal[end-start] = '\0';
	return retVal;
}


