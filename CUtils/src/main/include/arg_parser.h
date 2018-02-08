/*
 * \file arg_parser.h
 *
 * Easy wrapper on getopt_long
 *
 * Thanks to this module, you can avoid knowing how getopt works and focus on your CLI arguments section
 *
 *  Created on: Jul 20, 2017
 *      Author: koldar
 */

#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

#include <stdbool.h>
#include "macros.h"

#ifndef CLIPARSER_MAX_OPTIONS_NUMBER
#	define CLIPARSER_MAX_OPTIONS_NUMBER 100
#endif

#ifndef CLIPARSER_MAX_OPTIONS_BUFFER_SIZE
#	define CLIPARSER_MAX_OPTIONS_BUFFER_SIZE (3* CLIPARSER_MAX_OPTIONS_NUMBER)
#endif

typedef enum {
	CLI_ARG_INT,
	CLI_ARG_LONG,
	CLI_ARG_BOOL,
	CLI_ARG_STRING,
	CLI_ARG_FLOAT,
	CLI_ARG_DOUBLE
} cli_arg_type;

typedef struct CLIParser CLIParser;

CLIParser* initCLIParser();

void destroyCLIParser(CLIParser* p);

void setBriefDescriptionInCLIParser(CLIParser* p, const char* brief);

void setLongDescriptionInCLIParser(CLIParser* p, const char* description);

bool parseCLIArgumentsWithCLIParser(CLIParser* p, const int argc, const char* args[]);

void addFlagToCLIParser(CLIParser* p, char briefName, const char* longName, bool defaultValue, bool* whereToStore, const char* description);

void addBoolOptionToCLIParser(CLIParser* p, char briefName, const char* longName, bool defaultValue, bool* whereToStore, const char* description);

void addOptionalIntOptionToCLIParser(CLIParser* p, char briefName, const char* longName, int defaultValue, int* whereToStore, const char* description);

void addOptionalLongOptionToCLIParser(CLIParser* p, char briefName, const char* longName, long defaultValue, long* whereToStore, const char* description);

/**
 *
 * Declare a command that  may be used in the command line arguments multiple times
 *
 * For example:
 *
 * ```
 * gcc -fno-strict-aliasing -ffast-math
 * ```
 *
 * <tt>-f</tt> is a command that may appear multiple times inside the terminal
 *
 * @param[inout] p the parser structure to update
 * @param[in] brief the char identifying the command. This is also the character used in the terminal to briefly refer to the command.
 * 				For example if this value is <tt>'v'</tt>, you can call this command on the terminal via <tt>-v</tt>. **The character must be unique in the parser** \c p!
 * @param[in] longName the long version of \c brief. This is also the string you use to call this command in the terminal. For example, if
 * 				this value is <tt>"verbose"</tt>, you can call the command via <tt>--verbose</tt>;
 * @param[in] isOptional if true, this command can be avoided. If that's the case, the default value is placed automatically in \c whereToStore
 * @param[in] defaultValue the default value to use if the option is not specified and \c isOptional is true. Ignored if \c isOptional is false;
 * @param[in] whereToStoreSize the size of the array \c whereToStore
 * @param[in] whereToStore a pointer to an array of `char*` (aka array of strings) where we will put the values fetched by the command. After the parsing, this array
 * 					will contain what has been parsed from the command line
 * @param[in] a description of the command
 */
void addOptionalStringOptionToCLIParser(CLIParser* p, char briefName, const char* longName, const char* defaultValue, char* whereToStore, const char* description);

void addOptionalFloatOptionToCLIParser(CLIParser* p, char briefName, const char* longName, float defaultValue, float* whereToStore, const char* description);

void addOptionalDoubleOptionToCLIParser(CLIParser* p, char briefName, const char* longName, double defaultValue, double* whereToStore, const char* description);

void addOptionalStringOptionListToCLIParser(CLIParser* p, char brief, const char* longName, int defaultValueSize, const char* defaultValue[], int whereToStoreSize, char* whereToStore[], const char* description);

void addRequiredIntOptionToCLIParser(CLIParser* p, char briefName, const char* longName, int* whereToStore, const char* description);

void addRequiredLongOptionToCLIParser(CLIParser* p, char briefName, const char* longName, long* whereToStore, const char* description);

