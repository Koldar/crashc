Introduction
============

In the following page we will show the **sections**, namely the main feature crashC has. I promise: this page is long onlly because the initial testing code is long (and the testing code is really super easy, so you don't have to stress yourself understanding cryptic code!).

What are sections?
==================

Sections allows you to physically compress test source code. While testing, it often happens copy-paste testing source code because you want
to test pretty similar scenarios but a little bit different, one from another.
If this behaviour remain uncheked, testing source code may become big and unmantainable. The testing framework should provide you
a way to avoid such copy-pasting.

CrashC answer to this problem are **sections**. Note that this is by no means a *novel idea*: (https://github.com/philsquared/Catch)[Catch]
has this mechanism from a long time. However, nothing similar existed for C (at least, not in a pure way). If you know Catch Cpp, you'll find
coding in crashC to be somewhat similar.

Ok, enough of this chit chats! Let's see an example!

Section example
===============

Assume you're testing  a list implementation:


    #include <crashC.h>

    TESTS_START
    REGISTER_SUITE(1);
    TESTS_END

    TESTSUITE(1) {
        //testing init and destroy
        list* l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        destroyList(l);
    
        //testing addTail function
        l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        addTail(l, 5);
        assert(!isEmpty(l));
        assert(getSize(l)==1);
        destroyList(l);
    
        //testing addHead function
        l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        addHead(l, 3);
        assert(!isEmpty(l));
        assert(getSize(l)==1);
        destroyList(l);
    
        //testing  multiple addTail and addHead
        l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        addTail(l, 5);
        addHead(l, 3);
        addTail(l, 7);
        assert(!isEmpty(l));
        assert(getSize(l)==3);
        destroyList(l);
    
        //testing isPresent function
        l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        addTail(l, 5);
        addHead(l, 3);
        addTail(l, 7);
        assert(isPresent(l, 7));
        assert(!isPresent(l, 4));
        destroyList(l);
    
        //testing removal successful
        l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        addTail(l, 5);
        addHead(l, 3);
        addTail(l, 7);
        removeItem(l, 7);
        assert(!isPresent(l, 7));
        assert(!isEmpty(l));
        assert(getSize(l)==2);
        destroyList(l);
        
        //testing removal unsuccessful
        l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        addTail(l, 5);
        addHead(l, 3);
        addTail(l, 7);
        removeItem(l, 4);
        assert(!isPresent(l, 4));
        assert(!isEmpty(l));
        assert(getSize(l)==3);
        destroyList(l);
    }

As you might see there is a lot of redundant code. Testing software often has it: you *need* to be sure that the software behaves exactly as you think.
We can use this (toy) example to showcase sections. 

Testcase section
----------------

First of all `TESTCASE`: while `TESTSUITE` defines a **test suites**, `TESTCASE` defines
a series of test somewhat similar among them.

    #include <crashC.h>

    TESTS_START
    REGISTER_SUITE(1);
    TESTS_END

    TESTSUITE(1) {

        TESTCASE("list", "") {
            //testing init and destroy
            list* l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            destroyList(l);

            //testing addTail function
            l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            addTail(l, 5);
            assert(!isEmpty(l));
            assert(getSize(l)==1);
            destroyList(l);

            //testing addHead function
            l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            addHead(l, 3);
            assert(!isEmpty(l));
            assert(getSize(l)==1);
            destroyList(l);

            //testing  multiple addTail and addHead
            l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            addTail(l, 5);
            addHead(l, 3);
            addTail(l, 7);
            assert(!isEmpty(l));
            assert(getSize(l)==3);
            destroyList(l);

            //testing isPresent function
            l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            addTail(l, 5);
            addHead(l, 3);
            addTail(l, 7);
            assert(isPresent(l, 7));
            assert(!isPresent(l, 4));
            destroyList(l);

            //testing removal successful
            l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            addTail(l, 5);
            addHead(l, 3);
            addTail(l, 7);
            removeItem(l, 7);
            assert(!isPresent(l, 7));
            assert(!isEmpty(l));
            assert(getSize(l)==2);
            destroyList(l);

            //testing removal unsuccessful
            l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            addTail(l, 5);
            addHead(l, 3);
            addTail(l, 7);
            removeItem(l, 4);
            assert(!isPresent(l, 4));
            assert(!isEmpty(l));
            assert(getSize(l)==3);
            destroyList(l);
        }
    }

Ok, really cool but nothing actually changed! Hell, we even increase the total line number in the source code!
However, using `TESTCASE` allows you to create indipendent pieces of tests within the same test suite:
at the end, *test suites* are meant as containers of *test cases*. You can view the code as:

> A test suite contains several test cases. Each test case tests something.

You are encouraged to create as many `TESTCASE` as you want: you want to create `TESTCASE` because **only inside them** you can exploit all other sections Crashc provides.

When section
------------

You can start compressing code by using `WHEN`. The previous example is encoded in crashC as:

    #include <crashC.h>

    TESTS_START
    REGISTER_SUITE(1);
    TESTS_END

    TESTSUITE(1) {
        TESTCASE("list", "") {
            list* l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            WHEN("adding on tail", "") {
                addTail(l, 5);
                assert(!isEmpty(l));
                assert(getSize(l)==1);
            }
            WHEN("adding on head", "") {
                addHead(l, 3);
                assert(!isEmpty(l));
                assert(getSize(l)==1);
            }
            WHEN("lists of 3 items", "") {
                addTail(l, 5);
                addHead(l, 3);
                addTail(l, 7);
                assert(!isEmpty(l));
                assert(getSize(l)==3);    
                WHEN("checking presence", "") {
                    assert(isPresent(l, 7));
                    assert(!isPresent(l, 4));
                }  
                WHEN("removing present item", "") {
                    removeItem(l, 7);
                    assert(!isPresent(l, 7));
                    assert(!isEmpty(l));
                    assert(getSize(l)==2);
                }
                WHEN("removing absent item", "") {
                    removeItem(l, 4);
                    assert(!isPresent(l, 4));
                    assert(!isEmpty(l));
                    assert(getSize(l)==3);
                }
            }
            destroyList(l);
        }
    }

The code below is perfectly equivalent to the one in the first example. **Such compactness! Much Wow!**
But how does it achieve it?
Code inside a `WHEN` defines **an hypotetic use case while its body represents what happens when such use case to test happens**.
CrashC (behind the hood) tries to test all the hypotetic use case you provide with `WHEN`, one per time. Code inside `TESTCASE` is always
executed so you can put code in the `TESCASE` to setup and teardown resources (see `l` variable).

So, the first 2 `WHEN` of `TESTCASE` represents 2 different and **separate** use cases:

- WHEN we're adding on tail;
- WHEN we're adding on head;

If there are nested `WHEN`, they represent combinations of scenarios. For example the last `WHEN` contains 3 different use cases:

- WHEN there is a lists of 3 items AND we're checking presence do;
- WHEN there is a lists of 3 items AND removing present item do;
- WHEN there is a lists of 3 items AND removing absent item do;

Note that the code inside "lists of 3 items" is executed for every of those 3 scenarios!

Then
----

We can improve readbiliy by adding the `THEN` section, thus *somewhat* achieving a pseudo BDD-style:

    #include <crashC.h>

    TESTS_START
    REGISTER_SUITE(1);
    TESTS_END

    TESTSUITE(1) {
        TESTCASE("list", "") {
            list* l = initList();
            assert(isEmpty(l));
            assert(getSize(l)==0);
            WHEN("adding on tail", "") {
                addTail(l, 5);
                THEN("size increments","") {
                    assert(!isEmpty(l));
                    assert(getSize(l)==1);
                }
            }
            WHEN("adding on head", "") {
                addHead(l, 3);
                THEN("size increments", "") {
                    assert(!isEmpty(l));
                    assert(getSize(l)==1);
                }
            }
            WHEN("lists of 3 items", "") {
                addTail(l, 5);
                addHead(l, 3);
                addTail(l, 7);
                THEN("size is 3", "") {
                    assert(!isEmpty(l));
                    assert(getSize(l)==3);    
                }
                WHEN("checking presence", "") {
                    assert(isPresent(l, 7));
                    assert(!isPresent(l, 4));
                }  
                WHEN("removing present item", "") {
                    removeItem(l, 7);
                    THEN("size decrements", "") {
                        assert(!isPresent(l, 7));
                        assert(!isEmpty(l));
                        assert(getSize(l)==2);
                    }
                }
                WHEN("removing absent item", "") {
                    removeItem(l, 4);
                    THEN("size is unchanged", "") {
                        assert(!isPresent(l, 4));
                        assert(!isEmpty(l));
                        assert(getSize(l)==3);
                    }
                }
            }
            destroyList(l);
        }
    }

`THEN` sections are useful to improve test readability.

What's next?
============

The core or crashC is explained. For a small project this is enough.
However, if you want to extensively use crashC, you need to learn other stuff: don't worry, it's super easy stuff, no rocket science involved!
Want to know more? See the (next page)[https://github.com/Koldar/crashc/blob/master/wiki/04_MultipleFiles.md].
