Introduction
============

In this section we will look at how we can code tests with CrashC.

Hello Test World!
=================

The following code is bare minimum you need to use crashC. As you can see, you can setup tests really quickly!
The boilerplate code is reduced to the minimum as well.

    #include<crashC.h>
    
    TESTS_START
    REGISTER_SUITE(1)
    TESTS_END
    
    TESTSUITE(1) {
    //code your tests here!
    assert(1==1);
    }
    
Follows a brief description of what we're  doing:

- ´#include<crashC.h>´: mandatory if you want to use crashC!
- ´TESTS_START´ defines what **test suites** you want to run. Put all the suites you want inside it;
- ´REGISTER_SUITE(1)´: mark the given suite as "wants to be run";
- ´TESTSUITE(1) {´: declare what is the code to run in the ´suite(1)´;

The rest of the example is pretty much self-explanatory. Briefly, we create a test suite, and we tell crashC to run it.
From this example it seems crashC is not different from any other C testing framework you came across. As you will realize, suites **aren't** 
the most important concepts in crashC, **section are**. Understand what I mean by that in the (following page)[].
