#include "catch.hpp"

int Factorial( int number ) {
   //return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
   return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE( "2: Factorial of 0 is 1 (fail)", "[multi-file:2]" ) {
    REQUIRE( Factorial(0) == 1 );
}
