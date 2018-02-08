/*
 * space_measurement.c
 *
 *  Created on: Nov 27, 2017
 *      Author: koldar
 */

#include "space_measurement.h"

#include <sys/types.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "log.h"
#include "macros.h"

static int parseLine(char* line);

#define STATUS_FILE "/proc/self/status"

mem_size_t convertSpaceConsumptionInto(mem_size_t from, space_unit_enum_t unit) {
	for (int i=0; i<unit; i++) {
		from /= 1000;
	}
	return from;
}

double convertSpaceConsumptionIntoDouble(mem_size_t from, space_unit_enum_t unit) {
	double retVal = from;
	for (int i=0; i<unit; i++) {
		retVal /= 1000.0f;
	}
	return retVal;
}

mem_size_t getTotalVirtualMemory() {
	struct sysinfo memInfo;
	sysinfo(&memInfo);

	mem_size_t totalVirtualMem = (mem_size_t) memInfo.totalram;
	totalVirtualMem += (mem_size_t) memInfo.totalswap;
	totalVirtualMem *= (mem_size_t) memInfo.mem_unit;
	critical("mem unit %ld", memInfo.mem_unit);

	return totalVirtualMem;
}

mem_size_t getVirtualMemoryUsed() {
	struct sysinfo memInfo;

	sysinfo(&memInfo);
	mem_size_t virtualMemUsed = (mem_size_t)memInfo.totalram - (mem_size_t)memInfo.freeram;
	virtualMemUsed += (mem_size_t)memInfo.totalswap - (mem_size_t)memInfo.freeswap;
	virtualMemUsed *= (mem_size_t)memInfo.mem_unit;
	critical("mem unit %ld", memInfo.mem_unit);

	return virtualMemUsed;
}

mem_size_t getVirtualMemoryUsedByCurrentProcess() {
	FILE* file = fopen(STATUS_FILE, "r");
	int result = 0;
	char line[128];
	bool found = false;

	while (fgets(line, 128, file) != NULL){
		if (strncmp(line, "VmSize:", 7) == 0){
			//Note: this value is in KB!
			result = (mem_size_t) parseLine(line);
			result *= 1000;
			found = true;
			break;
		}
	}
	fclose(file);

	if (!found) {
		ERROR_NOT_SUCH_OBJECT("VmSize line in " STATUS_FILE "not found", 7);
	}
	return result;
}

mem_size_t getTotalPhysicalMemory() {
	struct sysinfo memInfo;
	sysinfo(&memInfo);
	mem_size_t totalPhysMem = (mem_size_t) memInfo.totalram;
	totalPhysMem *= memInfo.mem_unit;
	critical("mem unit %ld", memInfo.mem_unit);

	return totalPhysMem;
}

mem_size_t getPhysicalMemoryUsed() {
	struct sysinfo memInfo;
	sysinfo(&memInfo);

	mem_size_t physMemUsed = (mem_size_t)memInfo.totalram - (mem_size_t)memInfo.freeram;
	physMemUsed *= memInfo.mem_unit;
	critical("mem unit %ld", memInfo.mem_unit);
	return physMemUsed;
}

mem_size_t getPhysicalMemoryUsedByCurrentProcess() {
	FILE* file = fopen(STATUS_FILE, "r");
	mem_size_t result = 0;
	char line[128];
	bool found = false;;

	while (fgets(line, 128, file) != NULL){
		if (strncmp(line, "VmRSS:", 6) == 0){
			//Note: this value is in KB!
			result = (mem_size_t) parseLine(line);
			result *= 1000;
			found = true;
			break;
		}
	}
	fclose(file);

	if (!found) {
		ERROR_NOT_SUCH_OBJECT("VmRSS line in " STATUS_FILE "not found", 7);
	}
	return result;
}

/**
 * Parse the number from a string whose pattern is <tt>^[^0-9]+([0-9]+) Kb$</tt>
 *
 * Example of string may be
 * <p>
 * 	<tt>VmSize:     9660 kB</tt>
 * </p>
 *
 * @param[in] line the line to parse
 * @return the number within such line
 */
static int parseLine(char* line) {
	int i = strlen(line);
	const char* p = line;
	//find the first number in the line
	while (*p <'0' || *p > '9') {
		p++;
	}
	// This assumes that a digit will be found and the line ends in " Kb".
	line[i-3] = '\0';
	critical("fetched \"%s\"", p);
	i = atoi(p);
	return i;
}
