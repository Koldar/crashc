Introduction
============

This page will explain how to use crashC with multiple files.

Multiple files on crashC
========================

Setupping multiple files in crashC is super easy! You **need** to have a "register" source file containing `TESTS_END` and `TESTS_START` macros.
There you normally add all the testsuites as per usual: be sure to include "crashC" header in this "register" source file! For example:

    //this is the file allTests.c (aka the "register" source file)
    #include<crashC.h>
    
    TESTS_START {
        REGISTER_SUITE(1);
        REGISTER_SUITE(2);
    }
    
Then you need to add a source file per suite. Actually a source file is allowed to contain as many suites as you want; the "one source file per suite" is more a constraint to improve project structure than a crashC requirements. Nonetheless, Here's an of a source file *suite1.c*:

    //this is suite1.c file
    #include<crashC.h>
    
    TESTSUITE(1) {
        assert(1==1);
    }
    
and *suite2.c*:

    //this is suite2.c
    #include<crashC.h>

    TESTSUITE(2) {
        assert(2==2);
    }
    
CrashC doesn't look the names of the files, so you can name them whatever you want. All its needed is that your build process correctly compile all of them!
For example a building process might be:

    gcc -c allTest.c -o allTest.o
    gcc -c suite1.c -o suite1.o
    gcc -c suite2.c -o suite2.o
    gcc suite1.o suite2.o allTest.o -o Test
    
And that's it!

What's next?
============

Like in [Catch Cpp](https://github.com/philsquared/Catch), CrashC has tags allowing you to quickly customize which tests to run and which tests you want to skip. Want to know more? Go to the next [page]().
