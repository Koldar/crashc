/*
 * macros.h
 *
 *  Created on: Jun 29, 2017
 *      Author: koldar
 */

#ifndef MACROS_H_
#define MACROS_H_


#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define UV(v) TOKENPASTE2(v, __LINE__)


#endif /* MACROS_H_ */
