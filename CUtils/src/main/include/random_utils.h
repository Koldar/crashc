/*
 * random_utils.h
 *
 *  Created on: Jul 18, 2017
 *      Author: koldar
 */

#ifndef RANDOM_UTILS_H_
#define RANDOM_UTILS_H_

#include <stdbool.h>

/**
 * Get a random integer number inside the range \f$ \[ min; max \]\f$
 *
 * \pre
 * 	\li \f$ max > min \f$;
 *
 * @param[in] min the range lowerbound
 * @param[in] max the range upper bound
 * @param[in] minIncluded true if min might be choosen in the random, false otherwise
 * @param[in] maxIncluded true if max might be choosen in the random, false otherwise
 * @return the number choosen
 */
int getRandomInRange(int min, int max, bool minIncluded, bool maxIncluded);

int getRandomValueInArray(int n, const int array[]);

/**
 * Often you need to check a condition depending on the outcome of a probability.
 * For example do something with probability 0.7. This method returns true if such probability is checked
 *
 * @param[in] probability a number between 0.0 and 1.0
 * @return
 * 	\li true if the random succeed in the probability;
 * 	\li false otherwise
 */
bool getRandomInProbability(double probability);

#endif /* RANDOM_UTILS_H_ */
