/*
 * errors.h
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#ifndef ERRORS_H_
#define ERRORS_H_

/**
 * Callback executed when a malloc returns NULL
 */
#ifndef MALLOCERRORCALLBACK
#	define MALLOCERRORCALLBACK() exit(1)
#endif



#endif /* ERRORS_H_ */
