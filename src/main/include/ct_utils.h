/*
 * ct_utils.h
 *
 *  Created on: Oct 31, 2017
 *      Author: noodles
 */

#ifndef CT_UTILS_H_
#define CT_UTILS_H_

//TODO: Maybe improve this function
/**
 * Returns the basename of a file, given its absolute path.
 * Note that this function is written specifically to fit CrashC needs: we get the file name
 * from the __FILE__ macro, which always expand to an absolute filename. Due to the fact that
 * the filename is included compile-time in the binary we also return a pointer to a specific
 * offset in the binary in order not to need any dynamic allocation
 */
char * ct_file_basename(const char * filename);


#endif /* CT_UTILS_H_ */
