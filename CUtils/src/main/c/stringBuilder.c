/*
 * stringBuilder.c
 *
 *  Created on: Feb 27, 2017
 *      Author: koldar
 */

//used to exploit vasprintf
#define _GNU_SOURCE

#include "stringBuilder.h"
#include "utility.h"
#include "log.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void resizeOutput(StringBuilder* sb);

StringBuilder* initStringBuilder(const char* startString) {
	StringBuilder* sb = malloc(sizeof(StringBuilder));
	if (sb == NULL) {
		ERROR_MALLOC();
	}

	sb->output = NULL;
	sb->length = 0;
	sb->bufferSize = 0;
	sb->resizeFactor = 100;

	if (startString != NULL) {
		appendToStringBuilder(sb, startString);
	}

	return sb;
}

StringBuilder* initStringBuilderQuick() {
	return initStringBuilder(NULL);
}

void appendToStringBuilder(StringBuilder* sb, const char* str) {
	int strLength = strlen(str);
	while (sb->bufferSize < (sb->length + strLength + 1)) {
		resizeOutput(sb);
	}

	strcpy(sb->output + sb->length, str);
	sb->length += strLength;
}

void appendTemplateToStringBuilder(StringBuilder* sb, const char* template, ...) {
	char* buffer;

	va_list vaList;
	va_start(vaList, template);
	vasprintf(&buffer, template, vaList);
	va_end(vaList);

	appendToStringBuilder(sb, buffer);

	free(buffer);
}

void appendStringToStringBuilder(StringBuilder* sb, const char* string) {
	appendToStringBuilder(sb, string);
}

void appendCharToStringBuilder(StringBuilder* sb, const char ch) {
	appendTemplateToStringBuilder(sb, "%c", ch);
}

void appendIntToStringBuilder(StringBuilder* sb, const int i) {
	appendTemplateToStringBuilder(sb, "%d", i);
}

void appendFloatToStringBuilder(StringBuilder* sb, const float f) {
	appendTemplateToStringBuilder(sb, "%f", f);
}

void appendDoubleToStringBuilder(StringBuilder* sb, const double d) {
	appendTemplateToStringBuilder(sb, "%f", d);
}

void appendLongToStringBuilder(StringBuilder* sb, const long l) {
	appendTemplateToStringBuilder(sb, "%ld", l);
}

char* getStringOfStringBuilder(StringBuilder* sb) {
	return sb->length > 0 ? sb->output : "";
}

void clearStringBuilder(StringBuilder* sb) {
	if (sb->output != NULL) {
		free(sb->output);
	}
	sb->output = NULL;
	sb->length = 0;
	sb->bufferSize = 0;
}

void destroyStringBuilder(StringBuilder* sb) {
	clearStringBuilder(sb);
	free(sb);
}

static void resizeOutput(StringBuilder* sb) {
	char* newStr = calloc((sb->bufferSize + sb->resizeFactor), sizeof(char));
	if (newStr == NULL) {
		ERROR_MALLOC();
	}

	debug("output is \"%p\"", sb->output);
	if (sb->output != NULL) {
		debug("copying\"%s\" into newStr", sb->output);
		strcpy(newStr, sb->output);
		free(sb->output);
	}
	sb->output = newStr;
	sb->bufferSize += sb->resizeFactor;
}

