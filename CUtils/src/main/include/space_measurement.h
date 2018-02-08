/*
 * \file space_measurement.h
 *
 * The module allows you to fetch information about the memory your program is currently consuming
 *
 * \note
 * The module supports only linux for the moment and is based upon the information fetched in <a href="https://stackoverflow.com/a/64166/1887602">this SO answer</a>.
 *
 *  Created on: Nov 27, 2017
 *      Author: koldar
 */

#ifndef SPACE_MEASUREMENT_H_
#define SPACE_MEASUREMENT_H_

#include <stdbool.h>
#include "macros.h"

/**
 * Represents a measurement of memory. The type refers to bytes.
 */
typedef unsigned long long mem_size_t;

/**
 * An enum used to specify which space measurement you want
 * in the macro
 */
typedef enum {
	SME_USED_VIRTUAL,
	SME_PROCESS_USED_VIRTUAL,
	SME_USED_PHYSICAL,
	SME_PROCESS_USED_PHYSICAL
} space_measurement_enum_t;

typedef enum {
	SUE_BYTE = 0,
	SUE_KB = 1,
	SUE_MB = 2,
	SUE_GB = 3,
} space_unit_enum_t;

/**
 * Convert a specified amount of memory into another one
 *
 * @param[in] from the space amount to convert
 * @param[in] unit the unit measure you would like to convert \c from to
 * @return the amount of memory in \c unit
 */
mem_size_t convertSpaceConsumptionInto(mem_size_t from, space_unit_enum_t unit);

/**
 * like ::convertSpaceConsumptionInto but returns a double, not an integer
 *
 * @param[in] from the space amount to convert
 * @param[in] unit the unit measure you would like to convert \c from to
 * @return the amount of memory in \c unit
 */
double convertSpaceConsumptionIntoDouble(mem_size_t from, space_unit_enum_t unit);

/**
 *
 * \note
 * By virtual memory we mean we consider both RAM and SWAP
 *
 * @return the total virtual memory available on the system
 */
mem_size_t getTotalVirtualMemory();

/**
 * \note
 * By virtual memory we mean we consider both RAM and SWAP
 *
 * @return the total virtual memory used right now
 */
mem_size_t getVirtualMemoryUsed();

/**
 * \note
 * By virtual memory we mean we consider both RAM and SWAP
 *
 * @return the total virtual memory used by the current process right now
 */
mem_size_t getVirtualMemoryUsedByCurrentProcess();

/**
 * @return the total physical memory available on the system
 */
mem_size_t getTotalPhysicalMemory();

/**
 * @return the total physical memory used right now
 */
mem_size_t getPhysicalMemoryUsed();

/**
 * @return the total physical memory used by the current process right now
 */
mem_size_t getPhysicalMemoryUsedByCurrentProcess();

/**
 * The macro will invoke a function of this module depending of \c sme value.
 *
 * You can use the macro inside statements as well
 *
 * @param[in] sme a variable of type ::space_measurement_enum_t
 */
#define _INVOKE_SPACE_FUNCTION(sme)																\
	(sme == SME_USED_VIRTUAL ? getVirtualMemoryUsed() : (										\
		sme == SME_PROCESS_USED_VIRTUAL ? getVirtualMemoryUsedByCurrentProcess() : (			\
			sme == SME_USED_PHYSICAL ? getPhysicalMemoryUsed() : (								\
				sme == SME_PROCESS_USED_PHYSICAL ? getPhysicalMemoryUsedByCurrentProcess() : 	\
					getVirtualMemoryUsedByCurrentProcess()										\
				)																				\
			)																					\
		)																						\
	)																							\

/**
 * Detect how much space the operation takes in order to be completed
 *
 * Usage of the macro is the following one:
 * @code
 *  void* p = NULL;
 * 	SPACE_PROFILE_CODE(spaceOccupied, SME_PROCESS_USED_VIRTUAL, SUE_BYTE) {
 * 		p = malloc(3 * 1024 * sizeof(char));
 * 	}
 * 	if (p != NULL) {
 * 		free(p);
 * 	}
 * 	printf("the instructions within SPACE_PROFILE_CODE occupied %llu bytes\n.", spaceOccupied);
 * @endcode
 *
 * @param[in] spaceVar represents the name of the variable of type \c double that will contain the space consumption the operations inside
 * 	the macro will took. \c spaceVar will be a **delta** of space consumed, namely space occupied after the operation minus the space occiupied before it.
 * @param[in] _sme a variable of type ::space_measurement_enum_t representing what space you want to compute
 * @param[in] _unit a variable of type ::space_unit_enum_t representing wether the space consumption is needdd in byte, kilobyte or something else.
 */
#define SPACE_PROFILE_CODE(spaceVar, _sme, _unit) \
	double spaceVar; \
	for (bool UV(loop)=true; UV(loop); ) \
		for (space_unit_enum_t UV(unit) = _unit; UV(loop);) \
			for (space_measurement_enum_t UV(sme) = _sme; UV(loop); ) \
				for ( \
					mem_size_t UV(startSpace) = _INVOKE_SPACE_FUNCTION(UV(sme)),\
					UV(endSpace)=0 \
					; \
					UV(loop) \
					; \
					UV(loop)=false, \
					UV(endSpace)=_INVOKE_SPACE_FUNCTION(UV(sme)), \
					critical("sme %d start %llu end %llu", UV(sme), UV(startSpace), UV(endSpace)), \
					spaceVar = convertSpaceConsumptionIntoDouble(UV(endSpace) - UV(startSpace), UV(unit)) \
				) \

#endif /* SPACE_MEASUREMENT_H_ */
