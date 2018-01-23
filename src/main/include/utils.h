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


#endif /* CT_UTILS_H_ */
