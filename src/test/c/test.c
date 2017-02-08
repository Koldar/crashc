/*
 * test.c
 *
 *  Created on: Jan 23, 2017
 *      Author: koldar
 */

#include "crashC.h"

int main(int argc, const char* argv[]) {

//	TESTCASE("testcase") {
//		printf("before when1\n");
//		WHEN("when1") {
//			printf("when1\n");
//		}
//		printf("when1 -> when2\n");
//		WHEN("when2") {
//			printf("when2\n");
//		}
//		printf("when2 -> when3\n");
//		WHEN("when3") {
//			printf("when3\n");
//		}
//		printf("after when3\n");
//	}
//
//	printf("testcase has ssuccessfully completed\n");

	TESTCASE("testcase 1") {
		WHEN("when 1") {
			THEN("then 1") {

			}
			THEN("then 2") {

			}
		}
		WHEN("when 2") {
			THEN("then 3") {

			}
			THEN("then 4") {

			}
			THEN("then 5") {

			}
		}

		WHEN("when 3") {

		}
	}

	TESTCASE("testcase 2") {
		WHEN("when 4") {
			THEN("then 6") {

			}
		}
	}

	TESTCASE("testcase 3") {
		WHEN("when 5") {

		}
	}

	TESTCASE("testcase 4") {
		THEN("then 7") {

		}
	}

	TESTCASE("testcase 5") {

	}

	printf("DONE\n");
	//printSectionData(&rootSection, true);

	return 0;
}
