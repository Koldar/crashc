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
 * @param[in] tag_separator the character used to separate tags in the command line parsing (eg. -I or -E). See \ref tags for further information
 * @param[inout] run_tags hashtable from crashc model. Hashtable must be already initialized. The table will be populated at the end of the function
 * @param[inout] exclude_tags hashtable from crashc model. Hashtable must be already initialized. The table will be populated at the end of the function
 */
void ct_parse_args(const int argc, char* const* args, char tag_separator, tag_ht* run_tags, tag_ht* exclude_tags);

/**
 * Print the help of the command line
 *
 * @param[inout] fout the file where to write the help on
 */
void ct_print_help(FILE* fout);

#endif /* COMMAND_LINE_H_ */