/**
 * Declare a required string argument for your parser
 *
 * @code
 * char* outputFileName;
 * ...
 * addRequiredStringOptionToCLIParser(p, 'o', "output_file", &outputFileName, "the name of the file to produce");
 * @encode
 *
 * @param[inout] p the parser structure to update
 * @param[in] brief the char identifying the command. This is also the character used in the terminal to briefly refer to the command.
 * 				For example if this value is <tt>'v'</tt>, you can call this command on the terminal via <tt>-v</tt>. **The character must be unique in the parser** \c p!
 * @param[in] longName the long version of \c brief. This is also the string you use to call this command in the terminal. For example, if
 * 				this value is <tt>"verbose"</tt>, you can call the command via <tt>--verbose</tt>;
 * @param[in] whereToStore a pointer to a `char*`  where we will put the values fetched by the command. After the parsing, that pointer will
 * 				contain the string you've requested
 * @param[in] description a description of the command
 */
void addRequiredStringOptionToCLIParser(CLIParser* p, char briefName, const char* longName, char* whereToStore, const char* description);

void addRequiredFloatOptionToCLIParser(CLIParser* p, char briefName, const char* longName, float* whereToStore, const char* description);

void addRequiredDoubleOptionToCLIParser(CLIParser* p, char briefName, const char* longName, double* whereToStore, const char* description);

void addRequiredStringOptionListToCLIParser(CLIParser* p, char brief, const char* longName, int whereToStoreSize, char* whereToStore[], const char* description);

/**
 * Check the number of times the given option has been displayed in the command line
 *
 * This function is useful for options that may be used multiple times: if you want to cycle through the array where those options
 * are stored, you need to know how many of those elements are actually used. This function can help you in that
 *
 * Use this function **after** you have called ::parseCLIArgumentsWithCLIParser
 *
 * @param[in] p the parse to analyze
 * @param[in] optionIdentifier the character uniquely identifying the option you want
 * @return
 * 	\li 0 if the option has not been registered at all;
 * 	\li 1 if the option can be used at most 1 time;
 * 	\li the number of times the option \c optionIdentifier has been used inside the CLI
 */
int getNumberOfOptionsParsedWithCLIParser(const CLIParser* p, char optionIdentifier);

void* getWhereToStoreValueOfOptionInCLIParser(const CLIParser* p, char optionIdentifier);

/**
 * Parse a parameter string from command line
 *
 * An example fo command line parameter may be:
 *
 * <tt>OHSAT.sigma1Algorithm=IPASAT</tt>
 *
 * @param[in] parameter the parameter value
 * @param[out] optionName the place where to store the option name section of the parameter value. In the above example it weould be "OHSAT.sigma1Algorithm"
 * @param[out] optionValue the place where to store the option value section of the parameter value. In the above example it weould be "IPASAT"
 */
void parseOptionStringFromCLIParser(const char* parameter, char** optionName, char** optionValue);

/**
 * Loop over all the options declared by the flag \c option
 *
 * For example, if you have:
 *
 * <p><tt>--parameter="OHSAT.sigma1Algorithm=IPASAT" --parameter="OHSAT.improvement=true"</tt></p>
 *
 * this macro will allow you to cycle through all the parameters entries and fetch the values.
 *
 * \note
 * This macro works only with options list of strings.
 *
 * @param[in] p ::CLIParser to use
 * @param[in] option the flag identifiying the option. For exampe 'v' or 'm'
 * @param[in] optionName a variable name that will be used to allow you to get the left member of the parameter
 * @param[in] optionValue a variable name that will be used to allow you to get the right member of the parameter
 */
#define HANDLE_STRING_OPTION_LIST_IN_CLI_PARSER(p, value, optionName, optionValue) \
		CLIParser* UV(parser) = p; \
		char UV(option) = value; \
		int UV(optionSize) = getNumberOfOptionsParsedWithCLIParser(UV(parser), UV(option)); \
		char** UV(whereToStore) = (char**)getWhereToStoreValueOfOptionInCLIParser(UV(parser), UV(option)); \
		for (int UV(i)=0, UV(access)=true; (UV(access)=true, UV(i)<UV(optionSize)); UV(i)++) \
			for (char* optionName, *optionValue; UV(access); UV(access)=false) \
				for (parseOptionStringFromCLIParser(UV(whereToStore)[UV(i)], &optionName, &optionValue); UV(access); UV(access)=false)



#endif /* ARG_PARSER_H_ */
