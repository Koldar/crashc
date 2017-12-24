/**
 * @file
 *
 * Module for handling command line arguments
 *
 *
 * @author koldar
 * @date Oct 9, 2017
 */

#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <stdio.h>
#include "tag.h"

/**
 * Analyze the command line arguments and populates all the variables involved
 *
 * @param[in] argc \c argc from main
 * @param[in] args \c args from main
 * @param[in] tagSeparator the character used to separate tags in the command line parsing (eg. -I or -E). See \ref tags for further information
 * @param[inout] runIfTags hashtable from crashc model. Hashtable must be already initialized. The table will be populated at the end of the function
 * @param[inout] excludeTags hashtable from crashc model. Hashtable must be already initialized. The table will be populated at the end of the function
 */
void parseCommandLineArguments(const int argc, char* const* args, char tagSeparator, tag_ht* runIfTags, tag_ht* excludeTags);

/**
 * Print the help of the command line
 *
 * @param[inout] fout the file where to write the help on
 */
void printHelp(FILE* fout);

#endif /* COMMAND_LINE_H_ */
