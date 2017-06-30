/*
 * test.c
 *
 *  Created on: Jan 23, 2017
 *      Author: koldar
 */

#include "crashC.h"
#include "assertions.h"
#include "reportProducer.h"
#include <string.h>


//
//SUITE() {
//
//	TESTCASE("debug - long - optional") {
//		WHEN() {
//			THEN() {
//
//			}
//			 THEN() {
//
//			 }
//		}
//	}
//
//	TESTCASE() {
//
//	}
//
//	TESTCASE() {
//
//	}
//}

#ifdef TEST_1

static bufferId = 0;
static buffer2[1000];

#define isprintf(buffer, format, ...) {				\
	sprintf(buffer2, format, ##__VA_ARGS__);		\
	sprintf(buffer + bufferId, "%s", buffer2);		\
	bufferId += strlen(buffer2);					\
}

void resetBuffer() {
	bufferId = 0;
}

//typedef void test_function(void);
//
//#define _TEST(name, x)			\
//		void name();			\
//		x						\
//		void name()
//
//#define TEST(name) _TEST(name, int _ ## name = 0;)
//
//
//TEST(a) {
//	printf("a\n");
//}
//
//TEST(b) {
//	printf("b\n");
//}
//
//TEST(c) {
//	printf("c\n");
//}
//
//int main(int argc, const char* argv[]) {
//	a();
//	b();
//	c();
//	_a;
//}

int main(int argc, const char* argv[]) {

	char buffer[1000];

	TESTCASE("testcase", "") {
		isprintf(buffer, "a");
		WHEN("when1", "") {
			isprintf(buffer, "b");
		}
		isprintf(buffer, "c");
		WHEN("when2", "") {
			isprintf(buffer, "d");
		}
		isprintf(buffer, "e");
		WHEN("when3", "") {
			isprintf(buffer, "f");
		}
		isprintf(buffer, "g");
	}
	assert( strcmp(buffer, "abcegacdegacefg") == 0);
	resetBuffer();

	TESTCASE("testcase 1", "") {
		isprintf(buffer, "a");
		WHEN("when 1", "") {
			isprintf(buffer, "b");
			THEN("then 1", "") {
				isprintf(buffer, "c");
			}
			isprintf(buffer, "d");
			THEN("then 2", "") {
				isprintf(buffer, "e");
			}
			isprintf(buffer, "f");
		}
		isprintf(buffer, "g");
		WHEN("when 2", "") {
			isprintf(buffer, "h");
			THEN("then 3", "") {
				isprintf(buffer, "i");
			}
			isprintf(buffer, "j");
			THEN("then 4", "") {
				isprintf(buffer, "k");
			}
			isprintf(buffer, "l");
			THEN("then 5", "") {
				isprintf(buffer, "m");
			}
			isprintf(buffer, "n");
		}
		isprintf(buffer, "o");
		WHEN("when 3", "") {
			isprintf(buffer, "p");
		}
		isprintf(buffer, "q");
	}
	assert( strcmp(buffer, "abcdefgoqaghijklmnoqagopq") == 0);
	resetBuffer();

	TESTCASE("testcase 2", "") {
		isprintf(buffer, "a");
		WHEN("when 4", "") {
			isprintf(buffer, "b");
			THEN("then 6", "") {
				isprintf(buffer, "c");
			}
			isprintf(buffer, "d");
		}
		isprintf(buffer, "e");
	}
	assert( strcmp(buffer, "abcde") == 0);
	resetBuffer();

	TESTCASE("testcase 3", "") {
		isprintf(buffer, "a");
		WHEN("when 5", "") {
			isprintf(buffer, "b");
		}
		isprintf(buffer, "c");
	}
	assert( strcmp(buffer, "abc") == 0);
	resetBuffer();

	TESTCASE("testcase 4", "") {
		isprintf(buffer, "a");
		THEN("then 7", "") {
			isprintf(buffer, "b");
		}
		isprintf(buffer, "c");
	}
	assert( strcmp(buffer, "abc") == 0);
	resetBuffer();

	TESTCASE("testcase 5", "") {
		isprintf(buffer, "a");
	}
	assert( strcmp(buffer, "a") == 0);
	resetBuffer();

	printf("DONE\n");
	//printSectionData(&rootSection, true);

	produceReport(consoleProducer, &rootSection);

	destroySection(&rootSection);
	return 0;
}

#endif //ifdef test_1

#ifndef TEST_1

TESTS_START

REGTESTS(1, 2)

TESTS_END

#endif
