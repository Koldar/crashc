# crashc

Building
--------

    mkdir -p build
    cd build
    cmake ..
    make
    #launch tests
    ./Test
    
If you want to test memory leaks, just use:
    
    valgrind ./Test    
    
Testing
-------

CrashC tests are all different programs, each of them with their own main funciton. So, automated testing needs to
compile the framework with different flags. 

We have created a *bash* script that automatically builds and tests each source file. To use it, do the following_

	cd <crashC main directory>
	src/test/resources/testEverything.bash
	
If sdomething goes wrong you will see an error message.

Developers
==========

This section is related to any developer who wants to contribute to the framework

Testing
-------

If you want to add an automated test checking a particular feature, be sure to do the following:

 * add a file inside "src/test/c" called "test_issueXXXX.c" where "XXXX" is the id of the issue from github. If an issue
 	needs to use multiple files, label each file as "test_issueXXXX.c" (the first one) "test_issueXXXXb.c", "tesst_issueXXXXc.c" and so on;
 * Each source file inside "test_issueXXXX" needs to be guarded with a "#ifdef TEST_XXXX" where "XXXX" is the issue number on github to check;
 
Please look at "test_issue0001.c" and "test_issue0002b.c" for a working example. 

 
    
