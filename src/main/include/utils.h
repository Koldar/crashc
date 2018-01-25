/**
 * @file
 *
 * Contains generic utilities used throughout all the project
 *
 * @author noodles
 * @date Oct 31, 2017
 */

#ifndef CT_UTILS_H_
#define CT_UTILS_H_

#include <time.h>

//TODO: Maybe improve this function
/**
 * Returns the basename of a file, given its absolute path.
 *
 * \note
 * This function is written specifically to fit @crashc needs: we get the file name
 * from the \c __FILE__ macro, which always expand to an absolute filename. Due to the fact that
 * the filename is included compile-time in the binary we also return a pointer to a specific
 * offset in the binary in order not to need any dynamic allocation
 *
 * TODO is this example correct?
 * For example, if the input is <tt>/home/koldar/crashc/mytest.c</tt> the return value will be <tt>mytest.c</tt>
 *
 * @param[in] filename the absolute filename returned by \c __FILE__
 * @return the basename of the absolute file
 */
char* const ct_file_basename(char* const filename);

/**
 * Get the time *now*
 *
 * @return struct holding information about the time when this function has been called
 */
struct timespec ct_get_time();
/**
 * Get a time interval
 *
 * @param[in] start the start of the time interval
 * @param[in] end the end of the time interval
 * @param[in] format_str either one of the following options:
 * 	\li "s" if you want to the time interval in seconds;
 *  \li "m" if you want to the time interval in milliseconds;
 *  \li "u" if you want to the time interval in microseconds;
 *  \li "n" if you want to the time interval in nanoseconds;
 * @return the time elapsed between \c start and \c end
 * @see ct_get_time
 */
long ct_compute_time_gap(struct timespec start, struct timespec end, const char* format_str);



#endif /* CT_UTILS_H_ */
