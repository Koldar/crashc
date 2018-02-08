/*
 * test_checker.h
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#ifndef TEST_CHECKER_H_
#define TEST_CHECKER_H_

#include <stdbool.h>

#include "main_model.h"

/**
 * Prepare the test check to be used in a new crash c test
 */
void clear_test_checker();

/**
 * Add a character into the test checker
 *
 * @param[in] ch the character to add
 */
void add_char(char ch);

/**
 * Adds a string inside the test checker
 *
 * \note
 * it will also add the character set by ::set_separator if specified, substituting the added string NULL byte
 *
 * @param[in] str the string to add in the test checker
 */
void add_str(const char* str, bool add_split_char);

/**
 * Set the separator character
 *
 * @param[in] s the character to add everytime a string is added
 */
void set_separator(char s);

/**
 *	check if the test check is **exactly** populate like expected
 *
 *	@param[in] expected the string to check the test checker against
 *	@return
 *		\li true if the 2 strings are the same;
 *		\li false otherwise;
 */
bool check_test_checker(const char* expected);

/**
 * like ::check_test_checker but it will also call ::clear_test_checker
 */
bool check_and_reset_test_checker(const char* expected);

/**
 * like ::check_and_reset_test_checker but it throws an assert error if the 2 strings doesn't match
 *
 * @param[in] expected the string to check the test c hecker against
 */
void assert_and_reset_test_checker(const char* expected);


/**
 * Updates the model in order to use the testing version of the report producer
 */
void setup_testing_producer(struct ct_model* model);

#endif /* TEST_CHECKER_H_ */
