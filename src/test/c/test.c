/*
 * test.c
 *
 *  Created on: Jan 23, 2017
 *      Author: koldar
 */

#include "crashC.h"

int main(int argc, const char* argv[]) {

	TESTCASE("testcase") {
		printf("before when1\n");
		WHEN("when1") {
			printf("when1\n");
		}
		printf("when1 -> when2\n");
		WHEN("when2") {
			printf("when2\n");
		}
		printf("when2 -> when3\n");
		WHEN("when3") {
			printf("when3\n");
		}
		printf("after when3\n");
	}

	return 0;
}
