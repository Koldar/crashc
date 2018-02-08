/*
 * math_utils.c
 *
 *  Created on: Sep 19, 2017
 *      Author: koldar
 */

#include "math_utils.h"
#include "macros.h"

int getBinomialCoefficient(int n, int k) {
	int nFact = getFactorial(n);
	int kFact = getFactorial(k);
	int n_kFact = getFactorial(n - k);

	//this is a really naive implementation. A more accurate implementation would be to delete sahred factorials sectors
	return nFact / (kFact * n_kFact);
}

int getBinomialCoefficientOf2(int n) {
	return (n*(n-1))/2;
}

int getFactorial(int n) {
	//we do no use recursion to avoid unecessary stacks/unstacks
	int retVal = 1;
	for (int i=n; i>1; i--) {
		retVal *= i;
	}
	return retVal;
}
