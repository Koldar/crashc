/**
 * \file utility.h
 *
 * A file where lots of utility functions used throughout all the project are defined
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "macros.h"

#ifndef GRAPHIMPL
#	define GRAPHIMPL PredSuccGraph
#endif

#ifndef GRAPHEMPTYCONSTRUCTOR
#	define GRAPHEMPTYCONSTRUCTOR initPredSuccGraph
#endif

#ifndef GRAPHDESTRUCTOR
#	define GRAPHDESTRUCTOR destroyPredSuccGraph
#endif

/**
 * Static variable representing where we need to retrieve user commands
 *
 * The variable is used whenever a scanf is required or in general whenever we need to fetch data from user keyboard
 *
 * \ingroup globalVariables
 */
extern FILE* finput;
/**
 * Static variable representing where we need to print output
 *
 * The variable is used whenever we need to print out data to the console
 *
 * \ingroup globalVariables
 */
extern FILE* foutput;

/**
 * Represents a key-value pair
 */
typedef struct {
	///the key
	double key;
	///the value
	double value;
} DoublePair;

///**
// * show the message regarding a malloc failure
// */
//void errorMalloc();
//
///**
// * Show the message regarding 2 id should be equal but they are not
// */
//void errorIdMismatch(const unsigned int id1, const unsigned int id2);
//
///**
// * Show the message regarding a compilation failure of a regex
// */
//void errorRegexCompile(const char* regex);
//
///**
// * Show the message regarding a general failure of the regex
// *
// * @param[in] regex the regex failed
// * @param[in] line the test string you have match \c regex against
// */
//void errorRegexFailed(const char* regex, const char* line);
//
///**
// * Show the message regarding a file (eg. fioe not found, file not writeable and so on...)
// *
// * @param[in] filePath the path of the file
// */
//void __errorFile(const char* file, const int lineno, const char* filePath);
//
//#define errorFile(filePath) __errorFile(__FILE__, __LINE__, filePath)
//
///**
// * Show the message regarding that we couldn't fetch the number of nodes in the CSP
// */
//void errorCSPNodesNumber(const char* filename);
//
///**
// * Show when we couldn't find a node in the graph
// */
//void __errorNodeNotFound(const char* file, const int line, const int nodeId);
//
//#define errorNodeNotFound(nodeId) __errorNodeNotFound(__FILE__, __LINE__, nodeId)
//
///**
// * Show when we found a ::NOREL constraints in the graph.
// *
// * Graphs with at least one ::NOREL are inconsistent by default
// */
//void __errorNoRelFound(const char* file, const int line, const int sourceId, const int sinkId);
//
//#define errorNoRelFound(sourceId, sinkId) __errorNoRelFound(__FILE__, __LINE__, sourceId, sinkId)
//
///**
// * Show when we found a relation that we didn't expect
// */
//void errorUnexpectedRelation();
//
///**
// * Show when an assertion that we know should be true is false instead
// */
//void __errorUnexpectedMatch(const char* file, const int line);
//
//#define errorUnexpectedMatch() __errorUnexpectedMatch(__FILE__, __LINE__)

/**
 * @param[in] a the first number
 * @param[in] b the second number
 * @return the minimum between \c a and \c b
 */
int getMinimumBetweenInt(const int a, const int b);

/**
 * Prints something inside the file prefixing it with \c tabbed number of tabs
 *
 * @param[in] fout the file to write on
 * @param[in] tabbed the number of tabs to write into the file
 * @param[in] format printf argument
 * @param[in] varargs printf argument
 */
void fprintfTabbed(FILE* fout, int tabbed, const char* format, ...);

/**
 * get a random number
 *
 * @param[in] a the lower bound of the random number (it may be returned by the function)
 * @param[in] b the upper bound of the random numer;
 * @param[in] includeb true if want that the function may return b, false otherwise
 * @return an integer random nunber in \f$ [lowInclusive; highExclusive[ \f$
 */
int getRandom(const unsigned int a, const unsigned int b, const bool includeb);

/**
 * get a random number among those inside the array
 *
 * @param[in] n the size of the array \c array
 * @param[in] array the container of the numbers we need to choose from
 * @return one of the number inside \c array
 */
int getRandomFrom(int n, const int array[]);

/**
 * @param[in] str1 the first string to check
 * @apram[in] str2 the second string to check
 * @return
 * 	\li true if the 2 strings are the same;
 * 	\li false otherwise
 */
bool isStrEqual(char* str1, char* str2);

/**
 * @param[in] str1 the first string to check
 * @apram[in] str2 the second string to check
 * @return
 * 	\li true if the 2 strings are the same without considering the case of the letters;
 * 	\li false otherwise
 */
bool isStrCaseEqual(char* str1, char* str2);

/**
 * replace all the occurences of a character with another one in a string
 *
 * For example the string "abcbdb" will be "azczdz" if you replace "b" with "z". The input string
 * won't be modified at all
 *
 * @param[out] dest the string that will be modified. Nothing is made to ensure \c dest is large enough to contain the \c src modified.
 * 					At the end of the function \c dest will contain the output
 * @param[in] src the string where we're looking into
 * @param[in] toReplace the character we need to replace in \c src
 * @param[in] replacement the character to substitute to \c toReplace
 */
void strReplace(char* restrict dest, const char* restrict src, char toReplace, char replacement);

/**
 * Generate the specified path if it does not exist
 *
 * The function does nothing if the path already exists
 *
 * \post
 * 	\li \c path is present inside the filesystem
 *
 * @param[in] path the path to generate
 * @param[in] isFile false if \c path points to a directory, true if it points to a file
 */
void createPathIfNotExist(const char* path, bool isFile);

#endif /* UTILITY_H_ */
