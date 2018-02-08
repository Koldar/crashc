/*
 * \file regex.h
 *
 * Contains useful method to handle regular expression ina easy way with C
 *
 * Here's a normal use case for this library:
 *
 * \code
 *	char* string = "Hello wo123rld!";
 *	char** groups = NULL;
 *
 *	bool r = applyRegularExpression(string, "o\\([[:digit:]][[:digit:]]*\\)r", 1, &groups);
 *  if (r == true) {
 *		assert(strcmp(groups[1], "123") == 0);
 *		destroyGroupInfo(1, &groups);
 *	}
 * \endcode
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#ifndef REGULAREXPRESSION_H_
#define REGULAREXPRESSION_H_

#include <regex.h>
#include <stdbool.h>

/**
 * Convenience macro that can be used within regex representing a unsigned integer number
 */
#define RE_NUMBER "[[:digit:]][[:digit:]]*"
/**
 * Convenience macro that can be used within regex representing a space
 */
#define RE_SPACE "[[:blank:]][[:blank:]]*"
/**
 * Convenience macro that can be used within regex representing a alphabetic word (case unsensitive)
 */
#define RE_WORD "[[:alpha:]][[:alpha:]]*"

/**
 * Convenience macro that can be used within regex representing an alphanumeric word (case unsensitive)
 */
#define RE_ALPHAWORD "[[:alnum:]][[:alnum:]]*"

#define RE_ALPHANUMDOT "[\\.\\+a-zA-Z0-9][\\.\\+a-zA-Z0-9]*"

/**
 * Execute a regular expression
 *
 * After performing the function call, you can retrieve all the capturing groups via:
 * \code
 *  wholeStringMatched = groups[0];
 *  firstGroup = groups[1];
 *  ...
 *  destroyGroupInfo(groups);
 * \endcode
 *
 * @param[in] string the string to apply the regex on
 * @param[in] regex the regular expression. See the link <a href="http://www.regular-expressions.info/posix.html">here</a>
 * @param[in] groupSize the number of capturing groups inside the regular expression. A capturing group is one like <tt>\(ciao\)</tt>
 * @param[out] groups a double point of char that will be populated with all the capturing groups. The value is ignored if groupSize is 0
 * @return
 * 	\li true if string maatches the regex;
 * 	\li false otherwise. There can be several cause on this scenario: malformed regex, string doesn't match regex and so on
 */
bool applyRegularExpression(const char* restrict string, const char* restrict regex, int groupSize, char** groups[]);

bool isSatisfyingRegex(const char* restrict string, const char* restrict regex);

/**
 * Release from the memory the data inside groups ( see ::applyRegularExpression) generated by ::applyRegularExpression function
 *
 * @param[in] groupSize the number of capturing group inside the regex
 * @param[in] groups the array of string matched
 */
void destroyGroupInfo(int groupSize, char*** groups);

#endif /* REGULAREXPRESSION_H_ */