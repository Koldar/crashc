/*
 * command_line.h
 *
 *  Created on: Oct 9, 2017
 *      Author: koldar
 */

#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include "tag.h"

/**
 * Analyze the command line arguments and populates all the variables involved
 *
 * @param[in] argc \c argc from main
 * @param[in] args \c argv from main
 * @param[in] tagSeparator the character used to separate tags in the command line parsing (eg. -I or -E)
 * @param[inout] runIfTags hashtable from crashc model
 * @param[inout] excludeTags hashtable from crashc model
 */
void parseCommandLineArguments(const int argc, const char* args[], char tagSeparator, tag_ht* runIfTags, tag_ht* excludeTags);

/**
 * Print the help
 *
 * @param[inout] fout the file where to write the help on
 */
void printHelp(FILE* fout);

#endif /* COMMAND_LINE_H_ */
