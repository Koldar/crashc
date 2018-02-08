/*
 * math_utils.h
 *
 *  Created on: Sep 19, 2017
 *      Author: koldar
 */

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

/**
 * Compute \f${{n}\choose{k}}\f$
 *
 * @param[in] n the number of element choosable
 * @param[in] k the size each group chosen must have
 * @return the binomial coefficient
 */
int getBinomialCoefficient(int n, int k);

/**
 * like ::getBinomialCoefficient but performs \f${{n}\choose{2}}\f$
 *
 * @param[in] n the number of element choosable
 */
int getBinomialCoefficientOf2(int n);

/**
 * COmputes the factorial of a number
 *
 * @param[in] n the number whose factorial we need to compute
 * @return the factorial of the number
 */
int getFactorial(int n);

#endif /* MATH_UTILS_H_ */
