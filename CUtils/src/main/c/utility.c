/*
 * utility.c
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */


#include "utility.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <ctype.h>
#include <unistd.h>

#define ERROR_MALLOC "Malloc failed!"
#define ERROR_ID_MISMATCH "Id \"%d\" should be equal to \"%d\" but they are not! Exiting!"
#define ERROR_REGEX_COMPILE "Error in compiling regex %s"
#define ERROR_REGEX_FAILURE "The regex failed when matching \"%s\" with \"%s\""
#define ERROR_FILE "%s:%d An error occured while handling the file \"%s\""
#define ERROR_FILENAME "We couldn't fetch the number of nodes inside the CSP from the filename \"%s\""
#define ERROR_NODE_NOT_FOUND "%s:%d We couldn't find the node %d inside a graph"
#define ERROR_NOREL_FOUND "%s:%d There is a <no relation> within the graph %d -> %d. The graph is inconsistent by default"
#define ERROR_UNEXPECTED_RELATION "There is a relation that we didn't expect!"
#define ERROR_UNEXPECTED_MATCH "%s:%d We though something was always true, but it is not!"

FILE* finput = NULL;
FILE* foutput = NULL;


static void genericError(const char* str, const int errno, ...) {
	va_list argList;
	va_start(argList, errno);
	vfprintf(foutput, str, argList);
	fprintf(foutput, "\n");
	va_end(argList);
	exit(errno);
}

void errorMalloc() {
	genericError(ERROR_MALLOC, 1);
}

void errorIdMismatch(const unsigned int id1, const unsigned int id2) {
	genericError(ERROR_ID_MISMATCH, 2, id1, id2);
}

void errorRegexCompile(const char* regex) {
	genericError(ERROR_REGEX_COMPILE, 3, regex);
}

void errorRegexFailed(const char* regex, const char* line) {
	genericError(ERROR_REGEX_FAILURE, 4, regex, line);
}

void __errorFile(const char* file, const int line, const char* filePath) {
	genericError(ERROR_FILE, 5, file, line, filePath);
}

void errorCSPNodesNumber(const char* filename) {
	genericError(ERROR_FILENAME, 6, filename);
}

void __errorNodeNotFound(const char* file, const int line, const int nodeId) {
	genericError(ERROR_NODE_NOT_FOUND, 7, file, line, nodeId);
}

void __errorNoRelFound(const char* file, const int line, const int sourceId, const int sinkId) {
	genericError(ERROR_NOREL_FOUND, 8, file, line, sourceId, sinkId);
}

void errorUnexpectedRelation() {
	genericError(ERROR_UNEXPECTED_RELATION, 9);
}

void __errorUnexpectedMatch(const char* file, const int line) {
	genericError(ERROR_UNEXPECTED_MATCH, 10);
}


int getMinimumBetweenInt(const int a, const int b) {
	return a > b ? b : a;
}

void fprintfTabbed(FILE* fout, int tabbed, const char* format, ...) {
	va_list vaList;

	for (int i=0; i<tabbed; i++) {
		fprintf(fout, "\t");
	}
	va_start(vaList, format);
	vfprintf(fout, format, vaList);
	va_end(vaList);
}

//TODO remove. This function is deprecated
int getRandom(const unsigned int a, const unsigned int b, const bool includeb) {
	return a + (rand() % (b - a + (includeb ? 1 : 0)));
}

//TODO deprecated and replaced by getRandomValueInArray
int getRandomFrom(int n, const int array[]) {
	int place = getRandom(0, n, false);
	return array[place];
}

void strReplace(char* restrict dest, const char* restrict src, char toReplace, char replacement) {
	while(*src != '\0') {
		*dest = (*src == toReplace) ? replacement : *src;
		dest++;
		src++;
	}
	//copy null character
	*dest = *src;
}

bool isStrEqual(char* str1, char* str2) {
	if (str1 == str2) {
		return true;
	}

	while (true) {
		if (*str1 == 0 && *str2 == 0) {
			return true;
		}

		if (*str1 != *str2) {
			return false;
		}
		str1++;
		str2++;
	}
}

bool isStrCaseEqual(char* str1, char* str2) {
	if (str1 == str2) {
		return true;
	}

	while (true) {
		if (*str1 == 0 && *str2 == 0) {
			return true;
		}

		if (tolower(*str1) != tolower(*str2)) {
			return false;
		}
		str1++;
		str2++;
	}
}

void createPathIfNotExist(const char* path, bool isFile) {
	char mkdirBuffer[BUFFER_SIZE];
	char* path2 = strdup(path);
	char* path3 = path2;
	struct stat statOutput;

	if (stat(path, &statOutput) == 0) {
		free(path2);
		return;
	}
	path3 = isFile ? dirname(path2) : path2;
	sprintf(mkdirBuffer, "mkdir -p \"%s\"", path3);
	system(mkdirBuffer);
	free(path2);
}
