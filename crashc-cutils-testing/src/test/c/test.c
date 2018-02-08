#include "crashC.all.in.one.h"

int Factorial( int number ) {
   return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
// return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TESTSUITE(1) {
	TESTCASE( "2: Factorial of 0 is 1 (fail)", "[multi-file:2]" ) {
	    ASSERT( Factorial(0) == 1 );
	}
}
