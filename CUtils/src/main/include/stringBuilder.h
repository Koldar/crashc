/*
 * \file stringBuffer.h
 *
 * A naive porting of java's StringBuilder class
 *
 *  Created on: Feb 27, 2017
 *      Author: koldar
 */

#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_

#include <stdbool.h>

typedef struct StringBuilder {
	char* output;
	int length;
	int bufferSize;
	int resizeFactor;
} StringBuilder;

/**
 * @param[in] startString the string to initialize with the string builder. NULL if you don't want to intialize it with anything
 * @return the ::StringBuilder just initialized
 */
StringBuilder* initStringBuilder(const char* startString);

/**
 * like ::initStringBuilder with NULL argument
 *
 * @return the ::StringBuilder just initialized
 */
StringBuilder* initStringBuilderQuick();

/**
 * Appends a string into the builder
 *
 * The string \c string is copied by \b value inside the string buider;
 *
 * @param[inout] sb the string builder to alter
 * @param[in] template the string to append to the ::StringBuilder
 */
void appendToStringBuilder(StringBuilder* sb, const char* string);

/**
 * like ::appendToStringBuilder but it accepts a \c printf like var arguments
 *
 * @param[inout] sb the string builder to alter
 * @param[in] template the string to append to the ::StringBuilder
 * @param[in] ... the variable argument to add inside the \c template
 */
void appendTemplateToStringBuilder(StringBuilder* sb, const char* template, ...);

void appendStringToStringBuilder(StringBuilder* sb, const char* string);

void appendCharToStringBuilder(StringBuilder* sb, const char ch);

void appendIntToStringBuilder(StringBuilder* sb, const int i);

void appendFloatToStringBuilder(StringBuilder* sb, const float f);

void appendDoubleToStringBuilder(StringBuilder* sb, const double d);

void appendLongToStringBuilder(StringBuilder* sb, const long l);

char* getStringOfStringBuilder(StringBuilder* sb);

void clearStringBuilder(StringBuilder* sb);

void destroyStringBuilder(StringBuilder* sb);

#endif /* STRINGBUILDER_H_ */
