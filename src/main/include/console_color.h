/**
 * @file
 *
 * A small header containing ansi code for coloring the console
 *
 * We didn't use an external file to avoid adding a dependency in the the project
 *
 * If your console supports it, you can use this module like this:
 *
 * @code
 * printf("Hello %s%s%s!\n", ANSI_COLOR_RED, "George", ANSI_COLOR_RESET);
 * @endcode
 *
 * Be sure to call ::ANSI_COLOR_RESET whenever you use any macro of this header
 *
 * @author koldar
 * @date Feb 24, 2017
 */

#ifndef CONSOLE_COLOR_H_
#define CONSOLE_COLOR_H_

/**
 * red foreground, normal text
 */
#define ANSI_COLOR_RED		"\x1b[31m"
/**
 * green foreground, normal text
 */
#define ANSI_COLOR_GREEN	"\x1b[32m"
/**
 * yellow foreground, normal text
 */
#define ANSI_COLOR_YELLOW	"\x1b[33m"
/**
 * blue foreground, normal text
 */
#define ANSI_COLOR_BLUE		"\x1b[34m"
/**
 * magenta foreground, normal text
 */
#define ANSI_COLOR_MAGENTA	"\x1b[35m"
/**
 * red foreground, normal text
 */
#define ANSI_COLOR_CYAN		"\x1b[36m"
/**
 * reset console printing coloring back to normal
 */
#define ANSI_COLOR_RESET 	"\x1b[0m"

/**
 * An alias for a string.
 *
 * Useful when you want to convey the information the string is actually a color console
 */
typedef const char* ConsoleColor;

#endif /* CONSOLE_COLOR_H_ */
